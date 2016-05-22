#include "stdafx.h"
#include "virtual_memory.h"


CVirtualMemory::CVirtualMemory( SIZE_T dwSize, LPVOID lpAddress,
    DWORD flAllocationType, DWORD flProtect ):
    m_size(-1)
{
    m_pBaseAddress = ::VirtualAlloc( lpAddress, dwSize, flAllocationType, flProtect );
    if (m_pBaseAddress != NULL)
    {
        m_size = dwSize;
    }
}


CVirtualMemory::~CVirtualMemory()
{
    if ( m_pBaseAddress != NULL)
    {
        ::VirtualFree( m_pBaseAddress, 0, MEM_RELEASE );
        m_pBaseAddress = NULL;
    }
}


BOOL CVirtualMemory::IsValid()
{
    return (m_pBaseAddress != NULL);
}


LPVOID CVirtualMemory::GetBaseAddress() const
{
    return m_pBaseAddress;
}


DWORD CVirtualMemory::GetPageSize() const
{
    SYSTEM_INFO systemInfo;
    GetSystemInfo( &systemInfo );
    return systemInfo.dwPageSize;
}


SIZE_T CVirtualMemory::Size() const
{
    return m_size;
}


LPVOID CVirtualMemory::Alloc( LPVOID lpAddress, SIZE_T dwSize,
    DWORD flAllocationType, DWORD flProtect )
{
    return ::VirtualAlloc( lpAddress, dwSize, flAllocationType, flProtect );
}


LPVOID CVirtualMemory::ReAlloc( LPVOID lpAddress, SIZE_T dwSize,
    DWORD flAllocationType, DWORD flProtect )
{
    //  ���ԣ�����ָ����ʼ��ַ����Ҫ��ѯÿ����ҳ�Ƿ��ѱ��ύ�����ѱ��ύ����ԣ���δ�ύ���ύ��
    //  ����С�����ʱ����ʱ�������ύ��ΪԤ��״̬;
    //  ��һ��Ҫ���ٲ���Ҫ�ύ�����������ⲿά��״̬����Free��ȡ���ύ
    unsigned int pageCounts = dwSize / GetPageSize();
    MEMORY_BASIC_INFORMATION mbi;
    LPVOID lpTempAddress = lpAddress;
    LPVOID lpAlloc = NULL;
    for ( unsigned int pageIndex = 0; pageIndex < pageCounts; ++pageIndex )
    {
        memset(&mbi, 0, sizeof(mbi) );
        ::VirtualQuery( lpTempAddress, &mbi, sizeof( mbi ) );
        if (mbi.State != MEM_COMMIT)
        {
            lpAlloc = Alloc( lpTempAddress, GetPageSize() );
            if (lpAlloc == NULL)
            {
                //  ���ں����ҳ������ʧ�ܣ�֮ǰ����ɹ��ģ��ݲ�����
                //  ���û���Ҫ�������ɵ���Free�ֶ��ͷ�
                return NULL;
            }
        }
        lpTempAddress = (LPVOID)( (PBYTE)lpTempAddress + GetPageSize() );
    }

    //  ����ֱ�ӷ��س�ʼ����ַ,�ط�����̲���ı����ַ
    return lpAddress;
}


BOOL CVirtualMemory::Free( LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType )
{
    if (dwFreeType == MEM_RELEASE && dwSize == 0 && lpAddress == m_pBaseAddress)
    {
        m_pBaseAddress = NULL;
    }
    return ::VirtualFree( lpAddress, dwSize, dwFreeType );
}