///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   task_manager.h  
/// 	@brief	主要封装任务管理器
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
// 任务管理器
//
//  模式：bManual为False,则只要任务队列有任务，即可执行任务队列中的任务，后期加入的任务也会得到执行
//        当为True，则只有执行了SetTaskEvent后才会执行任务队列中的任务，直到任务队列为空；后期加入的任务
//        若之前队列已经执行完成，则需要重新执行SetTaskEvent才能继续执行任务队列中的任务
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
	//  以指定的线程优先级启动任务管理器
    //  欲执行任务必须调用的接口
	//
	BOOL Startup(int priority = THREAD_PRIORITY_NORMAL, BOOL bManual = FALSE);

    //  停止任务管理器的执行
    //  ShutdownForce:强制停止(可能会出现不安全的行为)
    //  ShutdownSafe:安全停止
    //  millisec:等待超时时间(ms为单位)
	BOOL ShutdownForce(DWORD millisec = INFINITE);
    BOOL ShutdownSafe();

    //  任务管理器是否开启
    BOOL IsStarted() const;

    //  是否已退出任务循环
    BOOL IsExited() const;

	//  开始执行任务事件(一般只用在当bManual == TRUE(手动任务控制)时)
	BOOL SetTaskEvent();

    //  清空任务队列
	void ClearTaskQueue();

    //  清空某类型任务队列
	void ClearKindTask(ULONG type);

    //  任务队列是否为空
	BOOL IsTaskQueueEmpty();

    //  获取当前任务队列数量
	ULONG GetCurTaskCount();

	// 返回任务队列，直接操作(慎用)
	const TaskQueue& GetTaskQueue();

	// 将当前任务按优先级顺序插入到任务队列中
	UUID AddTask(CTaskBase *pTask);

private:

    //  初始化操作
	void Init();

    //  清理操作
    void Clear();

    //  插入任务至任务队列
    void InsertTaskQueue(CTaskBase *pTask);

    //  获取当前任务对象
	CTaskBase* GetPopTask();

    //  将被清空的任务队列，得到最后一次被执行的机会
	void ClearedTaskQueue(TaskQueue &taskQueue);

    //  执行任务循环
	unsigned int Run();

    //  任务线程，用于执行队列中各个任务
	static unsigned __stdcall TaskThread(LPVOID lpVoid);

private:

    BOOL                    m_bManual;      //  手动执行标识

	volatile BOOL		    m_exited;       //  退出标识

	LONG				    m_taskId;		//  任务递增Id

	HANDLE				    m_hThread;		//  任务线程句柄

	HANDLE				    m_taskEvent;	//  任务启动通知事件

	TaskQueue			    m_taskQueue;	//  任务队列

	CCriticalSectionLock    m_taskLock;		//  任务队列锁

};

#endif	// _TASK_MANAGER_H_