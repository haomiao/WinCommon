///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   file_helper.h
/// 	@brief	�ļ���������. 
/// 
///		��Ҫ�ṩ�ļ���Ŀ¼���ҡ��Ƴ��������Ȳ���  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/02  8:54:24
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _FILE_HELPER_H_
#define     _FILE_HELPER_H_

#include "utility.h"


//  ���ⲿֱ��ʹ�õķ���ӿ�(�����ϲ�����ATL��shellapi�еĽӿ���Ǻ�����װ)

//  �ļ���Ŀ¼����(SHFileOperation)
/**
  *  @brief  Ŀ¼���ļ�����(������SH��ͷ�������ã�Ҳ���ⲿ��Ҫʱ�������) 
  *  @param  [in] 	pszSrc          Դ�ļ�Ŀ¼ 
  *  @param  [in] 	pszDest         Ŀ���ļ�Ŀ¼ 
  *  @param  [in] 	wFunc           ָ��������ʽ
  *  @return 				        ���ز����ɹ����״̬ 
  */
BOOL OperateDirOrFile( const TCHAR * pszSrc, const TCHAR * pszDest, UINT wFunc );

/**
*  @brief  ����Ŀ¼���ļ�����
*  @param  [in] 	pszSrc          Դ�ļ�Ŀ¼
*  @param  [in] 	pszDest         Ŀ���ļ�Ŀ¼
*  @param  [in] 	bFailIfExists   ʧ��ʱ�Ƿ�ֱ�ӷ���
*  @return 				            ���ز����ɹ����״̬
*/
BOOL SHCopyFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists = FALSE );

/**
*  @brief  �ƶ�Ŀ¼���ļ�����
*  @param  [in] 	pszSrc          Դ�ļ�Ŀ¼
*  @param  [in] 	pszDest         Ŀ���ļ�Ŀ¼
*  @param  [in] 	bFailIfExists   ʧ��ʱ�Ƿ�ֱ�ӷ���
*  @return 				            ���ز����ɹ����״̬
*/
BOOL SHMoveFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists = FALSE );

/**
*  @brief  ������Ŀ¼���ļ�����
*  @param  [in] 	pszSrc          Դ�ļ�Ŀ¼
*  @param  [in] 	pszDest         Ŀ���ļ�Ŀ¼
*  @param  [in] 	bFailIfExists   ʧ��ʱ�Ƿ�ֱ�ӷ���
*  @return 				            ���ز����ɹ����״̬
*/
BOOL SHRenameFile( LPCTSTR pszSrcFile, LPCTSTR pszDestFile, BOOL bFailIfExists );

/**
*  @brief  ɾ��Ŀ¼���ļ�����
*  @param  [in] 	pszSrc          Դ�ļ�Ŀ¼
*  @param  [in] 	pszDest         Ŀ���ļ�Ŀ¼
*  @param  [in] 	bFailIfExists   �ļ��������򷵻�ֱ�ӳɹ�
*  @return 				            ���ز����ɹ����״̬
*/
BOOL SHDeleteFile( LPCTSTR pszFile, BOOL bSucceedIfNotExists = TRUE );

//  ������ݷ�ʽ
HRESULT CreateShortcut( LPCTSTR lpszFileName, LPCTSTR lpszDesc, LPCTSTR lpszShortcutPath, LPCTSTR lpszWorkDir, LPCTSTR pszArguments = NULL );

//  �ļ���Ŀ¼�Ƿ����
BOOL IsFileOrDictoryExists( LPCTSTR pszFile );

//  �ļ��Ƿ����
BOOL IsFileExists( LPCTSTR pszFile );

//  Ŀ¼�Ƿ����
BOOL IsDictoryExists( LPCTSTR pszFile );

//  ѭ���ƶ�Ŀ¼�����ų�pszIgnoreFile
BOOL CycleMoveDir( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile );

//  ѭ���ƶ�Ŀ¼�����ų�pszIgnoreFile,�����ڲ����ƶ�����Ծ����ܱ�֤�ƶ��ɹ�
BOOL CycleMoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, LPCTSTR pszIgnoreFile );

//  ѭ������Ŀ¼
BOOL CycleCopyDir( LPCTSTR pszSrc, LPCTSTR pszDest );

//  ѭ������Ŀ¼����Ŀ��Ŀ¼���ڣ�����ɾ��Ŀ��Ŀ¼
BOOL CopyAllDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL RenameDir( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL MoveDirWithCheck( LPCTSTR pszSrc, LPCTSTR pszDest, BOOL bClearDestDir = TRUE );

BOOL RemoveAllDir( LPCTSTR pszPath );

BOOL GetParentPath( const CString& strPath, CString& strParentPath );

BOOL GetFileNameFromPath( const CString& strPath, CString& strFileName, BOOL isIncludeExternName = TRUE, const CString& strExternName = _T( "" ) );

BOOL CycleRenameDir( LPCTSTR pszSrc, LPCTSTR pszDest );

BOOL CycleDeleteDir( LPCTSTR pszSrc );


//  ����·������Ϊwindows�¸�ʽ���磺C��\\file\\abc.txt
// ��ȡ�ļ�·���µ��ļ�����
CString GetFileName( LPCTSTR lpszFullFileName );

// ��ȡ�ļ�·���µ��ļ�·��
CString GetFilePath( LPCTSTR lpszFullFileName );

// ��ȡָ��ģ���µ�·��
CString GetModulePath( HMODULE hModule = NULL );


//  FileFind��װ
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
