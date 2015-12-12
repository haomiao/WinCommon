#include "stdafx.h"
#include "lock_mutex.h"


CMutexLock::CMutexLock( BOOL bCreateFlag, LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName ) :m_mutexHandle(NULL), m_dwMilliseconds( INFINITE )
{
    if ( bCreateFlag == TRUE)
    {
        m_mutexHandle = ::CreateMutex( lpMutexAttributes, bInitialOwner, lpName );
    }
}


CMutexLock::~CMutexLock()
{
    if (m_mutexHandle != NULL)
    {
        ::CloseHandle( m_mutexHandle );
    }
}


BOOL CMutexLock::OpenMutex( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName )
{
    HANDLE tempHandle = ::OpenMutex(dwDesiredAccess, bInheritHandle, lpName);
    if ( tempHandle != NULL)
    {
        if ( IsLockValid() == TRUE )
        {
            ::CloseHandle( m_mutexHandle );
        }
        m_mutexHandle = tempHandle;
        return TRUE;
    }
    return FALSE;
}


void CMutexLock::Lock()
{
    ::WaitForSingleObject( m_mutexHandle, m_dwMilliseconds );
}


void CMutexLock::UnLock()
{
    ::ReleaseMutex( m_mutexHandle );
}


void CMutexLock::SetLockLimited( DWORD dwMilliseconds )
{
    m_dwMilliseconds = dwMilliseconds;
}


BOOL CMutexLock::IsLockValid()
{
    return (m_mutexHandle != NULL);
}