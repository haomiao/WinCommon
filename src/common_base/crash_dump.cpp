#include "stdafx.h"
#include <crtdbg.h>
#include "file_helper.h"
#include "crash_dump.h"

#pragma comment(lib, "dbghelp.lib")

namespace
{
    //  默认dump文件名
    #define DEFAULT_DUMP_FILE_NAME  _T("Dump.dmp")


    //  判断当前路径是否有效
    BOOL IsFilePathValid( const TCHAR* szFilePath )
    {
        if ( szFilePath == NULL )
        {
            return FALSE;
        }

        CString tempSrtr = szFilePath;
        int pos = tempSrtr.ReverseFind( '\\' );
        tempSrtr = tempSrtr.Left( pos );
        CFileFinder fileFinder( tempSrtr.GetString() );
        return fileFinder.IsDirectory();
    }

    //  使得其他设置异常捕获失效
    void DisableSetUnhandledExceptionFilter()
    {
#ifdef WIN64
#pragma message("The DisableSetUnhandledExceptionFilter may be no useful,so we let it jump out...")
#pragma message( "But we can hook the function \"SetUnhandledExceptionFilter\" to another function also ok and \nmy be more useful, whether on the 64 or 32 target machine..." )
        return;
#endif
        HMODULE hDll = ::LoadLibrary( _T( "Kernel32.dll" ) );
        if ( hDll != NULL )
        {
            void *addr = (void *)::GetProcAddress( hDll, "SetUnhandledExceptionFilter" );
            if ( addr != NULL )
            {
                unsigned char code[16] = { 0 };
                int size = 0;
                code[size++] = 0x33;
                code[size++] = 0xC0;
                code[size++] = 0xC2;
                code[size++] = 0x04;
                code[size++] = 0x00;

                DWORD dwOldFlag, dwTempFlag;
                ::VirtualProtect( addr, size, PAGE_READWRITE, &dwOldFlag );
                ::WriteProcessMemory( ::GetCurrentProcess(), addr, code, size, NULL );
                ::VirtualProtect( addr, size, dwOldFlag, &dwTempFlag );
            }
            ::FreeLibrary( hDll );
        }
    }
}


CString CCrashDump::m_processWindowName = _T( "" );
CString CCrashDump::m_processWindowClassName = _T( "" );
CString CCrashDump::m_filePathName = _T( "" );
MINIDUMP_CALLBACK_ROUTINE CCrashDump::m_miniDumpCallbackFun = NULL;
PVOID CCrashDump::m_callbackFunParam = NULL;
_purecall_handler CCrashDump::m_oldPureCallbackFun = NULL;
_invalid_parameter_handler CCrashDump::m_oldInvalidParamCallbackFun = NULL;
PTOP_LEVEL_EXCEPTION_FILTER CCrashDump::m_oldUnHandledExceptionFilter = NULL;
HWND CCrashDump::m_processWnd = NULL;


CCrashDump::CCrashDump()
{
    m_oldUnHandledExceptionFilter = ::SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER)CustomUnhandledExceptionFilter );
    DisableSetUnhandledExceptionFilter();
}


CCrashDump::~CCrashDump()
{
    ::SetUnhandledExceptionFilter( m_oldUnHandledExceptionFilter );
    ::_set_purecall_handler( m_oldPureCallbackFun );

#if _MSC_VER > 1400
    ::_set_invalid_parameter_handler( m_oldInvalidParamCallbackFun );
#endif

    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE );
}


BOOL CCrashDump::SetCatchProcessWindow( LPCTSTR lpClassName, LPCTSTR lpWindowName )
{
    m_processWindowClassName = lpClassName;
    m_processWindowName = lpWindowName;

    m_processWnd = ::FindWindow( m_processWindowClassName, m_processWindowName );
    if (m_processWnd == NULL)
    {
        return FALSE;
    }

//     TCHAR msgBuffer[1024] = _T("UnKnown Message!");
//     COPYDATASTRUCT cpData;
//     cpData.cbData = sizeof( TCHAR ) * (_tcslen( msgBuffer) + 1);
//     cpData.lpData = msgBuffer;
//     ::SendMessage( m_processWnd, WM_COPYDATA, 0, (LPARAM)&cpData );

    return TRUE;
}


