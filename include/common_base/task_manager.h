///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   task_manager.h  
/// 	@brief	��Ҫ��װ���������
///  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/01  11:26:47
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _TASK_MANAGER_H_
#define     _TASK_MANAGER_H_

#include <list>
#include "lock_critical_section.h"
#include "task_base.h"

//
// ���������
//
//  ģʽ��bManualΪFalse,��ֻҪ������������񣬼���ִ����������е����񣬺��ڼ��������Ҳ��õ�ִ��
//        ��ΪTrue����ֻ��ִ����SetTaskEvent��Ż�ִ����������е�����ֱ���������Ϊ�գ����ڼ��������
//        ��֮ǰ�����Ѿ�ִ����ɣ�����Ҫ����ִ��SetTaskEvent���ܼ���ִ����������е�����
//  
class CTaskManager
{
public:

	typedef std::list<CTaskBase*>		TaskQueue;
	typedef TaskQueue::iterator			TaskIter;
	typedef TaskQueue::const_iterator	TaskConstIter;
	
public:

	CTaskManager();

	~CTaskManager();

	//
	//  ��ָ�����߳����ȼ��������������
    //  ��ִ�����������õĽӿ�
	//
	BOOL Startup(int priority = THREAD_PRIORITY_NORMAL, BOOL bManual = FALSE);

    //  ֹͣ�����������ִ��
    //  ShutdownForce:ǿ��ֹͣ(���ܻ���ֲ���ȫ����Ϊ)
    //  ShutdownSafe:��ȫֹͣ
    //  millisec:�ȴ���ʱʱ��(msΪ��λ)
	BOOL ShutdownForce(DWORD millisec = INFINITE);
    BOOL ShutdownSafe();

    //  ����������Ƿ���
    BOOL IsStarted() const;

    //  �Ƿ����˳�����ѭ��
    BOOL IsExited() const;

	//  ��ʼִ�������¼�(һ��ֻ���ڵ�bManual == TRUE(�ֶ��������)ʱ)
	BOOL SetTaskEvent();

    //  ����������
	void ClearTaskQueue();

    //  ���ĳ�����������
	void ClearKindTask(ULONG type);

    //  ��������Ƿ�Ϊ��
	BOOL IsTaskQueueEmpty();

    //  ��ȡ��ǰ�����������
	ULONG GetCurTaskCount();

	// ����������У�ֱ�Ӳ���(����)
	const TaskQueue& GetTaskQueue();

	// ����ǰ�������ȼ�˳����뵽���������
	UUID AddTask(CTaskBase *pTask);

private:

    //  ��ʼ������
	void Init();

    //  �������
    void Clear();

    //  �����������������
    void InsertTaskQueue(CTaskBase *pTask);

    //  ��ȡ��ǰ�������
	CTaskBase* GetPopTask();

    //  ������յ�������У��õ����һ�α�ִ�еĻ���
	void ClearedTaskQueue(TaskQueue &taskQueue);

    //  ִ������ѭ��
	unsigned int Run();

    //  �����̣߳�����ִ�ж����и�������
	static unsigned __stdcall TaskThread(LPVOID lpVoid);

private:

    BOOL                    m_bManual;      //  �ֶ�ִ�б�ʶ

	volatile BOOL		    m_exited;       //  �˳���ʶ

	LONG				    m_taskId;		//  �������Id

	HANDLE				    m_hThread;		//  �����߳̾��

	HANDLE				    m_taskEvent;	//  ��������֪ͨ�¼�

	TaskQueue			    m_taskQueue;	//  �������

	CCriticalSectionLock    m_taskLock;		//  ���������

};

#endif	// _TASK_MANAGER_H_