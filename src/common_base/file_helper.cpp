#include "stdafx.h"
#include "file_helper.h"


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN			// 从 Windows 头中排除极少使用的资料
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef _WIN32_WCE
#ifndef _WIN32_WINNT			// 允许使用特定于 Windows NT 4 或更高版本的功能。
#define _WIN32_WINNT 0x0500	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif
#endif

#ifndef _AFX
#include <windows.h>
#include <tchar.h>
#include <atlstr.h>
#endif

#include <atlconv.h>
#include <atltime.h>
#include <atlfile.h>

#include <shellapi.h>
#include <Shlobj.h>


using namespace ATL;

#include <atlpath.h>
using namespace ATLPath;

namespace
{
    static LPCTSTR s_szDotCurrent = _T( "." );
    static LPCTSTR s_szDotParent = _T( ".." );
}


BOOL IsFileOrDictoryExists( LPCTSTR pszFile )
{
    BOOL bExists = ::GetFileAttributes( pszFile ) != INVALID_FILE_ATTRIBUTES;
    if ( !bExists )
    {
        DWORD dwError = ::GetLastError();
        bExists = (dwError != ERROR_FILE_NOT_FOUND) && (dwError != ERROR_PATH_NOT_FOUND);
    }

    return bExists;
}


BOOL IsFileExists( LPCTSTR pszFile )
{
    DWORD dwAttricutes = ::GetFileAttributes( pszFile );
    return ( ( dwAttricutes != INVALID_FILE_ATTRIBUTES )
        && ( dwAttricutes & FILE_ATTRIBUTE_DIRECTORY ) == 0 );
}


BOOL IsDictoryExists( LPCTSTR pszFile )
{
    DWORD dwAttricutes = ::GetFileAttributes( pszFile );
    return ( ( dwAttricutes != INVALID_FILE_ATTRIBUTES )
        && ( dwAttricutes & FILE_ATTRIBUTE_DIRECTORY ) != 0 );
}


BOOL OperateDirOrFile( const TCHAR * pszSrc, const TCHAR * pszDest, UINT wFunc )
{
    TCHAR src[MAX_PATH] = { 0 };
    TCHAR dest[MAX_PATH] = { 0 };
    if ( pszSrc )
        _tcscpy_s( src, MAX_PATH, pszSrc );
    if ( pszDest )
        _tcscpy_s( dest, MAX_PATH, pszDest );

    SHFILEOPSTRUCT   filestruct;
    filestruct.wFunc = wFunc;
    filestruct.pFrom = src;
    filestruct.pTo = dest;
    filestruct.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT | FOF_NOERRORUI;

    return ( SHFileOperation( &filestruct ) == 0 ) ? TRUE : FALSE;
}


BOOL SHCopyFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists )
{
    if ( bFailIfExists && ::FileExists( pszDestFile ) )
        return FALSE;

    return OperateDirOrFile( pszSrcFile, pszDestFile, FO_COPY );
}


BOOL SHMoveFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists )
{
    if ( bFailIfExists && ::FileExists( pszDestFile ) )
        return FALSE;

    return OperateDirOrFile( pszSrcFile, pszDestFile, FO_MOVE );
}


BOOL SHRenameFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists )
{
    if ( bFailIfExists && ::FileExists( pszDestFile ) )
        return FALSE;

    return OperateDirOrFile( pszSrcFile, pszDestFile, FO_RENAME );
}


BOOL SHDeleteFile( LPCTSTR pszFile, BOOL bSucceedIfNotExists )
{
    if ( bSucceedIfNotExists && !::FileExists( pszFile ) )
        return TRUE;

    return OperateDirOrFile( pszFile, NULL, FO_DELETE );
}


