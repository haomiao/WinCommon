///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014   
///     All rights reserved.  
///  
/// 	@file   process_heap.h
/// 	@brief	���̶�ջ(����˽�ж�). 
/// 
///		��Ҫʵ�ּ򵥵Ľ��̶�ջ���ɼ����ڴ����롢�ͷŵ����ڴ�й¶  
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
    *  @brief  ���̸�����
    *  @param  [in] 	flOptions       �ѷ���ѡ��(��Ҫ���Ʒ���Ȩ�ޡ�Ӱ�첿�ֶѺ�������)
    *  @param  [in] 	dwInitialSize   ��ʼ���ύҳ�棬�ֽڴ�С
    *  @param  [in] 	dwMaximumSize   ����ύҳ�棬�ֽڴ�С
    */
    CProcessHeap( DWORD flOptions = 0, SIZE_T dwInitialSize = 0, SIZE_T dwMaximumSize  = 0);

    ~CProcessHeap();

public:

    /**
    *  @brief  ���̸������Ƿ���Ч
    */
    BOOL IsValid();
    
    /**
    *  @brief  ����ѿռ�
    *  @param  [in] 	dwFlags         �ѷ����ʶ(����ͬ���캯���е�flOptions)
    *  @return          ��������õ��Ļ�������ַ
    */
    LPVOID AllocHeap( DWORD dwFlags, SIZE_T dwBytes );

    /**
    *  @brief  ����ѿռ�
    *  @param  [in] 	dwFlags         �ѷ����ʶ(����ͬ���캯���е�flOptions)
    *  @param  [in] 	lpMem           ͨ��AllocHeap��ReAllocHeap�õ��Ļ�������ַ
    *  @param  [in] 	dwBytes         ��������ĶѴ�С
    *  @return          ������������õ��Ļ�������ַ
    */
    LPVOID ReAllocHeap( DWORD dwFlags, LPVOID lpMem, SIZE_T dwBytes );

    /**
    *  @brief  ��ѯ��ȡ���Ķѿռ��С
    *  @param  [in] 	dwFlags         �ѷ����ʶ(����ͬ���캯���е�flOptions)
    *  @param  [in] 	lpMem           ͨ��AllocHeap��ReAllocHeap�õ��Ļ�������ַ
    *  @return          ��������õ��Ļ�������ַ�µõ���ʵ�ʷ���Ĵ�С
    */
    SIZE_T GetAllocedSize( DWORD dwFlags, LPCVOID lpMem );

    /**
    *  @brief  �ͷŻ�ȡ���Ķѿռ��ַ����Դ
    *  @param  [in] 	dwFlags         �ѷ����ʶ(����ͬ���캯���е�flOptions)
    *  @param  [in] 	lpMem           ͨ��AllocHeap��ReAllocHeap�õ��Ļ�������ַ
    *  @return          �����ͷųɹ����״̬
    */
    BOOL FreeHeap( DWORD dwFlags, LPVOID lpMem );

    /**
    *  @brief  ʹ�ѿռ����
    *  @param  [in] 	dwFlags         �ѷ����ʶ(����ͬ���캯���е�flOptions)
    *  @return          �������ύ�Ķѿ�ռ��С
    */
    SIZE_T CompactHeap( DWORD dwFlags );

    /**
    *  @brief  ���ý��̸�����
    *  @param  [in] 	flOptions       �ѷ���ѡ��(��Ҫ���Ʒ���Ȩ�ޡ�Ӱ�첿�ֶѺ�������)
    *  @param  [in] 	dwInitialSize   ��ʼ���ύҳ�棬�ֽڴ�С
    *  @param  [in] 	dwMaximumSize   ����ύҳ�棬�ֽڴ�С
    */
    BOOL ReSetHeap( DWORD flOptions = 0, SIZE_T dwInitialSize = 0, SIZE_T dwMaximumSize = 0 );

private:

    CProcessHeap( const CProcessHeap& );
    CProcessHeap & operator = ( const CProcessHeap & );

private:

    HANDLE      m_heapHandle;       //  ���̸����Ѿ��
};


//  �ѻ�����ģ�壬�ӶѶ����������ȡ
//  Ϊ�˱���ʹ�ã���ģ�彫�����롢�ͷŶѵı�ʶͳһ����
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

    T               * m_pBuffer;            //  ���뵽�Ļ�������ַ
    CProcessHeap    &m_processHeap;         //  ����������
};

typedef HeapBuffer<char, 0, 0>      HeapBufferStringA;
typedef HeapBuffer<wchar_t, 0, 0>   HeapBufferStringW;

#endif  //  _PROCESS_HEAP_H_