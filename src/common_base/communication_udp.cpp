#include "stdafx.h"
#include <string>
#include "communication_udp.h"


namespace
{
	#define CLOSE_SOCK(socket)\
	{\
			::closesocket(socket);\
			socket = INVALID_SOCKET;\
	}

    #define FATAL_CLOSE_SOCK(socket)\
	{\
            ::shutdown(socket, SD_BOTH);\
			::closesocket(socket);\
			socket = INVALID_SOCKET;\
	}
}


UdpComm::UdpComm() :
m_socket(INVALID_SOCKET),
m_timeout(3),
m_timeWaitRecv(5000),
m_timeWaitSend(3000),
m_nodelay(1),
m_linger( { 1, 3} ),
m_keepAlive(0),
m_boardCast(false),
m_reuseAddr(true)
{
    //m_linger.l_onoff = 1;
    //m_linger.l_linger = 3;
}


UdpComm::~UdpComm()
{
    Close();
}


int UdpComm::Open( const void* arg )
{
    if ( !arg )
	{
		return COMM_PARAM_ERR;
	}

    EtherCommArg* comArg = reinterpret_cast<EtherCommArg*>(const_cast<void *>(arg) );
	if ( !comArg )
	{
		return COMM_PARAM_ERR;
	}
	
	if (m_socket != INVALID_SOCKET)
	{ 
        CLOSE_SOCK( m_socket );
	}

	m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_socket == INVALID_SOCKET)
	{	
        return ::WSAGetLastError();
	}

    SetDefaultCommParameters();
	
    sockaddr_in destSockAddr;
	destSockAddr.sin_addr.s_addr	= inet_addr(comArg->ip.c_str() );
	destSockAddr.sin_port			= htons(comArg->port);
	destSockAddr.sin_family			= AF_INET;
	
	int ret = ConnectSever(m_socket, (const struct sockaddr *)&destSockAddr);
	if (ret != COMM_SUCCESS)
	{
        return ::WSAGetLastError();
	}
    return COMM_SUCCESS;
}


int UdpComm::SetDefaultCommParameters()
{
    int res = ::setsockopt( m_socket, SOL_SOCKET, SO_RCVTIMEO, (const char *)&m_timeWaitRecv,
        sizeof( int ) );
    res = ::setsockopt( m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&m_timeWaitSend, 
        sizeof( int ) );
    res = ::setsockopt( m_socket, SOL_SOCKET, SO_BROADCAST, (const char *)&m_boardCast,
        sizeof( bool ) );
    res = ::setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)&m_reuseAddr,
        sizeof( bool ) );
    return res;
}


int UdpComm::ConnectSever(SOCKET &socket, const struct sockaddr * serverAddr)
{
    if ( ::connect( socket, serverAddr, sizeof( sockaddr_in ) ) == SOCKET_ERROR )
    {
        return WSAGetLastError();
    }
    else
    {
        return COMM_SUCCESS;
    }
}


int UdpComm::Close()
{
    if ( m_socket != INVALID_SOCKET )
    {
        CLOSE_SOCK( m_socket );
        return COMM_SUCCESS;
    }
    return COMM_FAILURE;
}


int UdpComm::Read(void* buffer, int len)
{
    if (m_socket != INVALID_SOCKET)
    {
		if ( NULL == buffer || len <= 0 )
		{
			return COMM_PARAM_ERR;
		}

        timeval timeout;
        timeout.tv_sec  = 0;
        timeout.tv_usec = m_timeWaitRecv * 1000;
 
		fd_set fdSetWrite;
        FD_ZERO(&fdSetWrite);
        FD_SET(m_socket, &fdSetWrite);

        int ret = ::select(m_socket + 1, &fdSetWrite, NULL, NULL, &timeout);
		if (ret > 0)
		{
			int bufferLen = ::recv(m_socket, (char*)buffer, len, 0);
			if (bufferLen < 0)
			{
                return ::WSAGetLastError();
			}
            else if (bufferLen == 0)
            {
                return COMM_FAILURE;
            }
			return bufferLen;
		}
        else if (ret == 0)
        {
            return COMM_TIMEOUT;
        }
        else
        {
            return ::WSAGetLastError();
        }
    }

    return COMM_FAILURE;
}


int UdpComm::Write(const void* buffer, int len)
{
    if (m_socket != INVALID_SOCKET)
    {
		if (buffer == NULL || len < 0)
		{
			return COMM_PARAM_ERR;
		}

		fd_set	fdSetWrite;	
		FD_ZERO(&fdSetWrite);
        FD_SET(m_socket, &fdSetWrite);

		struct timeval timeout;											
		timeout.tv_sec  = 0;
        timeout.tv_usec = m_timeWaitSend * 1000;

        int tempLen = len;
        const char* pTempBuffer = static_cast<const char *>(buffer);

        while ( tempLen > 0)
        {
            int ret = ::select( m_socket + 1, NULL, &fdSetWrite, NULL, &timeout );
            if ( ret > 0 )
            {
                int ret = ::send( m_socket, pTempBuffer, tempLen, 0 );
                if ( ret > 0 )
                {
                    tempLen -= ret;
                    pTempBuffer += ret;
                }
                else if ( ret == 0 )
                {
                    return COMM_FAILURE;
                }
                else
                {
                    return ::WSAGetLastError();
                }
            }
            else if ( ret == 0 )
            {
                return COMM_TIMEOUT;
            }
            else
            {
                return ::WSAGetLastError();
            }
        }

        if ( tempLen == 0)
        {
            return COMM_SUCCESS;
        }
	}

    return COMM_FAILURE;
}


int UdpComm::Config(int code, void* param)
{
    int res = COMM_FAILURE;

    if ( param == NULL)
    {
        return res;
    }

    switch ( code )
    {
    case UDP_CFG_SENDTIME:
    {
        m_timeWaitSend = *(int *)param;
        res = COMM_SUCCESS;
    }
    break;
    case UDP_CFG_RECVTIME:
    {
        m_timeWaitRecv = *(int *)param;
        res = COMM_SUCCESS;
    }
    break;
    case UDP_CFG_BOARDCAST:
    {
        res = setsockopt( m_socket, SOL_SOCKET, SO_BROADCAST, (const char *)param, 
            sizeof( bool ) );
    }
    break;
    case UDP_CFG_REUSEADR:
    {
        res = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, (const char *)param,
            sizeof( bool ) );
    }
    break;
    }

    if ( res > COMM_SUCCESS )
    {
        res = ::WSAGetLastError();
    }

    return res;
}