HRESULT CreateShortcut( LPCTSTR lpszFileName, LPCTSTR lpszDesc, LPCTSTR lpszShortcutPath, LPCTSTR lpszWorkDir, LPCTSTR pszArguments )
{
    USES_CONVERSION;

    HRESULT hRes = E_FAIL;

    LPTSTR lpszFilePart;
    TCHAR szPath[MAX_PATH];
    CComPtr<IShellLink> ipShellLink;

    if ( ::GetFullPathName( lpszFileName, MAX_PATH, szPath, &lpszFilePart ) )
        if ( SUCCEEDED( hRes = ipShellLink.CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER ) ) )
            if ( SUCCEEDED( hRes = ipShellLink->SetPath( szPath ) ) )
                if ( SUCCEEDED( hRes = ipShellLink->SetDescription( lpszDesc ) ) )
                    if ( SUCCEEDED( hRes = ipShellLink->SetWorkingDirectory( lpszWorkDir ) ) )
                        if ( !pszArguments || SUCCEEDED( hRes = ipShellLink->SetArguments( pszArguments ) ) )
                        {
                            CComQIPtr<IPersistFile> ipPersistFile( ipShellLink );
                            hRes = ipPersistFile->Save( T2W( (LPTSTR)lpszShortcutPath ), TRUE );
                        }

    return hRes;
}


BOOL CycleMoveDir( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile )
{
    if ( !::FileExists( pszDest ) && !::CreateDirectory( pszDest, NULL ) )
        return FALSE;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    TCHAR pszFind[MAX_PATH] = { 0 };
    TCHAR newfilename[MAX_PATH] = { 0 };
    TCHAR pszDestSubDir[MAX_PATH] = { 0 };
    TCHAR pszRoot[MAX_PATH] = { 0 };

    _tcscpy_s( pszRoot, MAX_PATH, pszSrc );

    if ( pszRoot[_tcslen( pszRoot ) - 1] != _T( '\\' ) )
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s\\*.*" ), pszRoot );
        _tcscat_s( pszRoot, MAX_PATH, _T( "\\" ) );
    }
    else
        _stprintf_s( pszFind, MAX_PATH, _T( "%s*.*" ), pszRoot );


    hFind = ::FindFirstFile( pszFind, &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE )
        return FALSE;

    CString strIgnoreFile( pszIgnoreFile );
    if ( strIgnoreFile.GetLength() > 0 && strIgnoreFile[strIgnoreFile.GetLength() - 1] != '|' )
        strIgnoreFile += '|';
    strIgnoreFile.MakeLower();

    BOOL bIsOK = TRUE;
    TCHAR filename[MAX_PATH] = { 0 };

    do{
        if ( _tcsicmp( FindFileData.cFileName, _T( "." ) ) == 0 || _tcsicmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
            continue;
        if ( strIgnoreFile.GetLength() > 0 )
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s|" ), CString( FindFileData.cFileName ).MakeLower() );
            if ( strIgnoreFile.Find( filename ) != -1 )
                continue;
        }

        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _stprintf_s( pszFind, MAX_PATH, _T( "%s%s\\" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( pszDestSubDir, MAX_PATH, _T( "%s\\%s\\" ), pszDest, FindFileData.cFileName );

            if ( ::CreateDirectory( pszDestSubDir, NULL ) )
            {
                if ( CycleMoveDir( pszFind, pszDestSubDir, _T( "" ) ) )
                    bIsOK = SHDeleteFile( pszFind );
                else
                    bIsOK = FALSE;
            }
            else
                bIsOK = FALSE;
        }
        else
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s%s" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( newfilename, MAX_PATH, _T( "%s\\%s" ), pszDest, FindFileData.cFileName );

            bIsOK = SHMoveFile( filename, newfilename );
        }

        if ( !bIsOK )
            break;

    } while ( ::FindNextFile( hFind, &FindFileData ) );

    ::FindClose( hFind );

    if ( bIsOK )
        bIsOK = SHDeleteFile( pszSrc );

    return bIsOK;
}


