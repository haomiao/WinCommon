#ifndef     _TASK_BASE_H_
#define     _TASK_BASE_H_

#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

class CTaskManager;

//
// �������(�ɼ̳�ʵ������������),�ṩ��Ҫ�Ľӿ�ʵ��
//
class CTaskBase
{
public:

	//
	//  ���ȼ�
	//
	enum TPriority
	{
		TP_Highest		= 0,
		TP_Above_Normal	= (1 << 0),
		TP_Normal		= (1 << 1),
		TP_Below_Normal	= (1 << 2),
		TP_Lowest		= (1 << 3),
	};

	//
	//  �¼�
	//
	enum TEvent
	{
		TE_Unknown = 0,     //  δ֪����
		TE_Canceled,        //  ��ȡ��������
		TE_Removed,         //  ���Ƴ�������
		TE_Completed,       //  ����ɵ�����
        TE_EventNum,        //  �����¼����ͼ���
	};

public:

	CTaskBase(LONG type);

	virtual ~CTaskBase() {}

public:

	//  ��������ʶ��ID
	void SetId(UUID id);
	UUID GetId() const;
    void SetId(RPC_WSTR StringUuid);
    void GetId(RPC_WSTR StringUuid) const;
    void SetId(RPC_CSTR StringUuid);
    void GetId(RPC_CSTR StringUuid) const;

	//  �������ͱ������0
	void SetType(LONG type);
	LONG GetType() const;

    //  ��������Ȩ
	void SetPriority(ULONG priority);
	ULONG GetPriority() const;

    //  ����UI�����ʶ
    //  bUITask:True��ΪUI������Ϊһ������
	void SetUITask(BOOL bUITask);
	BOOL IsUITask() const;

    //  ����UI���񴰿ھ��
    void SetUITaskHWnd(HWND hTaskWnd);
    HWND GetUITaskHWnd() const;
    
    //  ����UI������ʽ
    //  UIMsg��������Ϣ
    //  bUITaskFlag��TRUE����ʾʹ����Ϣ, False:��ʾʹ�ûص� 
    void SetUITaskFlag(UINT UIMsg, BOOL bUseMsg = FALSE);
    BOOL GetUITaskFlag(UINT &UIMsg) const;

    //  ����UI������÷�ʽ
    //  bMsgCall:TRUE,��ΪSend��False��ΪPost
    void CTaskBase::SetUIMsgCall(BOOL bMsgCall);
    BOOL CTaskBase::GetUIMsgCall() const;

	//  ���ü���
	LONG AddRef();
	LONG Release();

	// ����������¼�ͨ��
	// ����TRUE�����ʾ�Ѵ���������ٱ�������������񱻼�������
	virtual BOOL TaskProcess(const CTaskBase *pTask)
    {
        return FALSE;
    }

	virtual void TaskNotify(const CTaskBase *pTask, TEvent event)
    {

    }

    //  UI������Ϣ
    //  Msg:    ��Ϣ���� 
    //  wParam: ��������
    //  lParam: ��������
    virtual LRESULT TaskSendUIMsg(UINT Msg, WPARAM wParam,  LPARAM lParam)
    {
        if (m_hTaskWnd != NULL)
        {
            return ::SendMessage(m_hTaskWnd, Msg, wParam, lParam);
        }

        return FALSE;
    }

    virtual LRESULT TaskPostUIMsg(UINT Msg, WPARAM wParam,  LPARAM lParam)
    {
        if (m_hTaskWnd != NULL)
        {
            return ::PostMessage(m_hTaskWnd, Msg, wParam, lParam);
        }

        return FALSE;
    }

private:

	CTaskBase(const CTaskBase&);
	CTaskBase& operator=(const CTaskBase&);

protected:

	UUID			m_id;           //  �������ID

	LONG			m_type;         //  ��������(�ɸ���������ࡢʶ��)

	ULONG			m_priority;	    //  ���ȼ�(����ִ���Ⱥ�����Ȩ)

	BOOL			m_bUITask;	    //  �Ƿ���ҪUI�̴߳��������

    HWND            m_hTaskWnd;     //  ���񴰿ھ��

