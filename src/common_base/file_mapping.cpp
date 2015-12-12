#include "stdafx.h"
#include "file_mapping.h"


static ULONGLONG GetCombinData(DWORD hightData, DWORD lowData)
{
    return ( ( (ULONGLONG)hightData << 32) | lowData);
}


CFileMapping::CFileMapping( LPCTSTR lpName,
    DWORD dwMaximumSizeLow,
    HANDLE hFile,
    LPSECURITY_ATTRIBUTES lpAttributes,
    DWORD flProtect,
    DWORD dwMaximumSizeHigh)
{
    m_mapFileHandle = ::CreateFileMapping( hFile, lpAttributes, flProtect, dwMaximumSizeHigh, dwMaximumSizeLow, lpName );

    m_mapFileSize = GetCombinData(dwMaximumSizeHigh, dwMaximumSizeLow);
}


CFileMapping::~CFileMapping()
{
    if (IsValid() == TRUE)
    {
        ::CloseHandle(m_mapFileHandle);
        m_mapFileHandle = NULL;
    }
}


BOOL CFileMapping::IsValid()
{
    return (m_mapFileHandle != NULL);
}


BOOL CFileMapping::OpenFileMapping(LPCTSTR lpName, DWORD dwDesiredAccess, BOOL bInheritHandle)
{
    if (IsValid() == TRUE)
    {
        ::CloseHandle( m_mapFileHandle );
    }

    m_mapFileHandle = ::OpenFileMapping( dwDesiredAccess, bInheritHandle, lpName );

    return IsValid();
}


LPVOID CFileMapping::MapViewOfFile( DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap,
    DWORD dwFileOffsetHigh, DWORD dwDesiredAccess )
{
    if ( IsValid() == FALSE || GetCombinData( dwFileOffsetHigh, dwFileOffsetLow ) + dwNumberOfBytesToMap > m_mapFileSize )
    {
        return NULL;
    }

    return ::MapViewOfFile( m_mapFileHandle, dwDesiredAccess, dwFileOffsetHigh, dwFileOffsetLow,
        dwNumberOfBytesToMap );
}


BOOL CFileMapping::UnMapViewOfFile( LPCVOID lpBaseAddress )
{
    return ::UnmapViewOfFile( lpBaseAddress );
}


CRWSharedMemory::CRWSharedMemory( LPCTSTR lpName, DWORD dwMaximumSizeLow, DWORD dwMaximumSizeHigh ) :
m_fileMapping( lpName, dwMaximumSizeLow, INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, dwMaximumSizeHigh )
{

}


CRWSharedMemory::~CRWSharedMemory()
{

}


BOOL CRWSharedMemory::ReadMemory( LPVOID pBuffer, SIZE_T bufferBytes, DWORD OffsetLow, SIZE_T bytes, DWORD OffsetHigh )
{
    if ( m_fileMapping.GetMapFileSize() < GetCombinData(OffsetHigh, OffsetLow) + bytes ||
        bufferBytes < bytes)
    {
        return FALSE;
    }

    LPVOID pMapBuffer = m_fileMapping.MapViewOfFile( OffsetLow, bytes, OffsetHigh );
    if (pMapBuffer != NULL)
    {
        memcpy_s( pBuffer, bufferBytes, pMapBuffer, bytes );
        m_fileMapping.UnMapViewOfFile( pMapBuffer );
        return TRUE;
    }

    return FALSE;
}


BOOL CRWSharedMemory::WriteMemory(LPCVOID pBuffer, SIZE_T bufferbytes, DWORD OffsetLow,  DWORD OffsetHigh )
{
    if ( m_fileMapping.GetMapFileSize() < GetCombinData( OffsetHigh, OffsetLow ) + bufferbytes )
    {
        return FALSE;
    }

    LPVOID pMapBuffer = m_fileMapping.MapViewOfFile( OffsetLow, bufferbytes, OffsetHigh );
    if ( pMapBuffer != NULL )
    {
        memcpy_s( pMapBuffer, m_fileMapping.GetMapFileSize() - GetCombinData( OffsetHigh, OffsetLow ), pBuffer, bufferbytes );
        m_fileMapping.UnMapViewOfFile( pMapBuffer );
        return TRUE;
    }

    return FALSE;
}