BOOL CycleMoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile )
{
    if ( !::FileExists( pszDest ) && !::CreateDirectory( pszDest, NULL ) )
        return FALSE;

    WIN32_FIND_DATA FindFileData;
    HANDLE hFind = INVALID_HANDLE_VALUE;

    TCHAR pszFind[MAX_PATH] = { 0 };
    TCHAR newfilename[MAX_PATH] = { 0 };
    TCHAR pszDestSubDir[MAX_PATH] = { 0 };
    TCHAR pszRoot[MAX_PATH] = { 0 };

    _tcscpy_s( pszRoot, MAX_PATH, pszSrc );

    if ( pszRoot[_tcslen( pszRoot ) - 1] != _T( '\\' ) )
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s\\*.*" ), pszRoot );
        _tcscat_s( pszRoot, MAX_PATH, _T( "\\" ) );
    }
    else
        _stprintf_s( pszFind, MAX_PATH, _T( "%s*.*" ), pszRoot );


    hFind = ::FindFirstFile( pszFind, &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE )
        return FALSE;

    CString strIgnoreFile( pszIgnoreFile );
    if ( strIgnoreFile.GetLength() > 0 && strIgnoreFile[strIgnoreFile.GetLength() - 1] != '|' )
        strIgnoreFile += '|';
    strIgnoreFile.MakeLower();

    BOOL bIsOK = TRUE;
    TCHAR filename[MAX_PATH] = { 0 };

    do{
        if ( _tcsicmp( FindFileData.cFileName, _T( "." ) ) == 0 || _tcsicmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
            continue;
        if ( strIgnoreFile.GetLength() > 0 )
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s|" ), CString( FindFileData.cFileName ).MakeLower() );
            if ( strIgnoreFile.Find( filename ) != -1 )
                continue;
        }

        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _stprintf_s( pszFind, MAX_PATH, _T( "%s%s\\" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( pszDestSubDir, MAX_PATH, _T( "%s\\%s\\" ), pszDest, FindFileData.cFileName );

            if ( ::CreateDirectory( pszDestSubDir, NULL ) )
            {
                if ( CycleMoveDirWithCheck( pszFind, pszDestSubDir, _T( "" ) ) )
                    bIsOK = SHDeleteFile( pszFind );
                else
                    bIsOK = FALSE;
            }
            else
                bIsOK = FALSE;
        }
        else
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s%s" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( newfilename, MAX_PATH, _T( "%s\\%s" ), pszDest, FindFileData.cFileName );

            if ( ::FileExists( newfilename ) && !SHDeleteFile( newfilename ) )
            {
                // 改变当前文件名，保证文件移动尽可能成功
                SYSTEMTIME st;
                ::GetLocalTime( &st );
                CString strBKFileName;
                strBKFileName.Format( _T( "%s.%04d%02d%02d%02d%02d%02d%03d.bk" ),
                    newfilename, st.wYear, st.wMonth, st.wDay,
                    st.wHour, st.wMinute, st.wSecond, st.wMilliseconds );

                bIsOK = SHMoveFile( newfilename, strBKFileName );
            }

            if ( bIsOK )
                bIsOK = SHMoveFile( filename, newfilename );
        }

        if ( !bIsOK )
            break;

    } while ( ::FindNextFile( hFind, &FindFileData ) );

    ::FindClose( hFind );

    if ( bIsOK )
        bIsOK = SHDeleteFile( pszSrc );

    return bIsOK;
}


BOOL CycleRenameDir( LPCTSTR pszSrc, LPCTSTR pszDest )
{
    if ( !::FileExists( pszDest ) && !::CreateDirectory( pszDest, NULL ) )
        return FALSE;

    WIN32_FIND_DATA FindFileData;

    TCHAR pszFind[MAX_PATH] = { 0 };
    TCHAR newfilename[MAX_PATH] = { 0 };
    TCHAR pszDestSubDir[MAX_PATH] = { 0 };
    TCHAR pszRoot[MAX_PATH] = { 0 };

    _tcscpy_s( pszRoot, MAX_PATH, pszSrc );

    if ( pszRoot[_tcslen( pszRoot ) - 1] != _T( '\\' ) )
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s\\*.*" ), pszRoot );
        _tcscat_s( pszRoot, MAX_PATH, _T( "\\" ) );
    }
    else
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s*.*" ), pszRoot );
    }


    HANDLE hFind = ::FindFirstFile( pszFind, &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE )
        return FALSE;

    BOOL bIsOK = TRUE;
    TCHAR filename[MAX_PATH] = { 0 };

    do{
        if ( _tcsicmp( FindFileData.cFileName, _T( "." ) ) == 0 || _tcsicmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
            continue;
        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _stprintf_s( pszFind, MAX_PATH, _T( "%s%s\\" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( pszDestSubDir, MAX_PATH, _T( "%s\\%s\\" ), pszDest, FindFileData.cFileName );

            if ( ::CreateDirectory( pszDestSubDir, NULL ) )
            {
                if ( CycleRenameDir( pszFind, pszDestSubDir ) )
                    bIsOK = SHDeleteFile( pszFind );
                else
                    bIsOK = FALSE;
            }
            else
                bIsOK = FALSE;
        }
        else
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s%s" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( newfilename, MAX_PATH, _T( "%s\\%s" ), pszDest, FindFileData.cFileName );

            bIsOK = SHMoveFile( filename, newfilename );
        }

        if ( !bIsOK )
            break;

    } while ( ::FindNextFile( hFind, &FindFileData ) );

    ::FindClose( hFind );

    if ( bIsOK )
        bIsOK = SHDeleteFile( pszSrc );

    return bIsOK;
}


