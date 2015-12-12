#ifndef     _TASK_BASE_H_
#define     _TASK_BASE_H_

#include <Rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

class CTaskManager;

//
// 任务基类(可继承实现其他任务类),提供必要的接口实现
//
class CTaskBase
{
public:

	//
	//  优先级
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
	//  事件
	//
	enum TEvent
	{
		TE_Unknown = 0,     //  未知任务
		TE_Canceled,        //  已取消的任务
		TE_Removed,         //  已移除的任务
		TE_Completed,       //  已完成的任务
        TE_EventNum,        //  任务事件类型计数
	};

public:

	CTaskBase(LONG type);

	virtual ~CTaskBase() {}

public:

	//  设置任务识别ID
	void SetId(UUID id);
	UUID GetId() const;
    void SetId(RPC_WSTR StringUuid);
    void GetId(RPC_WSTR StringUuid) const;
    void SetId(RPC_CSTR StringUuid);
    void GetId(RPC_CSTR StringUuid) const;

	//  设置类型必须大于0
	void SetType(LONG type);
	LONG GetType() const;

    //  设置优先权
	void SetPriority(ULONG priority);
	ULONG GetPriority() const;

    //  设置UI任务标识
    //  bUITask:True则为UI，否则为一般任务
	void SetUITask(BOOL bUITask);
	BOOL IsUITask() const;

    //  设置UI任务窗口句柄
    void SetUITaskHWnd(HWND hTaskWnd);
    HWND GetUITaskHWnd() const;
    
    //  设置UI任务处理方式
    //  UIMsg：任务消息
    //  bUITaskFlag：TRUE，表示使用消息, False:表示使用回调 
    void SetUITaskFlag(UINT UIMsg, BOOL bUseMsg = FALSE);
    BOOL GetUITaskFlag(UINT &UIMsg) const;

    //  设置UI任务调用方式
    //  bMsgCall:TRUE,则为Send，False则为Post
    void CTaskBase::SetUIMsgCall(BOOL bMsgCall);
    BOOL CTaskBase::GetUIMsgCall() const;

	//  引用计数
	LONG AddRef();
	LONG Release();

	// 任务操作、事件通告
	// 返回TRUE，则表示已处理后续不再被处理，否则该任务被继续传递
	virtual BOOL TaskProcess(const CTaskBase *pTask)
    {
        return FALSE;
    }

	virtual void TaskNotify(const CTaskBase *pTask, TEvent event)
    {

    }

    //  UI界面消息
    //  Msg:    消息类型 
    //  wParam: 附加数据
    //  lParam: 附加数据
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

	UUID			m_id;           //  任务对象ID

	LONG			m_type;         //  任务类型(可根据任务分类、识别)

	ULONG			m_priority;	    //  优先级(决定执行先后、优先权)

	BOOL			m_bUITask;	    //  是否需要UI线程处理的任务

    HWND            m_hTaskWnd;     //  任务窗口句柄

    BOOL            m_bUseMsg;      //  UI任务执行方式(回调、消息)

    BOOL            m_bMsgCall;     //  UI任务调用方式(Send, Post)

    UINT            m_UIMsg;        //  UI任务消息标识

	volatile LONG	m_refCount;	    //  引用计数
};


//
// 任务模板类适配器
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

    Type                        *m_pInst;               //  实例指针对象

    TaskProcessFunCallback      m_pfProcessFun;         //  任务处理回调函数

    TaskNotifyFunCallback       m_pfNotifyFun;          //  任务通告回调函数

    TaskUIMsgCallback           m_pfSendUIMsgFun;       //  UI消息回调函数

    TaskUIMsgCallback           m_pfPostUIMsgFun;       //  UI消息回调函数

};


//
// 任务模板类适配器构造器(重载版本1)
// 使用方式: IBase *pTask = MakeTask(type, this, &ClassName::MemberFunc);
//
template<class T>  
CTaskAdapter<T>* MakeTask(LONG type, T *pInst, 
							 BOOL (T::*pfProcess)(const CTaskBase*), 
							 void (T::*pfNotify)(const CTaskBase*, CTaskBase::TEvent) = NULL)
{
	return new CTaskAdapter<T>(type, pInst, pfProcess, pfNotify);
}


// 任务模板类适配器构造器(重载版本2)
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