    BOOL            m_bUseMsg;      //  UI����ִ�з�ʽ(�ص�����Ϣ)

    BOOL            m_bMsgCall;     //  UI������÷�ʽ(Send, Post)

    UINT            m_UIMsg;        //  UI������Ϣ��ʶ

	volatile LONG	m_refCount;	    //  ���ü���
};


//
// ����ģ����������
//
template<class Type>
class CTaskAdapter : public CTaskBase
{
public:

    typedef BOOL (Type::*TaskProcessFunCallback)(const CTaskBase*);
    typedef void (Type::*TaskNotifyFunCallback)(const CTaskBase*, TEvent);
    typedef LRESULT (Type::*TaskUIMsgCallback)(UINT Msg, WPARAM wParam, LPARAM lParam);

public:

	CTaskAdapter(LONG type, Type *pInst, TaskProcessFunCallback taskProcCallback, 
        TaskNotifyFunCallback taskNotifyCallback = NULL): CTaskBase(type), m_pInst(pInst), 
        m_pfProcessFun(taskProcCallback), m_pfNotifyFun(taskNotifyCallback),m_pfSendUIMsgFun(NULL),m_pfPostUIMsgFun(NULL)
    {

    }


    CTaskAdapter(LONG type, Type *pInst, BOOL bUITaskFlag = TRUE, TaskUIMsgCallback taskUIMsgCallback = NULL): CTaskBase(type), m_pInst(pInst), 
        m_pfProcessFun(NULL), m_pfNotifyFun(NULL)
    {
        if (bUITaskFlag == TRUE)
        {
            m_pfSendUIMsgFun = taskUIMsgCallback;
            m_pfPostUIMsgFun = NULL;
        }
        else
        {
            m_pfPostUIMsgFun = taskUIMsgCallback;
            m_pfSendUIMsgFun = NULL;
        }

        if (m_pInst != NULL && m_pInst->GetSafeHwnd() != NULL)
        {
            SetUITaskHWnd(m_pInst->GetSafeHwnd() );
        }

        SetUITask(TRUE);
        SetUIMsgCall(bUITaskFlag);
    }


	virtual BOOL TaskProcess(const CTaskBase *pTask)
	{
		if(m_pInst != NULL && m_pfProcessFun != NULL)
        {
			return (m_pInst->*m_pfProcessFun)(pTask);
        }
		return TRUE;
	}

	virtual void TaskNotify(const CTaskBase *pTask, TEvent event)
	{
		if(m_pInst != NULL && m_pfNotifyFun != NULL)
        {
			(m_pInst->*m_pfNotifyFun)(pTask, event);
        }
	}

    virtual LRESULT TaskSendUIMsg(UINT Msg, WPARAM wParam,  LPARAM lParam)
    {
        if (GetUITaskFlag(Msg) == FALSE)
        {
            if (m_pInst != NULL && m_pfSendUIMsgFun != NULL)
            {
                return (m_pInst->*m_pfSendUIMsgFun)(Msg, wParam, lParam);
            }
            return -1L;
        }
        else
        {
            return CTaskBase::TaskSendUIMsg(Msg, wParam, lParam);
        }
    }

    virtual LRESULT TaskPostUIMsg(UINT Msg, WPARAM wParam,  LPARAM lParam)
    {
        if (GetUITaskFlag(Msg) == FALSE)
        {
            if (m_pInst != NULL && m_pfPostUIMsgFun != NULL)
            {
                return (m_pInst->*m_pfPostUIMsgFun)(Msg, wParam, lParam);
            }
            return -1L;
        }
        else
        {
            return CTaskBase::TaskPostUIMsg(Msg, wParam, lParam);
        }
    }

private:

    Type                        *m_pInst;               //  ʵ��ָ�����

    TaskProcessFunCallback      m_pfProcessFun;         //  ������ص�����

    TaskNotifyFunCallback       m_pfNotifyFun;          //  ����ͨ��ص�����

    TaskUIMsgCallback           m_pfSendUIMsgFun;       //  UI��Ϣ�ص�����

    TaskUIMsgCallback           m_pfPostUIMsgFun;       //  UI��Ϣ�ص�����

};


//
// ����ģ����������������(���ذ汾1)
// ʹ�÷�ʽ: IBase *pTask = MakeTask(type, this, &ClassName::MemberFunc);
//
template<class T>  
CTaskAdapter<T>* MakeTask(LONG type, T *pInst, 
							 BOOL (T::*pfProcess)(const CTaskBase*), 
							 void (T::*pfNotify)(const CTaskBase*, CTaskBase::TEvent) = NULL)
{
	return new CTaskAdapter<T>(type, pInst, pfProcess, pfNotify);
}


// ����ģ����������������(���ذ汾2)
template<class T>  
CTaskAdapter<T>* MakeTask(LONG type, T *pInst, BOOL bUITaskFlag = TRUE,
                             LRESULT(T::*pfTaskUIMsg)(UINT Msg, WPARAM wParam, LPARAM lParam) = NULL)
{
        return new CTaskAdapter<T>(type, pInst, bUITaskFlag, pfTaskUIMsg);
}


//////////////////////////////////////////////////////////////////////////
inline CTaskBase::CTaskBase(LONG type)
{
	memset(&m_id, 0, sizeof(m_id) );
	m_type          = type;
	m_priority      = TP_Normal;
	m_bUITask       = FALSE;
    m_hTaskWnd      = NULL;
    m_UIMsg         = WM_USER;
    m_bUseMsg       = FALSE;
    m_bMsgCall      = TRUE;
	m_refCount      = 0L;
}


inline void CTaskBase::SetId(UUID id) 
{ 
    m_id = id; 
}


inline UUID CTaskBase::GetId() const 
{ 
    return m_id;
}


inline void CTaskBase::SetId(RPC_WSTR StringUuid)
{
    UuidFromStringW(StringUuid, &m_id);
}


inline void CTaskBase::GetId(RPC_WSTR StringUuid) const
{
    UuidToStringW(&m_id, &StringUuid);
}


inline void CTaskBase::SetId(RPC_CSTR StringUuid)
{
    UuidFromStringA(StringUuid, &m_id);
}


inline void CTaskBase::GetId(RPC_CSTR StringUuid) const
{
    UuidToStringA(&m_id, &StringUuid);
}


inline void CTaskBase::SetType(LONG type) 
{ 
    m_type = type; 
}


inline LONG CTaskBase::GetType() const 
{ 
    return m_type;
}


inline void CTaskBase::SetPriority(ULONG priority) 
{ 
    m_priority = priority; 
}


inline ULONG CTaskBase::GetPriority() const 
{ 
    return m_priority; 
}


inline void CTaskBase::SetUITask(BOOL bUITask) 
{ 
    m_bUITask = bUITask; 
}


inline BOOL CTaskBase::IsUITask() const 
{ 
    return m_bUITask; 
}


inline void CTaskBase::SetUIMsgCall(BOOL bMsgCall) 
{ 
    m_bMsgCall = bMsgCall; 
}


inline BOOL CTaskBase::GetUIMsgCall() const 
{ 
    return m_bMsgCall; 
}



inline void CTaskBase::SetUITaskHWnd(HWND hTaskWnd)
{
    m_hTaskWnd = hTaskWnd;
}


inline HWND CTaskBase::GetUITaskHWnd() const
{
    return m_hTaskWnd;
}


inline void CTaskBase::SetUITaskFlag(UINT UIMsg, BOOL bUseMsg )
{
    m_UIMsg     = UIMsg;
    m_bUseMsg   = bUseMsg;
}


inline BOOL CTaskBase::GetUITaskFlag(UINT &UIMsg) const
{
    UIMsg = m_UIMsg;
    return m_bUseMsg;
}


inline LONG CTaskBase::AddRef() 
{ 
    return InterlockedIncrement(&m_refCount); 
}


inline LONG CTaskBase::Release() 
{ 
	if(InterlockedDecrement(&m_refCount) <= 0)
	{ 
		delete this;
        return 0L; 
	}
	else 
	{ 
		return m_refCount;
	}
}

#endif	// _TASK_BASE_H_