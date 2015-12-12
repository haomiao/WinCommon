#include "stdafx.h"
#include "lock_semaphore .h"


CSemaphoreLock::CSemaphoreLock( BOOL bCreateFlag, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, LONG lMaximumCount, LPCTSTR lpName ) :m_semaphoreHandle( NULL ), m_releaseCount( 1 )
{
    if ( bCreateFlag == TRUE)
    {
        m_semaphoreHandle = ::CreateSemaphore( lpSemaphoreAttributes, lInitialCount, lMaximumCount, lpName );
    }
}


CSemaphoreLock::~CSemaphoreLock()
{
    if ( m_semaphoreHandle != NULL )
    {
        ::CloseHandle( m_semaphoreHandle );
    }
}


BOOL CSemaphoreLock::OpenEvent( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName )
{
    HANDLE tempHandle = ::OpenSemaphore(dwDesiredAccess, bInheritHandle, lpName);
    if ( tempHandle != NULL)
    {
        if ( IsLockValid() == TRUE )
        {
            ::CloseHandle( m_semaphoreHandle );
        }
        m_semaphoreHandle = tempHandle;
        return TRUE;
    }
    return FALSE;
}


void CSemaphoreLock::Lock()
{
    ::WaitForSingleObject( m_semaphoreHandle, INFINITE );
}


void CSemaphoreLock::UnLock()
{
    ::ReleaseSemaphore( m_semaphoreHandle, m_releaseCount, NULL );
}


BOOL CSemaphoreLock::IsLockValid()
{
    return ( m_semaphoreHandle != NULL );
}


void CSemaphoreLock::SetReleaseCount( LONG releaseCount )
{
    m_releaseCount = releaseCount;
}