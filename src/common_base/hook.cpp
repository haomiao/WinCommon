#include "stdafx.h"
#include "hook.h"

CHook::CHook()
{

}


CHook::~CHook()
{
    ClearAllHook();
}


HHOOK CHook::AddHook( int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId )
{
    ReleaseHook( idHook );

    HHOOK hHook = ::SetWindowsHookEx( idHook, lpfn, hMod, dwThreadId );
    if (hHook != NULL)
    {
        m_hookMap.insert( std::make_pair( idHook, hHook ) );
    }
    return hHook;
}


BOOL CHook::ReleaseHook( int idHook )
{
    BOOL res = FALSE;
    HOOKMAP::iterator it = m_hookMap.find( idHook );
    if ( it != m_hookMap.end() )
    {
        res = ::UnhookWindowsHookEx( it->second );
        m_hookMap.erase( it );
    }

    return res;
}


void CHook::ClearAllHook()
{
    HOOKMAP::iterator it = m_hookMap.begin();
    HOOKMAP::iterator end = m_hookMap.end();
    for ( ; it != end; ++it )
    {
        ::UnhookWindowsHookEx( it->second );
    }
    m_hookMap.clear();
}