#include "stdafx.h"
#include "output_sink.h"
#include "string_helper.h"

COutPutBaseSink::COutPutBaseSink() :
m_bOutPutHeaderFlag( true )
{

}


COutPutBaseSink::~COutPutBaseSink()
{

}


void COutPutBaseSink::Flush()
{

}


std::string COutPutBaseSink::GetOutPutHeaderInfo()
{
    if ( m_bOutPutHeaderFlag == true )
    {
        if ( m_customOutPutHeaderInfo.empty() )
        {
            return GetStandardHeaderInfo();
        }
        else
        {
            return m_customOutPutHeaderInfo;
        }
    }
    else
    {
        return "";
    }
}


void COutPutBaseSink::SetRecordOutPutHeader( bool outPutHeaderFlag )
{
    m_bOutPutHeaderFlag = outPutHeaderFlag;
}


bool COutPutBaseSink::IsRecordOutPutHeader()
{
    return m_bOutPutHeaderFlag;
}


void COutPutBaseSink::SetCustomOutPutHeaderInfo( const std::string & outPutHeaderInfoStr )
{
    m_customOutPutHeaderInfo = outPutHeaderInfoStr;
}


std::string COutPutBaseSink::GetStandardHeaderInfo()
{
    __time64_t ltime;
    _time64( &ltime );
    tm localTm;
    if ( _localtime64_s( &localTm, &ltime ) == 0 )
    {
        return StringFormatA( "[# %04d-%02d-%02d %02d:%02d:%02d]: ",
            localTm.tm_year + 1900, localTm.tm_mon + 1, localTm.tm_mday,
            localTm.tm_hour, localTm.tm_min, localTm.tm_sec );
    }
    else
    {
        return "";
    }
}


COutPutNoneSink::COutPutNoneSink()
{

}


COutPutNoneSink::~COutPutNoneSink()
{

}


COutPutNoneSink * COutPutNoneSink::GetInstance()
{
    static COutPutNoneSink s_outPutNoneSink;
    return &s_outPutNoneSink;
}


void COutPutNoneSink::Write( const std::string & outPutInfoStr )
{

}


void COutPutNoneSink::Write( const char outPutInfoC )
{

}


void COutPutNoneSink::Write( const void* outPutInfo, int len )
{

}


COutPutConsoleSink::COutPutConsoleSink()
{

}


COutPutConsoleSink::~COutPutConsoleSink()
{

}


COutPutConsoleSink * COutPutConsoleSink::GetInstance()
{
    static COutPutConsoleSink s_outPutConsoleSink;
    return &s_outPutConsoleSink;
}


void COutPutConsoleSink::Write( const std::string & outPutInfoStr )
{
    std::cout << outPutInfoStr << std::endl;
}


void COutPutConsoleSink::Write( const char outPutInfoC )
{
    std::cout << outPutInfoC << std::endl;
}

void COutPutConsoleSink::Write( const void* outPutInfo, int len )
{
    std::cout << std::string( (const char *)outPutInfo, len) << std::endl;
}


void COutPutConsoleSink::Flush()
{
    std::cout.flush();
}


COutPutFileSink::COutPutFileSink() :
m_outPutFilePath( "" )
{

}


COutPutFileSink * COutPutFileSink::GetInstance()
{
    static COutPutFileSink s_fileSink;
    return &s_fileSink;
}


COutPutFileSink::~COutPutFileSink()
{
    SafeCloseFile();
}


void COutPutFileSink::Write( const std::string & outPutInfoStr )
{
    if ( m_fstream.is_open() == true )
    {
        m_fstream << outPutInfoStr << std::endl;
    }
    else
    {
        std::cout << outPutInfoStr << std::endl;
    }
}


void COutPutFileSink::Write( const char outPutInfoC )
{
    if ( m_fstream.is_open() == true )
    {
        m_fstream << outPutInfoC << std::endl;
    }
    else
    {
        std::cout << outPutInfoC << std::endl;
    }
}


void COutPutFileSink::Write( const void* outPutInfo, int len )
{
    if ( m_fstream.is_open() == true )
    {
        m_fstream << std::string( (const char *)outPutInfo, len ) << std::endl;
    }
    else
    {
        std::cout << std::string( (const char *)outPutInfo, len ) << std::endl;
    }
}


