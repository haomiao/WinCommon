///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   virtual_memory.h
/// 	@brief	虚拟内存. 
/// 
///		主要实现基本的虚拟内存操作、封装，并以此作为底层内存分配容器  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/12  17:46:53
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _VIRTUAL_MEMORY_H_
#define     _VIRTUAL_MEMORY_H_

#include <windows.h>

//  虚拟内存
//  可根据需要调整dwSize大小,或者更复杂的结构dwSize = sizeof(T) * n；或者
//  一个比较大的数据,一定要注意页对齐问题;
//  此外，带默认参数的尽可能使用默认参数即可，否则可能使用不当会导致失败
class CVirtualMemory
{
public:

    /**
    *  @brief  虚拟内存申请，预分配
    *  @param  [in] 	lpAddress           起始地址(一般申请预留为NULL)
    *  @param  [in] 	dwSize              申请大小(向下页对齐)，字节大小
    *  @param  [in] 	flAllocationType    分配类型
    *  @param  [in] 	flProtect           分配页保护属性
    */
    CVirtualMemory( SIZE_T dwSize,  LPVOID lpAddress = NULL, 
        DWORD flAllocationType = MEM_RESERVE, DWORD flProtect = PAGE_READWRITE );

    ~CVirtualMemory();

public:

    /**
    *  @brief  虚拟内存地址是否有效
    */
    BOOL IsValid();

    /**
    *  @brief  虚拟内存申请,提交分配
    *  @param  [in] 	lpAddress           起始地址(为基地址+需要的偏移)(向下页对齐)
    *  @param  [in] 	dwSize              申请大小(向下页对齐)，字节大小
    *  @param  [in] 	flAllocationType    分配类型
    *  @param  [in] 	flProtect           分配页保护属性
    */
    LPVOID Alloc( LPVOID lpAddress, SIZE_T dwSize, 
        DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_READWRITE );

    /**
    *  @brief  虚拟内存重申请,提交分配
    *  @param  [in] 	lpAddress           起始地址(为基地址+需要的偏移)(向下页对齐)
    *  @param  [in] 	dwSize              申请大小(向下页对齐)，字节大小
    *  @param  [in] 	flAllocationType    分配类型
    *  @param  [in] 	flProtect           分配页保护属性
    *  @detail: (一般情况下对于扩大区域,也可以在外部调用Alloc; 减少区域,释放指定区域大小
                以达到类型效果
    */
    LPVOID ReAlloc( LPVOID lpAddress, SIZE_T dwSize,
        DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_READWRITE );

    /**
    *  @brief  虚拟内存申请,提交分配
    *  @param  [in] 	lpAddress           起始地址(为基地址+需要的偏移)
    *  @param  [in] 	dwSize              申请大小(向下页对齐)，字节大小
    *  @param  [in] 	dwFreeType          释放类型
    */
    BOOL Free( LPVOID lpAddress, SIZE_T dwSize,
        DWORD dwFreeType = MEM_DECOMMIT);

    /**
    *  @brief  获取虚拟内存基地址
    */
    LPVOID GetBaseAddress() const;

    /**
    *  @brief  获取系统分页大小
    */
    DWORD GetPageSize() const;

    /**
    *  @brief  获取虚拟地址预留大小
    */
    SIZE_T Size() const;

private:

    CVirtualMemory( const CVirtualMemory& );
    CVirtualMemory & operator = ( const CVirtualMemory & );

private:

    LPVOID      m_pBaseAddress;         //    虚拟内存基地址
    SIZE_T      m_size;                 //    获取虚拟内存大小(预留的)
};


//  虚拟内存缓冲区模板，从虚拟内存中申请获取,简易实现
//  为了便于使用，该模板将对申请、释放虚拟内存的标识统一处理
//  限制：最后一次申请，前期申请的可能将会被释放掉；
//  若想多次申请且不被释放,请直接使用CVirtualMemory来进行操作
template <class T, DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_READWRITE >
class CVirtualMemoryBuffer
{
public:

    CVirtualMemoryBuffer( CVirtualMemory &virtualMemory) :
        m_virtualMemory(virtualMemory), m_pBuffer(NULL), m_size(0)
    {

    }

    ~CVirtualMemoryBuffer()
    {
        Free( MEM_RELEASE );
    }

public:

    T * Alloc( SIZE_T size )
    {
        Free( MEM_DECOMMIT );

        if ( size * sizeof( T ) > m_virtualMemory.Size() )
        {
            return NULL;
        }

        if ( m_virtualMemory.IsValid() == TRUE )
        {
            m_pBuffer = (T *)m_virtualMemory.Alloc( m_virtualMemory.GetBaseAddress(), size * sizeof( T ), flAllocationType, flProtect );
            if ( m_pBuffer != NULL )
            {
                m_size = size;
            }
        }
        return m_pBuffer;
    }

    T * ReAlloc( SIZE_T size, DWORD dwAllocFlags = 0 )
    {
        if ( m_virtualMemory.IsValid() == TRUE && m_pBuffer != NULL)
        {
            m_pBuffer = (T *)m_virtualMemory.ReAlloc( m_pBuffer, size * sizeof( T ), flAllocationType, flProtect );
            if ( m_pBuffer != NULL )
            {
                m_size = size;
            }
        }
        return m_pBuffer;
    }

    SIZE_T Size( DWORD dwFlags = 0 )
    {
        if ( m_pBuffer == NULL || m_virtualMemory.IsValid() == FALSE )
        {
            return SIZE_T( -1 );
        }
        return m_size;
    }

    BOOL Free( DWORD dwFreeType = MEM_DECOMMIT )
    {
        BOOL res = FALSE;
        if ( m_virtualMemory.IsValid() == TRUE )
        {
            if ( dwFreeType == MEM_DECOMMIT && m_pBuffer != NULL)
            {
                res = m_virtualMemory.Free( m_pBuffer, m_size * sizeof(T), dwFreeType );
                m_pBuffer = NULL;
            }
            else if (dwFreeType == MEM_RELEASE)
            {
                res = m_virtualMemory.Free( m_virtualMemory.GetBaseAddress(), 0, dwFreeType );
                m_pBuffer = NULL;
            }
        }
        return res;
    }

    operator T* ( )	const
    {
        return m_pBuffer;
    }

private:

    T               * m_pBuffer;                //  申请到的缓冲区地址
    CVirtualMemory    &m_virtualMemory;         //  虚拟内存引用
    SIZE_T            m_size;                   //  申请的缓冲区大小
};

typedef CVirtualMemoryBuffer<char, MEM_COMMIT, PAGE_READWRITE>      VirtualMemoryBufferStringA;
typedef CVirtualMemoryBuffer<wchar_t, MEM_COMMIT, PAGE_READWRITE>   VirtualMemoryBufferStringW;

#endif  //  _VIRTUAL_MEMORY_H_