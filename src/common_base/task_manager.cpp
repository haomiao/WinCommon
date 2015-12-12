#include "stdafx.h"
#include <process.h>
#include "auto_lock.h"
#include "task_manager.h"


CTaskManager::CTaskManager():m_taskLock(3000)
{ 
    Init(); 
    m_taskEvent = ::CreateEvent(NULL, FALSE, TRUE, NULL);
}


CTaskManager::~CTaskManager()
{ 
    ShutdownForce();
    ::CloseHandle(m_taskEvent);
    m_taskQueue.clear();
}


void CTaskManager::Init()
{
    m_bManual   = FALSE;
    m_exited    = FALSE;
    m_taskId    = 1;
    m_hThread   = (HANDLE)-1;
    m_taskEvent = NULL;
}


BOOL CTaskManager::Startup(int priority, BOOL bManual)
{
	if(IsStarted() == TRUE)
    {
		return TRUE;
    }

	m_hThread = (HANDLE)_beginthreadex(NULL, 0, TaskThread, this, CREATE_SUSPENDED, NULL);
	if(m_hThread != (HANDLE)-1)
	{
		::SetThreadPriority(m_hThread, priority);
        m_bManual = bManual;
        if (m_bManual == TRUE)
        {
            ::ResetEvent(m_taskEvent);
        }
		::ResumeThread(m_hThread);
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}


unsigned CTaskManager::TaskThread(LPVOID lpVoid)
{
	CTaskManager *pTM = (CTaskManager*)lpVoid;
    if (pTM == NULL)
    {
        return 0;
    }
    return pTM->Run();
}


unsigned int CTaskManager::Run()
{
	while(true)
	{
		//  �ȴ�����ִ������
        if (::WaitForSingleObject(m_taskEvent, INFINITE) != WAIT_OBJECT_0 || IsExited() == TRUE )
        {
            break;
        }

		CTaskBase *pTask;

		while( FALSE == IsExited() && ( (pTask = GetPopTask() ) != NULL) )
		{
			//  ��������(���ȿ����Ƿ�Ϊ�����߳�)
			if(pTask->IsUITask() == FALSE)
			{
				// ��TaskProcess�������򽻸�TaskNotifyͨ�洦��
				if(pTask->TaskProcess(pTask) == FALSE)
				{
					pTask->TaskNotify(pTask, CTaskBase::TE_Completed);
				}
			}
            else
			{
				// ��ҪUI�̴߳��������,תUI�߳�
                UINT UIMsg;
                pTask->GetUITaskFlag(UIMsg);
                if (pTask->GetUIMsgCall() == TRUE)
                {
                    pTask->TaskSendUIMsg(UIMsg, (WPARAM)(pTask), (LPARAM)(this) );
                }
                else
                {
                    //  TaskPostUIMsg:���ڷ���,�˴���ʱȡ��
                    pTask->TaskPostUIMsg(UIMsg, (WPARAM)(pTask), (LPARAM)(this) );
                }
			}
            pTask->Release();
		}
	}

	return 0;
}


BOOL CTaskManager::ShutdownForce(DWORD millisec)
{
	if( !IsStarted() )
    {
		return TRUE;
    }

	// �˳�
	m_exited = TRUE;
	SetTaskEvent();

	// �ȴ������߳��˳�
    DWORD ret = ::WaitForSingleObject(m_hThread, millisec);
	if(ret == WAIT_TIMEOUT)
    {
        ::TerminateThread(m_hThread, 0);
        ::WaitForSingleObject(m_hThread, INFINITE);
    }

    Clear();

	return TRUE;
}


BOOL CTaskManager::ShutdownSafe()
{
    if( !IsStarted() )
    {
        return TRUE;
    }

    // �˳�
    m_exited = TRUE;
    SetTaskEvent();

    // �ȴ������߳��˳�
    if(::WaitForSingleObject(m_hThread, INFINITE) == WAIT_OBJECT_0)
    {
        Clear();
    }

    return TRUE;
}


void CTaskManager::Clear()
{
    //  �����߳���Դ
    ::CloseHandle(m_hThread);

    // ����������
    ClearTaskQueue();

    //  ���³�ʼ��
    Init();
}


void CTaskManager::InsertTaskQueue(CTaskBase *pTask)
{
	m_taskQueue.push_back(pTask);

	//
	// ��β��ͷ����������(���Ż����ɿ��ǽ������Ȩ�����ʹ�á�����Ȩ�����Ȳ��Զ�̬����)
	//
	TaskIter it = --m_taskQueue.end();
	TaskIter ed = m_taskQueue.begin();
	for(; it != ed; --it)
	{
		TaskIter next = it;
        --next;
		if((*it)->GetPriority() < (*next)->GetPriority())
		{
			CTaskBase *pTask = *it;
			*it = *next;
			*next = pTask;
		}
		else
		{
			break;
		}
	}
}


CTaskBase* CTaskManager::GetPopTask()
{
    CTaskBase *pTask;

    {
        AutoLock autoLock( &m_taskLock );
        if(m_taskQueue.empty())
        {
            pTask = NULL;
        }
        else
        {
            pTask = m_taskQueue.front();
            m_taskQueue.pop_front();
        }
    }
    
    return pTask;
}


void CTaskManager::ClearedTaskQueue(TaskQueue &taskQueue)
{
	TaskIter it = taskQueue.begin();
	TaskIter ed = taskQueue.end();
	for(; it != ed; ++it)
	{
        //  ������δ�����򽻸�Notifyͨ��
        if ( (*it)->TaskProcess(*it) == FALSE)
        {
            (*it)->TaskNotify(*it, CTaskBase::TE_Removed);
        }
		(*it)->Release();
	}
}


void CTaskManager::ClearTaskQueue()
{
	TaskQueue taskQueue;

    {
        AutoLock autoLock( &m_taskLock );
	    taskQueue.swap(m_taskQueue);
    }
	
	ClearedTaskQueue(taskQueue);
}


void CTaskManager::ClearKindTask(ULONG type)
{
    TaskQueue taskQueue;

    {
        AutoLock autoLock( &m_taskLock );

		TaskIter it = m_taskQueue.begin();
		TaskIter ed = m_taskQueue.end();
		for(; it != ed; )
		{
			TaskIter cur = it++;
			if((*cur)->GetType() == type)
			{
				taskQueue.push_back(*cur);
				m_taskQueue.erase(cur);
			}
		}
	}

	ClearedTaskQueue(taskQueue);
}


BOOL CTaskManager::IsStarted() const 
{ 
    return (m_hThread != (HANDLE)-1); 
}


BOOL CTaskManager::SetTaskEvent()
{
    if( !IsStarted() )
    {
        return FALSE;
    }

    ::SetEvent(m_taskEvent);
    return TRUE;
}

const CTaskManager::TaskQueue& CTaskManager::GetTaskQueue() 
{ 
    return m_taskQueue; 
}


UUID CTaskManager::AddTask(CTaskBase *pTask)
{	
    UUID uid = {0};
    memset(&uid, 0, sizeof(uid) );
    if(pTask == NULL)
    {
        return uid;
    }

    {
        AutoLock autoLock( &m_taskLock );
        pTask->AddRef();
        UuidCreate(&uid);
        pTask->SetId(uid);
        InsertTaskQueue(pTask);
    }

    if (m_bManual == FALSE)
    {
        SetTaskEvent();
    }

    return uid;
}


ULONG CTaskManager::GetCurTaskCount()
{
    ULONG count = 0;
    {
        AutoLock autoLock( &m_taskLock );
        count = static_cast<ULONG>(m_taskQueue.size() );
    }
    return count;
}


BOOL CTaskManager::IsTaskQueueEmpty()
{ 
    return (GetCurTaskCount() == 0);
}


BOOL CTaskManager::IsExited() const 
{ 
    return m_exited; 
}
