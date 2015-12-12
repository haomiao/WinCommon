#include "stdafx.h"
#include <assert.h>
#include <time.h>
#include <climits>
#include <exception>
#include <iostream>
#include <process.h>
#include "thread.h"
#include "auto_lock.h"
#include "string_helper.h"


CThreadControl::CThreadControl( HANDLE threadHandle ) :
m_threadHandle( threadHandle ),
m_threadID(GetCurrentThreadId() )
{

}


CThreadControl::CThreadControl( HANDLE threadHandle, DWORD threadID ) :
m_threadHandle( threadHandle ),
m_threadID( threadID )
{

}


CThreadControl::~CThreadControl()
{

}


//  只需要比较线程ID即可
bool CThreadControl::operator==( const CThreadControl& rhs ) const
{
    return m_threadID == rhs.m_threadID;
}


bool CThreadControl::operator!=( const CThreadControl& rhs ) const
{
    return !(operator == (rhs) );
}


void CThreadControl::Join( DWORD watiTime )
{
    if ( m_threadHandle == 0 )
    {
        assert(0);
        return;
    }

    DWORD rc = WaitForSingleObject( m_threadHandle, watiTime );
    if (rc == WAIT_OBJECT_0 )
    {
        Detach();
        return;
    }

    //  有风险，可能导致资源无法释放！！！建议使用INFINITE
    TerminateThread( m_threadHandle, 0 );
    ::Sleep( 200 );
    Detach();
}


void CThreadControl::Detach()
{
    if ( m_threadHandle == 0 )
    {
        assert(0);
        return;
    }
    
    if ( CloseHandle( m_threadHandle ) == 0 )
    {
        assert(0);
        return;
    }
}


DWORD CThreadControl::GetThreadID() const
{
    return m_threadID;
}


void CThreadControl::SleepThread( size_t timeout )
{
    ::Sleep(timeout);
}


void CThreadControl::YieldThread()
{
    //
    // A value of zero causes the thread to relinquish the remainder
    // of its time slice to any other thread of equal priority that is
    // ready to run.
    //
    ::Sleep(0);
}


CThread::CThread() :
    m_started(false),
    m_running(false),
    m_threadHandle(0),
    m_threadID(0),
    m_threadName("NoneName")
{

}


CThread::CThread( const std::string& name ) :
    m_threadName(name),
    m_started( false ),
    m_running(false),
    m_threadHandle( 0 ),
    m_threadID( 0 )
{

}


CThread::~CThread()
{

}


//  执行线程，线程执行完成后将自动释放自身对象
unsigned int CThread::ThreadProc( void* arg )
{
    // Ensure that the thread doesn't go away until run() has
    // completed.
    //
    ThreadPtr thread;

    try
    {
        CThread* rawThread = static_cast<CThread*>( arg );

        //
        // Ensure that the thread doesn't go away until run() has
        // completed.
        //
        thread = rawThread;

#if (defined _WIN32_WCE || defined WIN32)
        //
        // Initialize the random number generator in each thread on
        // Windows (the rand() seed is thread specific).
        //
        srand(GetTickCount() ^ thread->GetThreadControl().GetThreadID() );
#endif

        //
        // See the comment in Thread::Start() for details.
        //
        rawThread->DecRef();
        thread->Run();
    }
    catch(...)
    {
        if ( !thread->GetThreadNameA().empty() )
        {
			printf(" thread terminal ");
            std::cerr << thread->GetThreadNameA() << " terminating" << std::endl;
        }
#ifdef _DEBUG
#if defined(_MSC_VER) && (_MSC_VER < 1300)
        terminate();
#else
	std::terminate();
#endif
#endif
    }

    thread->Done();
   
    return 0;
}


CThreadControl CThread::Start( size_t stackSize )
{
    return Start(stackSize, THREAD_PRIORITY_NORMAL);
}


CThreadControl CThread::Start( size_t stackSize, int priority )
{
    //
    // Keep this alive for the duration of start
    //
    ThreadPtr keepMe = this;

    AutoLock lock(&m_stateLock);

    if(m_started == true)
    {
        assert(0);
        return CThreadControl( m_threadHandle, m_threadID );
    }

    AddRef();

#if !(defined _WIN32_WCE)
    unsigned int id;
    m_threadHandle = 
        reinterpret_cast<HANDLE>(
            _beginthreadex(0, 
                            static_cast<unsigned int>(stackSize), 
                            ThreadProc, this, CREATE_SUSPENDED, &id ) );
#else
	DWORD id;
    _handle = CreateThread(NULL, static_cast<unsigned int>(stackSize), ThreadProc, this, CREATE_SUSPENDED, &id);
#endif
    m_threadID = id;

    if(m_threadHandle == 0)
    {
        DecRef();
        throw std::exception("create thread fail");
    }
    if ( SetThreadPriority( m_threadHandle, priority ) == 0 )
    {
        DecRef();
        throw std::exception( "SetThreadPriority fail" );
	}    
	m_started = true;
	m_running = true;
    if(ResumeThread(m_threadHandle) == -1)
    {
        DecRef();
        throw std::exception( "ResumeThread fail" );
    }

    return CThreadControl( m_threadHandle, m_threadID );
}


CThreadControl CThread::GetThreadControl()
{
    AutoLock lock(&m_stateLock);
    if(!m_started)
    {
        throw std::exception( "thread have not started" );
    }
    return CThreadControl( m_threadHandle, m_threadID );
}


bool CThread::operator==( const CThread& rhs ) const
{
    return this == &rhs;
}


bool CThread::operator!=( const CThread& rhs ) const
{
    return this != &rhs;
}


bool CThread::operator<( const CThread& rhs ) const
{
    return this < &rhs;
}


bool CThread::IsAlive()
{
    AutoLock lock(&m_stateLock);
    return m_running;
}


void CThread::Done()
{
    AutoLock lock( &m_stateLock );
    m_running = false;
	m_started = false;
}


std::string CThread::GetThreadNameA() const
{
    return m_threadName;
}


std::wstring CThread::GetThreadNameW() const
{
    return MBCS2Unicode(m_threadName.c_str() );
}
