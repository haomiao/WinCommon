///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   file_mapping.h
/// 	@brief	�ļ�ӳ�䡢�����ڴ�. 
/// 
///		��Ҫʵ�ֻ������ļ�ӳ��������Լ������ڴ�  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/02  14:26:02
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _FILE_MAPPING_H_
#define     _FILE_MAPPING_H_

#include <windows.h>

class CFileMapping
{
public:

    /**
      *  @brief  CFileMapping���캯�� 
      *  @param  [in] 	lpName                  ӳ���ļ�����
      *  @param  [in] 	dwMaximumSizeLow        ӳ���ļ�����С��λ��С
      *  @param  [in] 	hFile                   ӳ���ļ����(ΪCreateFile�ļ������INVALID_HANDLE_VALUE)
      *  @param  [in] 	lpAttributes            ��ȫ����
      *  @param  [in] 	flProtect               ӳ���ļ��ܱ�������(ע��һ����Ҫ��CreateFile�������Գ�ͻ)
      *  @param  [in] 	dwMaximumSizeHigh       ӳ���ļ�����С��λ��С
      *  detail: ��hFileΪINVALID_HANDLE_VALUE����dwMaximumSizeLow�ɴ�GetLargePageMinimum��ȡ��ҳ��С��С
      */
    CFileMapping( LPCTSTR lpName = NULL,
        DWORD dwMaximumSizeLow = ::GetLargePageMinimum(),
        HANDLE hFile = INVALID_HANDLE_VALUE, 
        LPSECURITY_ATTRIBUTES lpAttributes = NULL, 
        DWORD flProtect = PAGE_READWRITE,
        DWORD dwMaximumSizeHigh = 0);

    ~CFileMapping();

public:

    //  ��ǰӳ���Ƿ���Ч
    BOOL IsValid();

    /**
    *  @brief  ���Ѵ��ڵ�����ӳ���ļ�
    *  @param  [in] 	lpName                  �Ѵ򿪵�ӳ���ļ�����
    *  @param  [in] 	dwDesiredAccess         ����Ȩ��
    *  @param  [in] 	bInheritHandle          �Ƿ�̳е�ǰ���
    */
    BOOL OpenFileMapping(LPCTSTR lpName, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS, 
        BOOL bInheritHandle = FALSE);
    
    /**
    *  @brief  ӳ�䵱ǰ��"�ļ�"�����̵�ַ�ռ�
    *  @param  [in] 	dwFileOffsetLow         �ļ���λ��ƫ�Ƶ�λ��С
    *  @param  [in] 	dwNumberOfBytesToMap    ӳ���ļ��ֽ�����С
    *  @param  [in] 	dwFileOffsetHigh        �ļ���λ��ƫ�Ƹ�λ��С
    *  @param  [in] 	dwDesiredAccess         ����Ȩ��
    *  return           ����ӳ����ַ�ռ�
    */
    LPVOID MapViewOfFile( DWORD dwFileOffsetLow = 0, 
        SIZE_T dwNumberOfBytesToMap = 0 ,
        DWORD dwFileOffsetHigh = 0, 
        DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);

    /**
    *  @brief  ����ӳ�䵱ǰ��"�ļ�"����ӳ��
    *  @param  [in] 	lpBaseAddress         ͨ��MapViewOfFile���ص�ӳ���ļ���ַ
    */
    BOOL UnMapViewOfFile(LPCVOID lpBaseAddress);

    inline HANDLE GetMapHandle()
    {
        return m_mapFileHandle;
    }

    inline operator HANDLE()
    { 
        return m_mapFileHandle; 
    }

    inline ULONGLONG GetMapFileSize()
    {
        return m_mapFileSize;
    }

private:

    CFileMapping( const CFileMapping& );
    CFileMapping& operator = ( const CFileMapping & );

private:

    HANDLE          m_mapFileHandle;        //  ӳ���ļ����
    ULONGLONG       m_mapFileSize;          //  ӳ���ļ���С
};


//  �����ڴ�(ͨ���ļ�ӳ��ķ�ʽʵ�ֹ����ڴ�)
//  ����д�����ڴ�����
class CRWSharedMemory
{
public:

    /**
    *  @brief  ���������ڴ�
    *  @param  [in] 	lpName                  ӳ���ļ�����
    *  @param  [in] 	dwMaximumSizeLow        �����ڴ�����С��λ
    *  @param  [in] 	dwMaximumSizeLow        �����ڴ�����С��λ
    */
    CRWSharedMemory( LPCTSTR lpName, DWORD dwMaximumSizeLow = ::GetLargePageMinimum(), DWORD dwMaximumSizeHigh = 0 );

    ~CRWSharedMemory();

public:

    /**
    *  @brief  ��ȡӳ����̵�ַ�ռ�����
    *  @param  [out] 	pBuffer                 ��ȡӳ���ļ����ݻ�����
    *  @param  [in] 	bufferBytes             ��ȡӳ���ļ����ݻ�������С
    *  @param  [in] 	OffsetLow               ��λ��ƫ�Ƶ�λ��С
    *  @param  [in] 	bytes                   ��ȡӳ���ļ��ֽ�����С
    *  @param  [in] 	OffsetHigh              ��λ��ƫ�Ƹ�λ��С
    */
    BOOL ReadMemory( LPVOID pBuffer, SIZE_T bufferBytes, DWORD OffsetLow, SIZE_T bytes, DWORD OffsetHigh = 0 );

    /**
    *  @brief  д��������ӳ����̵�ַ�ռ�
    *  @param  [out] 	OffsetLow               ��λ��ƫ�Ƶ�λ��С
    *  @param  [in] 	pBuffer                 д��ӳ���ļ����ݻ�����
    *  @param  [in] 	bufferBytes             д��ӳ���ļ����ݻ������������ݴ�С(���ֽ�Ϊ��λ)
    *  @param  [in] 	OffsetHigh              ��λ��ƫ�Ƹ�λ��С
    */
    BOOL WriteMemory( LPCVOID pBuffer, SIZE_T bufferBytes, DWORD OffsetLow, DWORD OffsetHigh = 0 );

private:

    CRWSharedMemory( const CRWSharedMemory& );
    CRWSharedMemory& operator = ( const CRWSharedMemory & );

private:

    CFileMapping    m_fileMapping;              //  �ļ�ӳ�����
};


//  ��ʵ�ϣ�Ҳ����ͨ���ļ�ӳ��ʵ�ֹ����ڴ�����ڴ����֮�������(������Ҫ���з���(ӳ��)����������ʹ�� )
//  �������ڴ������������Ҫ��װ�����롢�ͷš���¼��Ƭ���ϲ���Ƭ�����Һ��ʵĿ�������Ȳ���
#endif  //  _FILE_MAPPING_H_