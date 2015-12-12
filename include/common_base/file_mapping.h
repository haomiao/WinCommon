///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   file_mapping.h
/// 	@brief	文件映射、共享内存. 
/// 
///		主要实现基本的文件映射操作、以及共享内存  
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
      *  @brief  CFileMapping构造函数 
      *  @param  [in] 	lpName                  映射文件命名
      *  @param  [in] 	dwMaximumSizeLow        映射文件最大大小低位大小
      *  @param  [in] 	hFile                   映射文件句柄(为CreateFile文件句柄或INVALID_HANDLE_VALUE)
      *  @param  [in] 	lpAttributes            安全属性
      *  @param  [in] 	flProtect               映射文件受保护属性(注意一定不要与CreateFile访问属性冲突)
      *  @param  [in] 	dwMaximumSizeHigh       映射文件最大大小高位大小
      *  detail: 若hFile为INVALID_HANDLE_VALUE，则dwMaximumSizeLow可从GetLargePageMinimum获取分页最小大小
      */
    CFileMapping( LPCTSTR lpName = NULL,
        DWORD dwMaximumSizeLow = ::GetLargePageMinimum(),
        HANDLE hFile = INVALID_HANDLE_VALUE, 
        LPSECURITY_ATTRIBUTES lpAttributes = NULL, 
        DWORD flProtect = PAGE_READWRITE,
        DWORD dwMaximumSizeHigh = 0);

    ~CFileMapping();

public:

    //  当前映射是否有效
    BOOL IsValid();

    /**
    *  @brief  打开已存在的命名映射文件
    *  @param  [in] 	lpName                  已打开的映射文件名称
    *  @param  [in] 	dwDesiredAccess         访问权限
    *  @param  [in] 	bInheritHandle          是否继承当前句柄
    */
    BOOL OpenFileMapping(LPCTSTR lpName, DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS, 
        BOOL bInheritHandle = FALSE);
    
    /**
    *  @brief  映射当前的"文件"至进程地址空间
    *  @param  [in] 	dwFileOffsetLow         文件首位置偏移低位大小
    *  @param  [in] 	dwNumberOfBytesToMap    映射文件字节数大小
    *  @param  [in] 	dwFileOffsetHigh        文件首位置偏移高位大小
    *  @param  [in] 	dwDesiredAccess         访问权限
    *  return           返回映射后地址空间
    */
    LPVOID MapViewOfFile( DWORD dwFileOffsetLow = 0, 
        SIZE_T dwNumberOfBytesToMap = 0 ,
        DWORD dwFileOffsetHigh = 0, 
        DWORD dwDesiredAccess = FILE_MAP_ALL_ACCESS);

    /**
    *  @brief  撤销映射当前的"文件"数据映像
    *  @param  [in] 	lpBaseAddress         通过MapViewOfFile返回的映射文件地址
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

    HANDLE          m_mapFileHandle;        //  映射文件句柄
    ULONGLONG       m_mapFileSize;          //  映射文件大小
};


//  共享内存(通过文件映射的方式实现共享内存)
//  读、写共享内存数据
class CRWSharedMemory
{
public:

    /**
    *  @brief  创建共享内存
    *  @param  [in] 	lpName                  映射文件名称
    *  @param  [in] 	dwMaximumSizeLow        共享内存最大大小低位
    *  @param  [in] 	dwMaximumSizeLow        共享内存最大大小高位
    */
    CRWSharedMemory( LPCTSTR lpName, DWORD dwMaximumSizeLow = ::GetLargePageMinimum(), DWORD dwMaximumSizeHigh = 0 );

    ~CRWSharedMemory();

public:

    /**
    *  @brief  读取映射进程地址空间数据
    *  @param  [out] 	pBuffer                 读取映射文件数据缓冲区
    *  @param  [in] 	bufferBytes             读取映射文件数据缓冲区大小
    *  @param  [in] 	OffsetLow               首位置偏移低位大小
    *  @param  [in] 	bytes                   读取映射文件字节数大小
    *  @param  [in] 	OffsetHigh              首位置偏移高位大小
    */
    BOOL ReadMemory( LPVOID pBuffer, SIZE_T bufferBytes, DWORD OffsetLow, SIZE_T bytes, DWORD OffsetHigh = 0 );

    /**
    *  @brief  写入数据至映射进程地址空间
    *  @param  [out] 	OffsetLow               首位置偏移低位大小
    *  @param  [in] 	pBuffer                 写入映射文件数据缓冲区
    *  @param  [in] 	bufferBytes             写入映射文件数据缓冲区数据内容大小(以字节为单位)
    *  @param  [in] 	OffsetHigh              首位置偏移高位大小
    */
    BOOL WriteMemory( LPCVOID pBuffer, SIZE_T bufferBytes, DWORD OffsetLow, DWORD OffsetHigh = 0 );

private:

    CRWSharedMemory( const CRWSharedMemory& );
    CRWSharedMemory& operator = ( const CRWSharedMemory & );

private:

    CFileMapping    m_fileMapping;              //  文件映像对象
};


//  事实上，也可以通过文件映射实现共享内存或者内存分配之类的容器(根据需要从中分配(映射)当作缓冲区使用 )
//  若当作内存分配器，则需要封装，申请、释放、记录碎片、合并碎片、查找合适的空闲区域等操作
#endif  //  _FILE_MAPPING_H_