BOOL CycleDeleteDir( LPCTSTR pszSrc )
{
    WIN32_FIND_DATA FindFileData;

    TCHAR pszFind[MAX_PATH] = { 0 };
    TCHAR newfilename[MAX_PATH] = { 0 };
    TCHAR pszRoot[MAX_PATH] = { 0 };

    _tcscpy_s( pszRoot, MAX_PATH, pszSrc );

    if ( pszRoot[_tcslen( pszRoot ) - 1] != _T( '\\' ) )
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s\\*.*" ), pszRoot );
        _tcscat_s( pszRoot, MAX_PATH, _T( "\\" ) );
    }
    else
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s*.*" ), pszRoot );
    }


    HANDLE hFind = ::FindFirstFile( pszFind, &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE )
        return FALSE;

    BOOL bIsOK = TRUE;
    TCHAR filename[MAX_PATH] = { 0 };

    do{
        if ( _tcsicmp( FindFileData.cFileName, _T( "." ) ) == 0 || _tcsicmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
            continue;
        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _stprintf_s( pszFind, MAX_PATH, _T( "%s%s\\" ), pszRoot, FindFileData.cFileName );

            if ( CycleDeleteDir( pszFind ) )
                bIsOK = SHDeleteFile( pszFind );
            else
                bIsOK = FALSE;
        }
        else
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s%s" ), pszRoot, FindFileData.cFileName );

            bIsOK = SHDeleteFile( filename );
        }

        if ( !bIsOK )
            break;

    } while ( ::FindNextFile( hFind, &FindFileData ) );

    ::FindClose( hFind );

    if ( bIsOK )
        bIsOK = SHDeleteFile( pszRoot );

    return bIsOK;
}