LONG WINAPI CCrashDump::CustomUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo )
{
    if ( !IsFilePathValid(m_filePathName.GetString() ) )
    {
        SetDefaultDumpPath();
    }

    BOOL res = CreateMiniDump(m_filePathName, pExceptionInfo);
    if (res == TRUE)
    {
        return EXCEPTION_EXECUTE_HANDLER;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}


BOOL CCrashDump::CreateMiniDump( LPCTSTR filePathName, PEXCEPTION_POINTERS pExceptionInfo )
{
    HANDLE hFile = ::CreateFile( filePathName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
    if ( hFile != NULL && hFile != INVALID_HANDLE_VALUE )
    {
        MINIDUMP_EXCEPTION_INFORMATION mdei;
        mdei.ThreadId = ::GetCurrentThreadId();
        mdei.ExceptionPointers = pExceptionInfo;
        mdei.ClientPointers = NULL;

        MINIDUMP_CALLBACK_INFORMATION mci;
        mci.CallbackParam = m_callbackFunParam;
        mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)( m_miniDumpCallbackFun );

        ::MiniDumpWriteDump( ::GetCurrentProcess(), ::GetCurrentProcessId(), hFile,
            MiniDumpNormal, ( pExceptionInfo != NULL ) ? &mdei : NULL, NULL, ( mci.CallbackRoutine != NULL) ? &mci : NULL);
        ::CloseHandle( hFile );

        CString strError;
        strError.Format( _T( "很抱歉,软件发生异常! 请将文件 \"%s\" 发送给相关负责人处理,谢谢！" ), filePathName );
        ::FatalAppExit( 0, strError );
        return TRUE;
    }

    return FALSE;
}


BOOL CCrashDump::SetDumpFilePath( LPCTSTR filePathName )
{
    if ( filePathName == NULL || !IsFilePathValid( filePathName ) )
    {
        SetDefaultDumpPath();
        return FALSE;
    }

    m_filePathName = filePathName;
    return TRUE;
}


void CCrashDump::SetDefaultDumpPath()
{
    TCHAR tempBuffer[_MAX_PATH + 1] = { 0 };
    ::GetModuleFileName( NULL, tempBuffer, _MAX_PATH );
    m_filePathName = tempBuffer;
    int pos = m_filePathName.ReverseFind( '\\' );
    m_filePathName = m_filePathName.Left( pos + 1 );
    m_filePathName += DEFAULT_DUMP_FILE_NAME;
}


void CCrashDump::SetMiniDumpCallback( MINIDUMP_CALLBACK_ROUTINE miniDumpCallbackFun, PVOID callbackFunParam )
{
    m_miniDumpCallbackFun   = miniDumpCallbackFun;
    m_callbackFunParam      = callbackFunParam;
}


void CCrashDump::SetPurecallHandlerCallback( _purecall_handler pureCallbackFun )
{
    m_oldPureCallbackFun = ::_set_purecall_handler( pureCallbackFun );
    if ( pureCallbackFun != NULL )
    {
        _CrtSetReportMode( _CRT_ASSERT, 0 );
    }
    else
    {
        _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE );
    }
}


BOOL CCrashDump::SetInvalidParameterHandlerCallback( _invalid_parameter_handler invalidParamCallbackFun )
{

#if _MSC_VER > 1400
    m_oldInvalidParamCallbackFun = ::_set_invalid_parameter_handler( invalidParamCallbackFun );
    if ( invalidParamCallbackFun != NULL )
    {
        _CrtSetReportMode( _CRT_ASSERT, 0 );
    }
    else
    {
        _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE );
    }
    return TRUE;
#endif

    return FALSE;
}

