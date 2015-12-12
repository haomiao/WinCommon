///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   process_heap.h
/// 	@brief	进程堆栈(进程私有堆). 
/// 
///		主要实现简单的进程堆栈，可减少内存申请、释放导致内存泄露  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/27  15:10:34
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _PROCESS_HEAP_H_
#define     _PROCESS_HEAP_H_

#include <windows.h>

class CProcessHeap
{
public:

    /**
    *  @brief  进程辅助堆
    *  @param  [in] 	flOptions       堆分配选项(主要控制访问权限、影响部分堆函数调用)
    *  @param  [in] 	dwInitialSize   初始化提交页面，字节大小
    *  @param  [in] 	dwMaximumSize   最大提交页面，字节大小
    */
    CProcessHeap( DWORD flOptions = 0, SIZE_T dwInitialSize = 0, SIZE_T dwMaximumSize  = 0);

    ~CProcessHeap();

public:

    /**
    *  @brief  进程辅助堆是否有效
    */
    BOOL IsValid();
    
    /**
    *  @brief  申请堆空间
    *  @param  [in] 	dwFlags         堆分配标识(参数同构造函数中的flOptions)
    *  @return          返回申请得到的缓冲区地址
    */
    LPVOID AllocHeap( DWORD dwFlags, SIZE_T dwBytes );

    /**
    *  @brief  申请堆空间
    *  @param  [in] 	dwFlags         堆分配标识(参数同构造函数中的flOptions)
    *  @param  [in] 	lpMem           通过AllocHeap或ReAllocHeap得到的缓冲区地址
    *  @param  [in] 	dwBytes         请求申请的堆大小
    *  @return          返回重新申请得到的缓冲区地址
    */
    LPVOID ReAllocHeap( DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes );

    /**
    *  @brief  查询获取到的堆空间大小
    *  @param  [in] 	dwFlags         堆分配标识(参数同构造函数中的flOptions)
    *  @param  [in] 	lpMem           通过AllocHeap或ReAllocHeap得到的缓冲区地址
    *  @return          返回申请得到的缓冲区地址下得到的实际分配的大小
    */
    SIZE_T GetAllocedSize( DWORD dwFlags, LPCVOID lpMem );

    /**
    *  @brief  释放获取到的堆空间地址下资源
    *  @param  [in] 	dwFlags         堆分配标识(参数同构造函数中的flOptions)
    *  @param  [in] 	lpMem           通过AllocHeap或ReAllocHeap得到的缓冲区地址
    *  @return          返回释放成功与否状态
    */
    BOOL FreeHeap( DWORD dwFlags, LPVOID lpMem );

    /**
    *  @brief  使堆空间紧密
    *  @param  [in] 	dwFlags         堆分配标识(参数同构造函数中的flOptions)
    *  @return          返回已提交的堆块空间大小
    */
    SIZE_T CompactHeap( DWORD dwFlags );

    /**
    *  @brief  重置进程辅助堆
    *  @param  [in] 	flOptions       堆分配选项(主要控制访问权限、影响部分堆函数调用)
    *  @param  [in] 	dwInitialSize   初始化提交页面，字节大小
    *  @param  [in] 	dwMaximumSize   最大提交页面，字节大小
    */
    BOOL ReSetHeap( DWORD flOptions = 0, SIZE_T dwInitialSize = 0, SIZE_T dwMaximumSize = 0 );

private:

    CProcessHeap( const CProcessHeap& );
    CProcessHeap & operator = ( const CProcessHeap & );

private:

    HANDLE      m_heapHandle;       //  进程辅助堆句柄
};


//  堆缓冲区模板，从堆对象中申请获取
//  为了便于使用，该模板将对申请、释放堆的标识统一处理
template <class T, DWORD dwAllocFlags = 0, DWORD dwFreeFlags = 0>
class HeapBuffer
{
public:

    HeapBuffer( CProcessHeap &processHeap, SIZE_T size = 32 ) :
        m_processHeap(processHeap), m_pBuffer( NULL )
    {
        Alloc( size );
    }

    ~HeapBuffer()
    {
        Free( );
    }

public:

    T * Alloc( SIZE_T size )
    {
        Free();
        if (m_processHeap.IsValid() == TRUE)
        {
            m_pBuffer = (T *)m_processHeap.AllocHeap( dwAllocFlags, size * sizeof( T ) );
        }

        return m_pBuffer;
    }

    T * ReAlloc( SIZE_T size, DWORD dwAllocFlags = 0)
    {
        if (m_processHeap.IsValid() == TRUE)
        {
            m_pBuffer = (T *)m_processHeap.ReAllocHeap( dwAllocFlags, m_pBuffer, size * sizeof(T) );
        }

        return m_pBuffer;
    }

    SIZE_T Size( DWORD dwFlags = 0 )
    {
        if ( m_pBuffer == NULL || m_processHeap.IsValid() == FALSE )
        {
            return SIZE_T(-1);
        }

        return m_processHeap.GetAllocedSize(dwFlags , m_pBuffer ) / sizeof(T);
    }

    BOOL Free()
    { 
        BOOL res = FALSE;
        if (m_processHeap.IsValid() == TRUE && m_pBuffer != NULL)
        {
            res = m_processHeap.FreeHeap( dwFreeFlags, m_pBuffer );
            m_pBuffer = NULL;
        }
        return res;
    }

    operator T* ( )	const	
    { 
        return m_pBuffer; 
    }

private:

    T               * m_pBuffer;            //  申请到的缓冲区地址
    CProcessHeap    &m_processHeap;         //  辅助堆引用
};

typedef HeapBuffer<char, 0, 0>      HeapBufferStringA;
typedef HeapBuffer<wchar_t, 0, 0>   HeapBufferStringW;

#endif  //  _PROCESS_HEAP_H_