void COutPutFileSink::Flush()
{
    if ( m_fstream.is_open() == true )
    {
        m_fstream.flush();
    }
}


void COutPutFileSink::SetOutPutFilePath( const char * filePath )
{
    SafeCloseFile();
    if ( filePath != nullptr )
    {
        m_outPutFilePath = filePath;
    }
    OpenFile();
}


void COutPutFileSink::OpenFile()
{
    m_fstream.open( m_outPutFilePath, std::ios_base::out | std::ios_base::trunc );
}


void COutPutFileSink::SafeCloseFile()
{
    Flush();
    if ( m_fstream.is_open() == true )
    {
        m_fstream.close();
    }
}


COutPutTcpSink * COutPutTcpSink::GetInstance()
{
    static COutPutTcpSink s_tcpSink;
    return &s_tcpSink;
}


COutPutTcpSink::COutPutTcpSink() :
m_port( 0 ),
m_socket( INVALID_SOCKET ),
m_timeWaitSend( 3000 ),
m_isNetValid( false ),
m_ipAddress( "" )
{
    WSADATA wsaData;
    if ( ::WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) == 0 )
    {
        m_isNetValid = true;
    }
}


COutPutTcpSink::~COutPutTcpSink()
{
    SafeCloseConnect();
    ::WSACleanup();
}


void COutPutTcpSink::Write( const std::string & outPutInfoStr )
{
    Write( (const void*)outPutInfoStr.c_str(), outPutInfoStr.length() );
}


void COutPutTcpSink::Write( const char outPutInfoC )
{
    Write( (const void*)&outPutInfoC, 1 );
}


void COutPutTcpSink::Write( const void* outPutInfo, int len )
{
    if ( m_socket != INVALID_SOCKET )
    {
        if ( outPutInfo == NULL || len < 0 )
        {
            return;
        }

        fd_set	fdSetWrite;
        FD_ZERO( &fdSetWrite );
        FD_SET( m_socket, &fdSetWrite );

        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = m_timeWaitSend * 1000;

        int tempLen = len;
        const char* pTempBuffer = static_cast<const char *>( outPutInfo );

        while ( tempLen > 0 )
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
                else
                {
                    return;
                }
            }
            else
            {
                return;
            }
        }
    }
}


void COutPutTcpSink::SetDefaultCommParameters()
{
    int             lTimeWaitSend = 3000;             //  数据发送超时(ms)
    struct linger   lLinger = { 1, 3 };         //  延迟释放
    int             lNodelay = 1;                //  无延时
    int             lKeepAlive = 0;                //  保活

    ::setsockopt( m_socket, SOL_SOCKET, SO_SNDTIMEO, (const char *)&lTimeWaitSend,
        sizeof( int ) );
    ::setsockopt( m_socket, SOL_SOCKET, SO_LINGER, (const char *)&lLinger,
        sizeof( lLinger ) );
    ::setsockopt( m_socket, IPPROTO_TCP, TCP_NODELAY, (const char *)&lNodelay,
        sizeof( int ) );
    ::setsockopt( m_socket, SOL_SOCKET, SO_KEEPALIVE, (const char *)&lKeepAlive,
        sizeof( int ) );
}


void COutPutTcpSink::SetOutPutTarget( const char * ipAddress, unsigned short port )
{
    SafeCloseConnect();

    if ( m_isNetValid == false || ipAddress == nullptr )
    {
        return;
    }

    m_ipAddress = ipAddress;
    m_port = port;
    ConnectTarget();
}


void COutPutTcpSink::ConnectTarget()
{
    m_socket = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( m_socket != INVALID_SOCKET )
    {
        SetDefaultCommParameters();

        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.S_un.S_addr = inet_addr( m_ipAddress.c_str() );
        addr.sin_port = htons( m_port );
        if ( ::connect( m_socket, (sockaddr *)&addr, sizeof( addr ) ) == SOCKET_ERROR )
        {
            SafeCloseConnect();
        }
    }
}


void COutPutTcpSink::SafeCloseConnect()
{
    if ( m_socket != INVALID_SOCKET )
    {
        ::shutdown( m_socket, SD_SEND );
        ::closesocket( m_socket );
        m_socket = INVALID_SOCKET;
    }
}
