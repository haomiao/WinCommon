///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   timer_helper.h 
/// 	@brief	计时器、时间序列任务工具. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/11/29  16:13:12
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _TIMER_HELPER_H_
#define     _TIMER_HELPER_H_

#include <Windows.h>
#include <functional>
#include <deque>
#include <mmsystem.h>
#include "lock_critical_section.h"
#include "auto_lock.h"

#pragma comment(lib, "winmm.lib")


class timer_helper
{
public:

    timer_helper( const LONGLONG llTimeOut )
        :m_llTimeOut( llTimeOut )
        , m_llTimeStart(0)
        , m_llTimeEnd(0)
    {
        LARGE_INTEGER lager_int;
        if ( QueryPerformanceFrequency( &lager_int ) )
        {
            m_llCountPreSecond = lager_int.QuadPart;
        }
        else
        {
            m_llCountPreSecond = 0;
        }
    }

    ~timer_helper(){}

    void set_to_now(){ m_llTimeStart = GetCurTime(); }

    BOOL is_timeout()
    {
        m_llTimeEnd = GetCurTime();
        if ( m_llTimeEnd - m_llTimeStart < m_llTimeOut )
        {
            return FALSE;
        }

        m_llTimeStart = m_llTimeEnd;
        return TRUE;
    }

    LONGLONG GetCurTime()
    {
        LONGLONG llCount;
        LARGE_INTEGER ll_Int;
        if ( ( 0 != m_llCountPreSecond ) && QueryPerformanceCounter( &ll_Int ) )
        {
            llCount = ll_Int.QuadPart;
        }
        else
        {
            llCount = (LONGLONG)timeGetTime();
        }
        return llCount;
    }

protected:

    timer_helper(const timer_helper &);
    void operator=(const timer_helper&);

    LONGLONG m_llCountPreSecond;
    LONGLONG m_llTimeStart;
    LONGLONG m_llTimeEnd;
    LONGLONG m_llTimeOut;
};



template<typename _TyRet>
class task_timer
{
public:

    typedef std::tr1::function<_TyRet(void)> timerproc_t;
    typedef std::deque<timerproc_t> taskqueue_t;

public:

    task_timer(const U32 nPeriod)
        :m_hQueue(NULL)
        ,m_hTimer(NULL)
        ,m_timer(nPeriod)
    {
        m_hQueue = ::CreateTimerQueue();
        ::CreateTimerQueueTimer(&m_hTimer, m_hQueue, time_proc, this, 
            nPeriod, nPeriod, WT_EXECUTEINTIMERTHREAD);
    }

    virtual ~task_timer()
    {
        ::DeleteTimerQueueTimer(m_hQueue, m_hTimer, INVALID_HANDLE_VALUE);
        ::DeleteTimerQueue(m_hQueue);
    }

    void add_task(timerproc_t task)
    {
        if (!task)
        {
            return;
        }

        AutoLock lock(&m_crtQueue );
        m_queue.push_back(task);
    }

    void clear_task()
    {
        AutoLock lock( &m_crtQueue );
        m_queue.clear();
    }


protected:

    task_timer(const task_timer &);
    void operator=(const task_timer&);

    static void CALLBACK time_proc(void *param, BOOLEAN)
    {
        static_cast<task_timer *>(param)->exec_task();
    }

    void exec_task()
    {
        AutoLock lock( &m_crtQueue );
        if (m_queue.size() && m_timer.is_timeout())
        {
            (*m_queue.begin())();
            m_queue.pop_front();
        }
    }


protected:

    HANDLE                      m_hQueue;
    HANDLE                      m_hTimer;

    taskqueue_t                 m_queue;
    timer_helper                m_timer;
    CCriticalSectionLock        m_crtQueue;
};


//////////////////////////////////////////////////////////////////////////
//  特化版本，以支持某个任务按照指定时间间隔执行
//  任务函数：返回值类型T1，参数类型T2
template<typename T1, typename T2>
struct AutoTask
{
};

template<typename T1, typename T2>
class task_timer <AutoTask<T1, T2> >
{
public:

    typedef std::tr1::function<T1( T2 )> timerproc_t;
    typedef std::deque<timerproc_t> taskqueue_t;

public:

    task_timer( const U32 nPeriod )
        :m_hQueue( NULL )
        , m_hTimer( NULL )
        , m_timer( nPeriod )
    {
        m_hQueue = ::CreateTimerQueue();
        ::CreateTimerQueueTimer( &m_hTimer, m_hQueue, time_proc, this,
            nPeriod, nPeriod, WT_EXECUTEINTIMERTHREAD );
    }

    virtual ~task_timer()
    {
        ::DeleteTimerQueueTimer( m_hQueue, m_hTimer, INVALID_HANDLE_VALUE );
        ::DeleteTimerQueue( m_hQueue );
    }

    void add_task( timerproc_t task )
    {
        if ( !task )
        {
            return;
        }

        AutoLock lock( &m_crtQueue );
        if ( m_queue.size() )
        {
            m_queue.clear();
        }
        m_queue.push_back( task );
    }

    void clear_task()
    {
        AutoLock lock( &m_crtQueue );
        m_queue.clear();
    }


protected:

    task_timer( const task_timer & );
    void operator=( const task_timer& );

    static void CALLBACK time_proc( void *param, BOOLEAN )
    {
        static_cast<task_timer *>( param )->exec_task();
    }

    void exec_task()
    {
        AutoLock lock( &m_crtQueue );
        if ( m_queue.size() && m_timer.is_timeout() )
        {
            ( *m_queue.begin() )( );
        }
    }


protected:

    HANDLE                      m_hQueue;
    HANDLE                      m_hTimer;

    taskqueue_t                 m_queue;
    timer_helper                m_timer;
    CCriticalSectionLock        m_crtQueue;
};


#endif  //  _TIMER_HELPER_H_
