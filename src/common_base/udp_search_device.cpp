
#include "stdafx.h"

#include "udp_search_device.h"

#pragma comment(lib, "Iphlpapi.lib")

//////////////////////////////////////////////////////////////////////////
    //  早期采用非绑定方式，广播至网卡IP所在的直接广播地址，但依赖于子网掩码，
    //  若子网掩码有误，将导致无法搜索或广播到设备,
    //  后期采用绑定方式，广播至广播地址，将不再依赖于子网掩码,搜索更有效!
//////////////////////////////////////////////////////////////////////////

namespace
{
	/**
	* \brief 设置发送失败超时重发(ms)
	*/
    #define  SEND_TIME_OUT      200

	/**
	* \brief 关闭套接字
	*/
	#define CLOSESOCK(socket)\
	{\
        ::shutdown(socket, SD_BOTH);\
		::closesocket(socket);\
		socket = INVALID_SOCKET;\
	}\


	ULONG  CalDirAddress(ULONG ip, ULONG mask)
	{
		ULONG newIP = ip & mask | (0xffffffff ^ mask);
//		newIP = ip | (0xff000000);
		return newIP;
	}

}


CUDPBroadCast::CUDPBroadCast():
    m_searchDevNum(0),
    m_searchEventHandle(NULL),
    m_pAdatpterInfo(NULL),
    m_isNetComValid( true ),
    m_adatpterAddress(""),
    m_port(MAXINT16),
    m_searchCallBack(nullptr)
{
	WSADATA wsaData;
    if ( ::WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) != 0 )
	{
        m_isNetComValid = false;
	}
}


CUDPBroadCast::~CUDPBroadCast()
{
    m_dirAddressSet.clear();
    m_devMap.clear();

	if (m_pAdatpterInfo)
	{
		delete []m_pAdatpterInfo;
		m_pAdatpterInfo = NULL;
	}

    if (m_searchEventHandle)
    {
        ::CloseHandle( m_searchEventHandle );
        m_searchEventHandle = NULL;
    }

    ::WSACleanup();
}


bool CUDPBroadCast::SetAdatpterIPAddress( const char *ipAddress, unsigned short port )
{
    if ( !m_isNetComValid )
    {
        return false;
    }

    if (ipAddress)
    {
        m_adatpterAddress = ipAddress;
    }
    m_port = port;
    return true;
}


bool CUDPBroadCast::IsNetValid()
{
    return m_isNetComValid;
}


bool CUDPBroadCast::BroadCast( const char *broadCastInfo, unsigned int searchTimeout, bool asyncSearch, SearchCallBack searchCallBack )
{
    if ( broadCastInfo == nullptr || m_isNetComValid == false)
    {
        return false;
    }

    m_broadCastInfo  = broadCastInfo;
    m_searchTimeout  = searchTimeout;
    m_searchCallBack = searchCallBack;

    if ( m_searchEventHandle == NULL )
    {
        m_searchEventHandle = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    }

    if ( m_searchEventHandle == NULL )
    {
        return false;
    }

    ::ResetEvent( m_searchEventHandle );

    if ( asyncSearch == true )
    {
        HANDLE searchThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &SearchDevThread, this, CREATE_SUSPENDED, NULL );
        if ( searchThreadHandle == NULL )
        {
            return false;
        }

        ::ResumeThread( searchThreadHandle );
        ::CloseHandle( searchThreadHandle );
        searchThreadHandle = NULL;
        return true;
    }
    else
    {
        Scanning();
        return true;
    }
}


unsigned CUDPBroadCast::SearchDevThread( void *pParam )
{
    CUDPBroadCast * pSearchDev = (CUDPBroadCast *)pParam;
    if ( pSearchDev == NULL )
    {
        return 0;
    }

    pSearchDev->Scanning();
    return 0;
}


