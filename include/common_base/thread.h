///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   thread.h
/// 	@brief	����IEC��ȡʵ��windows���߳���ز���. 
/// 
///		�ṩ��Ҫ���̲߳�����  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/07  16:24:48
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _THREAD_H_
#define     _THREAD_H_

#include "config.h"
#include "ref_counter.h"
#include "lock_critical_section.h"
#include "smart_ptr.h"


//  �߳̿�����,��ȡ�����̵߳��߳̾�����߳�ID
//  �ⲿ��ʹ�ø�����̵߳Ŀ���
class CThreadControl
{
public:

    //
    // Constructs a ThreadControl representing the current thread.
    // join and detach cannot be called on such ThreadControl object.
    //
    CThreadControl( HANDLE handle = NULL );
    CThreadControl( HANDLE threadHandle, DWORD threadID );

    ~CThreadControl();

    //
    // Default copy destructor, assignment operator and destructor OK
    //

    //
    // == and != are meaningful only before the thread is joined/detached,
    // or while the thread is still running.
    //
    bool operator==( const CThreadControl& ) const;
    bool operator!=( const CThreadControl& ) const;

    //
    // Wait until the controlled thread terminates. The call has POSIX
    // semantics.
    //
    // At most one thread can wait for the termination of a given
    // thread. Calling join on a thread on which another thread is
    // already waiting for termination results in undefined behaviour,
    // as does joining with a thread after having joined with it
    // previously, or joining with a detached thread.
    //
    void Join( DWORD watiTime = INFINITE );

    //
    // Detach a thread. Once a thread is detached, it cannot be
    // detached again, nor can it be joined with. Every thread that
    // was created using the IceUtil::Thread class must either be
    // joined with or detached exactly once. Detaching a thread a
    // second time, or detaching a thread that was previously joined
    // with results in undefined behavior.
    //
    void Detach();

    //
    // GetThreadID() returns the Thread ID on Windows 
    //
    DWORD GetThreadID() const;

    /*
    *   sleep milisecond
    */
    static void SleepThread(size_t );
    static void YieldThread();

private:

    HANDLE      m_threadHandle;     //  �߳̾��
    DWORD       m_threadID;         //  �߳�ID
};



// Thread object must born by new and it will delete itself when thread quit
// So if you want to control it must keep referance with it
class CThread : public CRefCounterShared
{
public:

    CThread();

    CThread( const std::string& );

    virtual ~CThread();

    //  ����ʵ�ֵľ����ִ������
    virtual void Run() = 0;

    //  �����̡߳�ִ�в���
    //  stackSize:�̶߳�ջ��С
    //  priority: �߳����ȼ�
    CThreadControl Start( size_t stackSize = 0 );
    CThreadControl Start( size_t stackSize, int priority );

    //  ��ȡ�߳̿��ƶ���
    CThreadControl GetThreadControl();

    bool operator==( const CThread& ) const;
    bool operator!=( const CThread& ) const;
    bool operator<( const CThread& ) const;

    //
    // Check whether a thread is still alive.
    //
    bool IsAlive();

    //
    // Get the thread name
    //
    std::string GetThreadNameA() const;
    std::wstring GetThreadNameW() const;

protected:

    //  ��ɺ󣬺�����������
    void Done();

protected:

    const std::string       m_threadName;
    CCriticalSectionLock    m_stateLock;
    bool                    m_started;
    bool                    m_running;
    HANDLE                  m_threadHandle;
    DWORD                   m_threadID;

private:

    static unsigned int WINAPI ThreadProc( void* arg );

    DISALLOW_COPY_AND_ASSIGN( CThread );
};


//  ʹ�÷�ʽ��   class MyThread:public CThread{....};
//              ThreadPtr threadPtr = new MyThread( "ThreadOne" );
//              CThreadControl threadCtr = threadPtr->Start( 0 );
typedef CSmartPtr<CThread> ThreadPtr;

#endif  //  _THREAD_H_

