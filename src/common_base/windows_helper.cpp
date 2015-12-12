#include "stdafx.h"
#include <Tlhelp32.h>
#include "windows_helper.h"


struct EnumWndStruct
{
    LPTSTR  m_lpClassName;      //  窗口类名
    LPCTSTR m_lpWindowName;     //  窗口标题
    BOOL    m_bFind;            //  是否找到
    HWND    m_hWnd;             //  窗口句柄
};

struct EnumWindow
{
    HWND    m_hWnd;             //  窗口句柄
    DWORD   m_processID;        //  进程ID
};


TString GetSystemErrorDesc( DWORD dwErrorCode )
{
    TString msg;
    LPTSTR lpMsgBuf;

    if ( ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        nullptr,
        dwErrorCode,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
        (LPTSTR)&lpMsgBuf,
        0,
        nullptr
        ) != 0)
    {
        msg = lpMsgBuf;
        ::LocalFree( lpMsgBuf );
    }

    return msg;
}


//  通过创建进程快照并遍历查找进程对应ID
BOOL IsProcessExist( DWORD processID )
{  
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if ( hSnapshot == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    } 
    
    PROCESSENTRY32 pe; 
    pe.dwSize = sizeof(PROCESSENTRY32);  
    if ( FALSE == ::Process32First( hSnapshot, &pe ) )
    {
        ::CloseHandle( hSnapshot );
        return FALSE;
    }
    
    do
    {    
        if ( pe.th32ProcessID == processID )
        {
            ::CloseHandle(hSnapshot);
            return TRUE;
        }

        pe.dwSize = sizeof(PROCESSENTRY32);  
    }
    while(Process32Next(hSnapshot, &pe) != FALSE);

    ::CloseHandle(hSnapshot);
    return FALSE;  
}


