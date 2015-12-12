#include "stdafx.h"
#include "lock_critical_section.h"


CCriticalSectionLock::CCriticalSectionLock()
{
    ::InitializeCriticalSection(&m_criticalSection);
}


CCriticalSectionLock::CCriticalSectionLock( DWORD spinCount )
{
    ::InitializeCriticalSectionAndSpinCount( &m_criticalSection, spinCount );
}


CCriticalSectionLock::~CCriticalSectionLock()
{
    ::DeleteCriticalSection(&m_criticalSection);
}


void CCriticalSectionLock::Lock()
{
    EnterCriticalSection(&m_criticalSection);
}


void CCriticalSectionLock::UnLock()
{
    LeaveCriticalSection(&m_criticalSection);
}


BOOL CCriticalSectionLock::TryLock()
{
    return ::TryEnterCriticalSection( &m_criticalSection );
}


DWORD CCriticalSectionLock::SetSpinCount( DWORD dwSpinCount )
{
    return ::SetCriticalSectionSpinCount( &m_criticalSection, dwSpinCount );
}