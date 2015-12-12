#include "stdafx.h"
#include "process_heap.h"


CProcessHeap::CProcessHeap( DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize)
{
    m_heapHandle = ::HeapCreate( flOptions, dwInitialSize, dwMaximumSize );
}


CProcessHeap::~CProcessHeap()
{
    if (IsValid() == TRUE)
    {
        ::HeapDestroy( m_heapHandle );
        m_heapHandle = NULL;
    }
}


BOOL CProcessHeap::IsValid()
{
    return (m_heapHandle != NULL);
}


LPVOID CProcessHeap::AllocHeap( DWORD dwFlags, SIZE_T dwBytes )
{
    if (IsValid() == FALSE)
    {
        return NULL;
    }

    return ::HeapAlloc( m_heapHandle, dwFlags, dwBytes );
}


LPVOID CProcessHeap::ReAllocHeap( DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes )
{
    if (IsValid() == FALSE)
    {
        return NULL;
    }

    return ::HeapReAlloc( m_heapHandle, dwFlags, lpMem, dwBytes );
}


SIZE_T CProcessHeap::GetAllocedSize( DWORD dwFlags, LPCVOID lpMem )
{
    if ( IsValid() == FALSE )
    {
        return (SIZE_T)(-1);
    }

    return ::HeapSize( m_heapHandle, dwFlags, lpMem );
}


BOOL CProcessHeap::FreeHeap( DWORD dwFlags, LPVOID lpMem )
{
    if ( IsValid() == FALSE )
    {
        return FALSE;
    }

    return ::HeapFree( m_heapHandle, dwFlags, lpMem );
}


SIZE_T CProcessHeap::CompactHeap( DWORD dwFlags )
{
    if ( IsValid() == FALSE )
    {
        return (SIZE_T)( -1 );
    }

    return ::HeapCompact( m_heapHandle, dwFlags );
}


BOOL CProcessHeap::ReSetHeap( DWORD flOptions, SIZE_T dwInitialSize, SIZE_T dwMaximumSize )
{
    if (IsValid() == TRUE)
    {
        HeapDestroy( m_heapHandle );
    }

    m_heapHandle = ::HeapCreate( flOptions, dwInitialSize, dwMaximumSize );

    return IsValid();
}