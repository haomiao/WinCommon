///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   virtual_memory.h
/// 	@brief	�����ڴ�. 
/// 
///		��Ҫʵ�ֻ����������ڴ��������װ�����Դ���Ϊ�ײ��ڴ��������  
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

//  �����ڴ�
//  �ɸ�����Ҫ����dwSize��С,���߸����ӵĽṹdwSize = sizeof(T) * n������
//  һ���Ƚϴ������,һ��Ҫע��ҳ��������;
//  ���⣬��Ĭ�ϲ����ľ�����ʹ��Ĭ�ϲ������ɣ��������ʹ�ò����ᵼ��ʧ��
class CVirtualMemory
{
public:

    /**
    *  @brief  �����ڴ����룬Ԥ����
    *  @param  [in] 	lpAddress           ��ʼ��ַ(һ������Ԥ��ΪNULL)
    *  @param  [in] 	dwSize              �����С(����ҳ����)���ֽڴ�С
    *  @param  [in] 	flAllocationType    ��������
    *  @param  [in] 	flProtect           ����ҳ��������
    */
    CVirtualMemory( SIZE_T dwSize,  LPVOID lpAddress = NULL, 
        DWORD flAllocationType = MEM_RESERVE, DWORD flProtect = PAGE_READWRITE );

    ~CVirtualMemory();

public:

    /**
    *  @brief  �����ڴ��ַ�Ƿ���Ч
    */
    BOOL IsValid();

    /**
    *  @brief  �����ڴ�����,�ύ����
    *  @param  [in] 	lpAddress           ��ʼ��ַ(Ϊ����ַ+��Ҫ��ƫ��)(����ҳ����)
    *  @param  [in] 	dwSize              �����С(����ҳ����)���ֽڴ�С
    *  @param  [in] 	flAllocationType    ��������
    *  @param  [in] 	flProtect           ����ҳ��������
    */
    LPVOID Alloc( LPVOID lpAddress, SIZE_T dwSize, 
        DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_READWRITE );

    /**
    *  @brief  �����ڴ�������,�ύ����
    *  @param  [in] 	lpAddress           ��ʼ��ַ(Ϊ����ַ+��Ҫ��ƫ��)(����ҳ����)
    *  @param  [in] 	dwSize              �����С(����ҳ����)���ֽڴ�С
    *  @param  [in] 	flAllocationType    ��������
    *  @param  [in] 	flProtect           ����ҳ��������
    *  @detail: (һ������¶�����������,Ҳ�������ⲿ����Alloc; ��������,�ͷ�ָ�������С
                �Դﵽ����Ч��
    */
    LPVOID ReAlloc( LPVOID lpAddress, SIZE_T dwSize,
        DWORD flAllocationType = MEM_COMMIT, DWORD flProtect = PAGE_READWRITE );

    /**
    *  @brief  �����ڴ�����,�ύ����
    *  @param  [in] 	lpAddress           ��ʼ��ַ(Ϊ����ַ+��Ҫ��ƫ��)
    *  @param  [in] 	dwSize              �����С(����ҳ����)���ֽڴ�С
    *  @param  [in] 	dwFreeType          �ͷ�����
    */
    BOOL Free( LPVOID lpAddress, SIZE_T dwSize,
        DWORD dwFreeType = MEM_DECOMMIT);

    /**
    *  @brief  ��ȡ�����ڴ����ַ
    */
    LPVOID GetBaseAddress() const;

    /**
    *  @brief  ��ȡϵͳ��ҳ��С
    */
    DWORD GetPageSize() const;

    /**
    *  @brief  ��ȡ�����ַԤ����С
    */
    SIZE_T Size() const;

private:

    CVirtualMemory( const CVirtualMemory& );
    CVirtualMemory & operator = ( const CVirtualMemory & );

private:

    LPVOID      m_pBaseAddress;         //    �����ڴ����ַ
    SIZE_T      m_size;                 //    ��ȡ�����ڴ��С(Ԥ����)
};


//  �����ڴ滺����ģ�壬�������ڴ��������ȡ,����ʵ��
//  Ϊ�˱���ʹ�ã���ģ�彫�����롢�ͷ������ڴ�ı�ʶͳһ����
//  ���ƣ����һ�����룬ǰ������Ŀ��ܽ��ᱻ�ͷŵ���
//  �����������Ҳ����ͷ�,��ֱ��ʹ��CVirtualMemory�����в���
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

    T               * m_pBuffer;                //  ���뵽�Ļ�������ַ
    CVirtualMemory    &m_virtualMemory;         //  �����ڴ�����
    SIZE_T            m_size;                   //  ����Ļ�������С
};

typedef CVirtualMemoryBuffer<char, MEM_COMMIT, PAGE_READWRITE>      VirtualMemoryBufferStringA;
typedef CVirtualMemoryBuffer<wchar_t, MEM_COMMIT, PAGE_READWRITE>   VirtualMemoryBufferStringW;

#endif  //  _VIRTUAL_MEMORY_H_