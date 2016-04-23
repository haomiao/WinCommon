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
    //  策略：按照指定起始地址，需要查询每个分页是否已被提交，若已被提交则忽略，若未提交则提交；
    //  当缩小区域的时候，暂时不减少提交的为预留状态;
    //  若一定要减少不必要提交的区域，则建议外部维护状态调用Free来取消提交
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
                //  对于后面的页申请若失败，之前申请成功的，暂不撤销
                //  若用户需要撤销，可调用Free手动释放
                return NULL;
            }
        }
        lpTempAddress = (LPVOID)( (PBYTE)lpTempAddress + GetPageSize() );
    }

    //  可以直接返回初始基地址,重分配过程不会改变基地址
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