BOOL CycleCopyDir( LPCTSTR pszSrc, LPCTSTR pszDest )
{
    if ( !::FileExists( pszDest ) && !::CreateDirectory( pszDest, NULL ) )
        return FALSE;

    WIN32_FIND_DATA FindFileData;

    TCHAR pszFind[MAX_PATH] = { 0 };
    TCHAR newfilename[MAX_PATH] = { 0 };
    TCHAR pszDestSubDir[MAX_PATH] = { 0 };
    TCHAR pszRoot[MAX_PATH] = { 0 };

    _tcscpy_s( pszRoot, MAX_PATH, pszSrc );

    if ( pszRoot[_tcslen( pszRoot ) - 1] != _T( '\\' ) )
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s\\*.*" ), pszRoot );
        _tcscat_s( pszRoot, MAX_PATH, _T( "\\" ) );
    }
    else
    {
        _stprintf_s( pszFind, MAX_PATH, _T( "%s*.*" ), pszRoot );
    }


    HANDLE hFind = ::FindFirstFile( pszFind, &FindFileData );

    if ( hFind == INVALID_HANDLE_VALUE )
        return FALSE;

    BOOL bIsOK = TRUE;
    TCHAR filename[MAX_PATH] = { 0 };

    do{
        if ( _tcsicmp( FindFileData.cFileName, _T( "." ) ) == 0 || _tcsicmp( FindFileData.cFileName, _T( ".." ) ) == 0 )
            continue;
        if ( FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
        {
            _stprintf_s( pszFind, MAX_PATH, _T( "%s%s\\" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( pszDestSubDir, MAX_PATH, _T( "%s\\%s\\" ), pszDest, FindFileData.cFileName );

            if ( ::CreateDirectory( pszDestSubDir, NULL ) )
                bIsOK = CycleCopyDir( pszFind, pszDestSubDir );
            else
                bIsOK = FALSE;
        }
        else
        {
            _stprintf_s( filename, MAX_PATH, _T( "%s%s" ), pszRoot, FindFileData.cFileName );
            _stprintf_s( newfilename, MAX_PATH, _T( "%s\\%s" ), pszDest, FindFileData.cFileName );

            bIsOK = SHCopyFile( filename, newfilename );
        }

        if ( !bIsOK )
            break;

    } while ( ::FindNextFile( hFind, &FindFileData ) );

    ::FindClose( hFind );

    return TRUE;
}


BOOL RenameDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir )
{
    if ( bClearDestDir && ::FileExists( pszDest ) )
        RemoveAllDir( pszDest );

#ifndef _WIN32_WCE
    BOOL bRet = ( _trename( pszSrc, pszDest ) == NO_ERROR ) ? TRUE : FALSE;
#else
    BOOL bRet = SHDeleteFile( pszSrc, pszDest, TRUE );
#endif

    if ( !bRet )
    {
        if ( !::FileExists( pszDest ) )
            bRet = SHMoveFile( pszSrc, pszDest );
        else
        {
            CString strSrc = pszSrc;
            strSrc.TrimRight( _T( "\\/ " ) );
            strSrc += _T( "\\*.*" );
            bRet = SHMoveFile( strSrc, pszDest );
            if ( bRet )
                SHDeleteFile( pszSrc );
        }
    }

    if ( !bRet )
        bRet = CycleRenameDir( pszSrc, pszDest );

    return bRet;
}


BOOL MoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir )
{
    if ( bClearDestDir && ::FileExists( pszDest ) )
        RemoveAllDir( pszDest );

    BOOL bRet = FALSE;
    if ( !::FileExists( pszDest ) )
        bRet = SHMoveFile( pszSrc, pszDest );
    else
    {
        CString strSrc = pszSrc;
        strSrc.TrimRight( _T( "\\/ " ) );
        strSrc += _T( "\\*.*" );
        bRet = SHMoveFile( strSrc, pszDest );
        if ( bRet )
            SHDeleteFile( pszSrc );
    }

    if ( !bRet )
        bRet = CycleMoveDirWithCheck( pszSrc, pszDest, NULL );

    return bRet;
}


BOOL CopyAllDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir )
{
    if ( bClearDestDir && ::FileExists( pszDest ) )
        RemoveAllDir( pszDest );

    BOOL bRet = FALSE;
    if ( !::FileExists( pszDest ) )
        bRet = SHCopyFile( pszSrc, pszDest );
    else
    {
        CString strSrc = pszSrc;
        strSrc.TrimRight( _T( "\\/ " ) );
        strSrc += _T( "\\*.*" );
        bRet = SHCopyFile( strSrc, pszDest );
    }

    if ( !bRet )
        bRet = CycleCopyDir( pszSrc, pszDest );

    return bRet;
}


BOOL RemoveAllDir( LPCTSTR pszPath )
{
    if ( !SHDeleteFile( pszPath ) )
        return CycleDeleteDir( pszPath );
    else
        return TRUE;
}


BOOL GetParentPath( const CString& strPath, CString& strParentPath )
{
    CString strTemp = strPath;
    strTemp.Trim( _T( "\\/ " ) );

    int iPos = strTemp.ReverseFind( '\\' );
    if ( iPos == -1 )
        iPos = strTemp.ReverseFind( '/' );
    if ( iPos != -1 )
        strParentPath = strTemp.Left( iPos );

    return ( iPos != -1 );
}


