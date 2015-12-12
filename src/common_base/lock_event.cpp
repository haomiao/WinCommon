#include "stdafx.h"
#include "lock_event.h"


CEventLock::CEventLock( BOOL bCreateFlag, LPSECURITY_ATTRIBUTES lpEventAttributes, BOOL bManualReset, BOOL bInitialState, LPCTSTR lpName ) :m_eventHandle( NULL )
{
    if ( bCreateFlag == TRUE)
    {
        m_eventHandle = ::CreateEvent( lpEventAttributes, bManualReset, bInitialState, lpName );
    }
}


CEventLock::~CEventLock()
{
    if ( m_eventHandle != NULL )
    {
        ::CloseHandle( m_eventHandle );
    }
}


BOOL CEventLock::OpenEvent( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName )
{
    HANDLE tempHandle = ::OpenEvent(dwDesiredAccess, bInheritHandle, lpName);
    if ( tempHandle != NULL)
    {
        if ( IsLockValid() == TRUE )
        {
            ::CloseHandle( m_eventHandle );
        }
        m_eventHandle = tempHandle;
        return TRUE;
    }
    return FALSE;
}


void CEventLock::Lock()
{
    ::ResetEvent( m_eventHandle );
}


void CEventLock::UnLock()
{
    ::SetEvent( m_eventHandle );
}


BOOL CEventLock::IsLockValid()
{
    return ( m_eventHandle != NULL );
}