void CUDPBroadCast::Scanning()
{
    int nSelRet = 0;
    int nRecvRet = 0;

    #define MAX_RECIVE_STR  1024
    char szBuffer[MAX_RECIVE_STR] = { 0 };

    sockaddr_in addrBroadcast;
    addrBroadcast.sin_family = AF_INET;
    addrBroadcast.sin_addr.s_addr = INADDR_BROADCAST;

    sockaddr_in bindAddr;
    bindAddr.sin_family = AF_INET;

    // 	pSearchDev->m_socketClient = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    // 	if (pSearchDev->m_socketClient == INVALID_SOCKET)
    // 	{
    // 		return 0;
    // 	}

    // 	BOOL bOptVal = TRUE;
    // 	if(::setsockopt(pSearchDev->m_socketClient, SOL_SOCKET, SO_BROADCAST, 
    // 					(char FAR *)&bOptVal, sizeof(bOptVal)) == SOCKET_ERROR)
    // 	{
    // 		CLOSESOCK(pSearchDev->m_socketClient);
    // 		return 0;
    // 	}

    int nAddrLen = sizeof( SOCKADDR );
    m_dirAddressSet.clear();
    m_devMap.clear();

    if ( !m_adatpterAddress.empty() )
    {
        m_dirAddressSet.insert( inet_addr( m_adatpterAddress.c_str() ) );
    }
    else
    {
        GetHostAdapterInfo();
    }

    std::set<ULONG>::iterator Iter = m_dirAddressSet.begin();
    std::set<ULONG>::iterator endIter = m_dirAddressSet.end();

    if ( WAIT_OBJECT_0 == ::WaitForSingleObject( m_searchEventHandle, 0 ) )
    {
        goto Label;
    }

    Iter = m_dirAddressSet.begin();
    SOCKET socketClient( INVALID_SOCKET );
    for ( ; Iter != endIter; Iter++ )
    {
        socketClient = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
        if ( socketClient == INVALID_SOCKET )
        {
            goto Label;
        }

        BOOL bOptVal = TRUE;
        if ( ::setsockopt( socketClient, SOL_SOCKET, SO_BROADCAST,
            ( char FAR * )&bOptVal, sizeof( bOptVal ) ) == SOCKET_ERROR )
        {
            goto Label;
        }

        BOOL bReuse = TRUE;
        if ( SOCKET_ERROR == ::setsockopt( socketClient, SOL_SOCKET, SO_REUSEADDR, (CHAR*)&bReuse, sizeof( BOOL ) ) )
        {
            goto Label;
        }

        DWORD dwBytes = 0;
        BOOL bNewBehavior = FALSE;
        if ( SOCKET_ERROR == ::WSAIoctl( socketClient, SIO_UDP_CONNRESET,
            (LPVOID)&bNewBehavior, sizeof( bNewBehavior ),
            NULL, 0, &dwBytes, NULL, NULL ) )
        {
            goto Label;
        }

        bindAddr.sin_addr.s_addr = ( *Iter );
        bindAddr.sin_port = ::htons( m_port );

        if ( SOCKET_ERROR == ::bind( socketClient, ( const struct sockaddr FAR * )&bindAddr, sizeof( bindAddr ) ) )
        {
            goto Label;
        }

        if ( WAIT_OBJECT_0 == ::WaitForSingleObject( m_searchEventHandle, 0 ) )
        {
            goto Label;
        }

        //addrBroadcast.sin_addr.s_addr = (*Iter);///*0xffffffff;
        addrBroadcast.sin_port = ::htons( m_port );

        //  尝试至多3次测试
        #define MAX_COUNTS  3
        unsigned int indexCount = 0;
        for ( ; indexCount < MAX_COUNTS; ++indexCount )
        {
            if ( ::sendto( socketClient, m_broadCastInfo.c_str(), m_broadCastInfo.length(), 0,
                ( struct sockaddr FAR * )&addrBroadcast, nAddrLen ) != SOCKET_ERROR )
            {
                break;
            }
            ::Sleep( SEND_TIME_OUT );
        }

        if ( indexCount == MAX_COUNTS )
        {
            continue;
        }

        TIMEVAL tmVal;
        tmVal.tv_sec = 0;
        tmVal.tv_usec = m_searchTimeout;

        FD_SET fdRead;
        FD_ZERO( &fdRead );
        FD_SET( socketClient, &fdRead );

        sockaddr_in addrRecv;
        std::string devIP;

        while ( WAIT_OBJECT_0 != ::WaitForSingleObject( m_searchEventHandle, 0 ) )
        {
            memset( szBuffer , 0, sizeof(szBuffer) );

            /*从广播地址接收消息，所以第五个参数为发送广播的sender*/
            nSelRet = ::select( 0, &fdRead, NULL, NULL, &tmVal );
            if ( nSelRet <= 0 )
            {
                break;
            }

            nRecvRet = ::recvfrom( socketClient, szBuffer, sizeof( szBuffer ), 0,
                ( struct sockaddr FAR * )&addrRecv, ( int FAR * )&nAddrLen );
            if ( nRecvRet <= 0 )
            {
                break;
            }

            /*查询设备容器存在设备IP*/
            devIP = inet_ntoa( addrRecv.sin_addr );
            if ( m_devMap.find( devIP ) == m_devMap.end() )
            {
                ++( m_searchDevNum );
                m_devMap[devIP] = szBuffer;
            }

            //  回调
            if ( m_searchCallBack != nullptr)
            {
                m_searchCallBack(devIP, szBuffer);
            }
        }

        CLOSESOCK( socketClient );
    }

Label:

    CLOSESOCK( socketClient );
    ::SetEvent( m_searchEventHandle );
}