BOOL GetFileNameFromPath( const CString& strPath, CString& strFileName, BOOL isIncludeExternName, const CString& strExternName )
{
    CString strRealPath = strPath;
    strRealPath.Trim();

    int pos1 = strRealPath.ReverseFind( '\\' );
    int pos2 = strRealPath.ReverseFind( '/' );
    int begin = max( pos1, pos2 ) + 1;

    strFileName = strRealPath.Right( strRealPath.GetLength() - begin );
    int len = strFileName.GetLength();
    if ( len > 0 && !isIncludeExternName )
    {
        CString strRealExternName = strExternName;
        strRealExternName.Trim();
        if ( strRealExternName.GetLength() > 0 )
        {
            if ( strRealExternName.GetAt( 0 ) != '.' )
                strRealExternName.Insert( 0, '.' );

            int exlen = strRealExternName.GetLength();
            if ( len >= exlen )
            {
                if ( strFileName.Right( exlen ).CompareNoCase( strRealExternName ) == 0 )
                    strFileName = strFileName.Left( len - exlen );
            }
        }
        else
        {
            int dot = strFileName.ReverseFind( '.' );
            if ( dot != -1 )
                strFileName = strFileName.Left( dot );
        }
    }

    return !strFileName.IsEmpty();
}


CString GetFileName( LPCTSTR lpszFullFileName )
{
    CString tempStr = lpszFullFileName;
    if ( lpszFullFileName == NULL)
    {
        return tempStr;
    }

    int pos = tempStr.ReverseFind( _T( '\\' ) );
    if (pos != -1)
    {
        tempStr = tempStr.Right(tempStr.GetLength() - pos - 1);
    }

    return tempStr;
}


CString GetFilePath( LPCTSTR lpszFullFileName )
{
    CString tempStr = lpszFullFileName;
    if ( lpszFullFileName == NULL )
    {
        return tempStr;
    }

    int pos = tempStr.ReverseFind( _T( '\\' ) );
    if ( pos != -1 )
    {
        tempStr = tempStr.Left(pos);
    }

    return tempStr;
}


CString GetModulePath( HMODULE hModule )
{
    TCHAR pathBuffer[MAX_PATH] = { 0 };
    if (0 != ::GetModuleFileName( hModule, pathBuffer, MAX_PATH ) )
    {
        CString tempStr = pathBuffer;
        int pos = tempStr.ReverseFind( _T( '\\' ) );
        tempStr = tempStr.Left( pos );
        return tempStr;
    }

    return _T("");
}


CFileFinder::CFileFinder( const stlstring &szName ):
m_finderHandle( INVALID_HANDLE_VALUE ),
m_bIsDeviceRootDir( false )
{
    m_findData.cFileName[0] = 0;
    m_findData.cAlternateFileName[0] = 0;

    m_finderHandle = ::FindFirstFile( szName.data(), &m_findData );
    if ( m_finderHandle == INVALID_HANDLE_VALUE )
        m_bIsDeviceRootDir = ::GetFileAttributes( szName.data() ) != INVALID_FILE_ATTRIBUTES;
}


CFileFinder::CFileFinder( const TCHAR *pszName ):
m_finderHandle( INVALID_HANDLE_VALUE ),
m_bIsDeviceRootDir( false )
{
    m_findData.cFileName[0] = 0;
    m_findData.cAlternateFileName[0] = 0;

    m_finderHandle = ::FindFirstFile( pszName, &m_findData );
    if ( m_finderHandle == INVALID_HANDLE_VALUE )
        m_bIsDeviceRootDir = ::GetFileAttributes( pszName ) != INVALID_FILE_ATTRIBUTES;
}


CFileFinder::~CFileFinder( )
{
    if ( m_finderHandle != INVALID_HANDLE_VALUE )
    {
        ::FindClose( m_finderHandle );
        m_finderHandle = INVALID_HANDLE_VALUE;
    }
}


BOOL CFileFinder::FindNext()
{
    if ( m_finderHandle == INVALID_HANDLE_VALUE )
    {
        return FALSE;
    }
    return ::FindNextFile( m_finderHandle, &m_findData );
}


stlstring CFileFinder::GetFileName()
{
    stlstring szPath( GetFilePath() );
    size_t nPos( szPath.rfind( '\\' ) );
    return ( nPos != stlstring::npos ) ? szPath.substr( nPos ) : szPath;
}


BOOL CFileFinder::IsParentDot()
{
    return !_tcscmp( m_findData.cFileName, s_szDotParent );
}


BOOL CFileFinder::IsCurrentDot()
{
    return !_tcscmp( m_findData.cFileName, s_szDotCurrent );
}
