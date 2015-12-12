///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   file_helper.h
/// 	@brief	文件辅助工具. 
/// 
///		主要提供文件、目录查找、移除、创建等操作  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/02  8:54:24
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _FILE_HELPER_H_
#define     _FILE_HELPER_H_

#include "utility.h"


//  供外部直接使用的方便接口(基本上采用了ATL、shellapi中的接口外壳函数封装)

//  文件、目录操作(SHFileOperation)
/**
  *  @brief  目录、文件操作(被以下SH开头函数调用，也可外部需要时任意调用) 
  *  @param  [in] 	pszSrc          源文件目录 
  *  @param  [in] 	pszDest         目标文件目录 
  *  @param  [in] 	wFunc           指定操作方式
  *  @return 				        返回操作成功与否状态 
  */
BOOL OperateDirOrFile( const TCHAR * pszSrc, const TCHAR * pszDest, UINT wFunc );

/**
*  @brief  拷贝目录、文件操作
*  @param  [in] 	pszSrc          源文件目录
*  @param  [in] 	pszDest         目标文件目录
*  @param  [in] 	bFailIfExists   失败时是否直接返回
*  @return 				            返回操作成功与否状态
*/
BOOL SHCopyFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists = FALSE );

/**
*  @brief  移动目录、文件操作
*  @param  [in] 	pszSrc          源文件目录
*  @param  [in] 	pszDest         目标文件目录
*  @param  [in] 	bFailIfExists   失败时是否直接返回
*  @return 				            返回操作成功与否状态
*/
BOOL SHMoveFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists = FALSE );

/**
*  @brief  重命名目录、文件操作
*  @param  [in] 	pszSrc          源文件目录
*  @param  [in] 	pszDest         目标文件目录
*  @param  [in] 	bFailIfExists   失败时是否直接返回
*  @return 				            返回操作成功与否状态
*/
BOOL SHRenameFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists );

/**
*  @brief  删除目录、文件操作
*  @param  [in] 	pszSrc          源文件目录
*  @param  [in] 	pszDest         目标文件目录
*  @param  [in] 	bFailIfExists   文件不存在则返回直接成功
*  @return 				            返回操作成功与否状态
*/
BOOL SHDeleteFile( LPCTSTR pszFile, BOOL bSucceedIfNotExists = TRUE );

//  创建快捷方式
HRESULT CreateShortcut( LPCTSTR lpszFileName, LPCTSTR lpszDesc, LPCTSTR lpszShortcutPath, LPCTSTR lpszWorkDir, LPCTSTR pszArguments = NULL );

//  文件或目录是否存在
BOOL IsFileOrDictoryExists( LPCTSTR pszFile );

//  文件是否存在
BOOL IsFileExists( LPCTSTR pszFile );

//  目录是否存在
BOOL IsDictoryExists( LPCTSTR pszFile );

//  循环移动目录，并排除pszIgnoreFile
BOOL CycleMoveDir( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile );

//  循环移动目录，并排除pszIgnoreFile,此外内部含移动检查以尽可能保证移动成功
BOOL CycleMoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile );

//  循环拷贝目录
BOOL CycleCopyDir( LPCTSTR pszSrc, LPCTSTR pszDest );

//  循环拷贝目录，若目标目录存在，则先删除目标目录
BOOL CopyAllDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL RenameDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL MoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL RemoveAllDir( LPCTSTR pszPath );

BOOL GetParentPath( const CString& strPath, CString& strParentPath );

BOOL GetFileNameFromPath( const CString& strPath, CString& strFileName, BOOL isIncludeExternName = TRUE, const CString& strExternName = _T( "" ) );

BOOL CycleRenameDir( LPCTSTR pszSrc, LPCTSTR pszDest );

BOOL CycleDeleteDir( LPCTSTR pszSrc );


//  以下路径参数为windows下格式形如：C：\\file\\abc.txt
// 获取文件路径下的文件名称
CString GetFileName( LPCTSTR lpszFullFileName );

// 获取文件路径下的文件路径
CString GetFilePath( LPCTSTR lpszFullFileName );

// 获取指定模块下的路径
CString GetModulePath( HMODULE hModule = NULL );


//  FileFind封装
class CFileFinder
{
public:

    CFileFinder( const stlstring &szName );

    CFileFinder( const TCHAR *pszName );

    virtual ~CFileFinder( );

    BOOL FindNext();

    inline BOOL MatchAttributesMask( const U32 nMask )
    {
        return m_findData.dwFileAttributes & nMask;
    }

    inline BOOL IsArchived()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_ARCHIVE ); 
    }

    inline BOOL IsCompressed()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_COMPRESSED ); 
    }

    inline BOOL IsDirectory()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_DIRECTORY ); 
    }

    inline BOOL IsHidden()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_HIDDEN ); 
    }

    inline BOOL IsNormal()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_NORMAL );
    }

    inline BOOL IsReadonly()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_READONLY );
    }

    inline BOOL IsSystem()
    { 
        return MatchAttributesMask( FILE_ATTRIBUTE_SYSTEM ); 
    }

    inline BOOL IsTemporary()
    {
        return MatchAttributesMask( FILE_ATTRIBUTE_TEMPORARY );
    }

    inline BOOL IsDot()
    { 
        return IsParentDot() || IsCurrentDot();
    }

    BOOL IsParentDot();

    BOOL IsCurrentDot();

    inline FILETIME GetCreattionTime()
    {
        return m_findData.ftCreationTime;
    }

    inline FILETIME GetLastAccessTime()
    { 
        return m_findData.ftLastAccessTime;
    }

    inline FILETIME GetLastWriteTime()
    { 
        return m_findData.ftLastWriteTime;
    }

    inline U64 GetFileSize()
    {
        return ( (U64)m_findData.nFileSizeHigh << 32 ) | m_findData.nFileSizeLow;
    }

    inline stlstring GetFilePath()
    { 
        return stlstring(m_findData.cFileName); 
    }

    stlstring GetFileName();

    inline DWORD GetReserved0()			
    { 
        return m_findData.dwReserved0;
    }

    inline DWORD GetReserved1()			
    { 
        return m_findData.dwReserved1; 
    }

    inline TCHAR* GetAlternateFileName()	
    { 
        return m_findData.cAlternateFileName;
    }

#ifdef _MAC

    inline DWORD GetFileType()			
    { 
        return m_findData.dwFileType; 
    }

    inline DWORD GetCreatorType()		
    { 
        return m_findData.dwCreatorType;
    }

    inline WORD GetFinderFlags()		
    { 
        return m_findData.wFinderFlags; 
    }

#endif

    inline const WIN32_FIND_DATA *GetData()
    { 
        return &m_findData;
    }

    inline const WIN32_FIND_DATA *operator->( )
    {
        &m_findData; 
    }

    inline const WIN32_FIND_DATA &operator*( )
    { 
        return m_findData; 
    }

    inline operator BOOL() const
    {
        return ( ( m_finderHandle != INVALID_HANDLE_VALUE ) || m_bIsDeviceRootDir );
    }


protected:

    WIN32_FIND_DATA     m_findData;

    HANDLE              m_finderHandle;

    bool                m_bIsDeviceRootDir;
};

#endif  //       _FILE_HELPER_H_