void CUDPBroadCast::GetHostAdapterInfo()
{
	if (m_pAdatpterInfo == NULL)
	{
		m_pAdatpterInfo = new IP_ADAPTER_INFO();
	}

	if (m_pAdatpterInfo == NULL)
	{
		return;
	}

    BYTE * tempBuffer = NULL;

	ULONG bufLen    = sizeof(IP_ADAPTER_INFO);
	DWORD errCode   = GetAdaptersInfo(m_pAdatpterInfo, &bufLen);
	if (errCode == ERROR_BUFFER_OVERFLOW)
	{
		delete m_pAdatpterInfo;
        tempBuffer = new BYTE[bufLen];
        //m_pAdatpterInfo = reinterpret_cast<IP_ADAPTER_INFO *>(tempBuffer);
        m_pAdatpterInfo = new (tempBuffer) IP_ADAPTER_INFO;

		//m_pAdatpterInfo = (PIP_ADAPTER_INFO)new BYTE[bufLen];
		errCode = GetAdaptersInfo(m_pAdatpterInfo, &bufLen);
	}

	if (errCode == NO_ERROR)
	{
		ULONG ipAddress		= 0;
		ULONG mask			= 0;
		ULONG dirAddress	= 0;
		while (m_pAdatpterInfo)
		{
			PIP_ADDR_STRING ipAddressList = &(m_pAdatpterInfo->IpAddressList);
			while (ipAddressList)
			{
				ipAddress		= inet_addr(ipAddressList->IpAddress.String);
				mask			= inet_addr(ipAddressList->IpMask.String);
				//dirAddress		= CalDirAddress(ipAddress, mask);
				ipAddressList	= ipAddressList->Next;
				//m_dirAddress.insert(dirAddress);
                m_dirAddressSet.insert( ipAddress );
			}
			m_pAdatpterInfo = m_pAdatpterInfo->Next;
		}
	}

    if (tempBuffer)
    {
        m_pAdatpterInfo->~IP_ADAPTER_INFO();
        delete []tempBuffer;
        m_pAdatpterInfo = NULL;
    }
}


unsigned int CUDPBroadCast::GetDevMapInfo( DevMap &devMap, DWORD waitTimeout )
{
    if ( ( m_searchEventHandle == NULL ||
        WAIT_OBJECT_0 != ::WaitForSingleObject( m_searchEventHandle, waitTimeout ) ) )
    {
        return -1;
    }
    
    devMap = m_devMap;
    return m_devMap.size();
}