BOOL CALLBACK EnumWindowsProc2( HWND hWnd, LPARAM lParam )
{
    EnumWndStruct *pData = reinterpret_cast<EnumWndStruct *>( lParam );
    if (pData == NULL)
    {
        return FALSE;
    }

    //  获取顶层窗口
    HWND hPWnd = hWnd;
    while ( GetParent( hPWnd ) != NULL )
    {
        hPWnd = GetParent( hPWnd );
    }

    if (hPWnd != NULL)
    {
        TCHAR tempBuffer[MAX_PATH] = { 0 };
        BOOL res = FALSE;
        if (pData->m_lpClassName != NULL)
        {
            if ( ::GetClassName( hPWnd, tempBuffer, MAX_PATH ) != 0 )
            {
                if ( _tcscmp( tempBuffer, pData->m_lpClassName ) == 0 )
                {
                    res = TRUE;
                }
                else
                {
                    res = FALSE;
                }
            }
        }

        if (pData->m_lpWindowName != NULL)
        {
            if ( ::GetWindowText( hPWnd, tempBuffer, MAX_PATH ) )
            {
                if ( _tcscmp( tempBuffer, pData->m_lpWindowName ) == 0 )
                {
                    res = TRUE;
                }
                else
                {
                    res = FALSE;
                }
            }
        }
        
        if (res == TRUE)
        {
            pData->m_bFind = TRUE;
            pData->m_hWnd = hPWnd;
            return FALSE;
        }
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


BOOL IsProcessExist( LPCTSTR lpWindowName, LPTSTR lpClassName )
{
    if (lpClassName == NULL && lpWindowName == NULL)
    {
        return FALSE;
    }

    EnumWndStruct data = { lpClassName, lpWindowName, FALSE, NULL };
    EnumWindows( EnumWindowsProc2, reinterpret_cast<LPARAM>( &data ) );
    return data.m_bFind;
}


BOOL IsProcessExist2( LPCTSTR processName )
{
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( hSnapshot == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof( PROCESSENTRY32 );
    if ( FALSE == ::Process32First( hSnapshot, &pe ) )
    {
        ::CloseHandle( hSnapshot );
        return FALSE;
    }

    do
    {
        if ( _tcscmp( pe.szExeFile, processName ) == 0)
        {
            ::CloseHandle( hSnapshot );
            return TRUE;
        }

        pe.dwSize = sizeof( PROCESSENTRY32 );
    } while ( Process32Next( hSnapshot, &pe ) != FALSE );

    ::CloseHandle( hSnapshot );
    return FALSE;
}


BOOL CALLBACK EnumWindowsProc( HWND hWnd, LPARAM lParam )
{
    EnumWindow* data = reinterpret_cast<EnumWindow*>(lParam);
    if ( !data )
    {
        return FALSE;
    }

    DWORD processID = 0;
    GetWindowThreadProcessId( hWnd, &processID );
    if ( processID == data->m_processID )
    {
        data->m_hWnd = hWnd;
        return FALSE;
    }

    return TRUE;
}


//  通过枚举所有顶层窗口，在回调函数中查找对应窗口句柄下对应进程ID
HWND GetProcessTopWindowWnd( DWORD processID )
{
    if ( IsProcessExist(processID) == FALSE)
    {
        return NULL;
    }

    EnumWindow data = { NULL, processID };
    EnumWindows( EnumWindowsProc, reinterpret_cast<LPARAM>( &data ) );
    return data.m_hWnd;
}


HWND GetProcessTopWindowWnd( LPCTSTR lpWindowName, LPTSTR lpClassName )
{
    if ( lpClassName == NULL && lpWindowName == NULL )
    {
        return FALSE;
    }

    EnumWndStruct data = { lpClassName, lpWindowName, FALSE, NULL };
    EnumWindows( EnumWindowsProc2, reinterpret_cast<LPARAM>( &data ) );
    return data.m_hWnd;
}


HWND GetProcessTopWindowWnd2( LPCTSTR processName )
{
    HANDLE hSnapshot = ::CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if ( hSnapshot == INVALID_HANDLE_VALUE )
    {
        return NULL;
    }

    PROCESSENTRY32 pe;
    pe.dwSize = sizeof( PROCESSENTRY32 );
    if ( FALSE == ::Process32First( hSnapshot, &pe ) )
    {
        ::CloseHandle( hSnapshot );
        return NULL;
    }

    do
    {
        if ( _tcscmp( pe.szExeFile, processName ) == 0 )
        {
            ::CloseHandle( hSnapshot );
            return GetProcessTopWindowWnd( pe.th32ProcessID );
        }

        pe.dwSize = sizeof( PROCESSENTRY32 );
    } while ( Process32Next( hSnapshot, &pe ) != FALSE );

    ::CloseHandle( hSnapshot );
    return NULL;
}


HANDLE GetProcessHandle( DWORD processID )
{
    if (IsProcessExist(processID) == FALSE)
    {
        return NULL;
    }

    return ::OpenProcess( PROCESS_QUERY_INFORMATION, FALSE, processID );
}


HANDLE GetProcessHandle( LPCTSTR lpWindowName, LPTSTR lpClassName )
{
    HWND hWnd = GetProcessTopWindowWnd( lpWindowName, lpClassName );
    if ( hWnd != NULL)
    {
        DWORD processID = 0;
        GetWindowThreadProcessId( hWnd, &processID );
        return GetProcessHandle( processID );
    }
    return NULL;
}


HANDLE GetProcessHandle2( LPCTSTR processName )
{
    HWND hWnd = GetProcessTopWindowWnd( processName );
    if ( hWnd != NULL )
    {
        DWORD processID = 0;
        GetWindowThreadProcessId( hWnd, &processID );
        return GetProcessHandle( processID );
    }
    return NULL;
}


void GetCurrentSystemInfo( LPSYSTEM_INFO lpSystemInfo )
{
    ::GetNativeSystemInfo( lpSystemInfo );
}


DWORD GetSystemProcessorArchitecture()
{
    SYSTEM_INFO  sysInfo;
    GetCurrentSystemInfo( &sysInfo );
    return sysInfo.wProcessorArchitecture;
}


DWORD GetSystemPageSize()
{
    SYSTEM_INFO  sysInfo;
    GetCurrentSystemInfo( &sysInfo );
    return sysInfo.dwPageSize;
}


DWORD GetSystemProcessorNumber()
{
    SYSTEM_INFO  sysInfo;
    GetCurrentSystemInfo( &sysInfo );
    return sysInfo.dwNumberOfProcessors;
}


BOOL GetOSInfo( LPOSVERSIONINFO  lpOSInfo )
{
    return ::GetVersionEx( lpOSInfo );
}


LPVOID GetFileInfo( VS_FIXEDFILEINFO **pvffi )
{
    LPVOID pData = NULL;
    TCHAR pathName[MAX_PATH] = { 0 };
    HMODULE hModule = ::LoadLibrary( _T( "Kernel32.dll" ) );
    if ( hModule == NULL )
    {
        return pData;
    }

    if ( GetModuleFileName( hModule, pathName, MAX_PATH ) == 0 )
    {
        goto Label;
    }

    DWORD dHandle;
    DWORD verSize = GetFileVersionInfoSize( pathName, &dHandle );
    if ( verSize == 0 )
    {
        goto Label;
    }

    pData = ( LPVOID ) new BYTE[verSize + 1];
    if ( pData== NULL || GetFileVersionInfo( pathName, dHandle, verSize, pData ) == FALSE )
    {
        pData == NULL ? delete[]pData : TRUE;
        pData = NULL;
        goto Label;
    }

    
    UINT uLen = 0;
    if ( VerQueryValue( pData, L"\\", (LPVOID *)pvffi, &uLen ) == 0 )
    {
        delete[]pData;
        pData = NULL;
        goto Label;
    }

Label:
    FreeLibrary( hModule );
    return pData;
}


BOOL GetOSInfo( DWORD &versionMS, DWORD &versionLS )
{
    VS_FIXEDFILEINFO *pvffi = NULL;
    LPVOID pData = GetFileInfo( &pvffi );
    if ( pData == NULL )
    {
        return FALSE;
    }

    versionMS = pvffi->dwProductVersionMS;
    versionLS = pvffi->dwProductVersionLS;

    delete[]pData;
    return TRUE;
}


BOOL GetOSInfo( DWORD nVer[] )
{
    VS_FIXEDFILEINFO *pvffi = NULL;
    LPVOID pData = GetFileInfo( &pvffi );
    if ( pData == NULL )
    {
        return FALSE;
    }

    nVer[0] = HIWORD( pvffi->dwProductVersionMS );
    nVer[1] = LOWORD( pvffi->dwProductVersionMS );
    nVer[2] = HIWORD( pvffi->dwProductVersionLS );
    nVer[3] = LOWORD( pvffi->dwProductVersionLS );

    delete[]pData;
    return TRUE;
}