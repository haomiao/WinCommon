#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <wchar.h>
#include <tchar.h>
#include "communication_rs232.h"


namespace
{
	#define BUFFER_SIZE		2 * 1024 * 1024

	#define CLOSE_HANDLE(object)\
		{\
			CloseHandle(object);\
			object = INVALID_HANDLE_VALUE;\
		}
}


RS232Comm::RS232Comm():
m_comHandle(INVALID_HANDLE_VALUE),
m_inputBufferSize( BUFFER_SIZE ),
m_outputBufferSize( BUFFER_SIZE ),
m_commTimeouts( { 0xFFFFFFFF, 0 , 100, 0, 50} ),
m_pureFlags( PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR )
{

}


RS232Comm::~RS232Comm()
{
	Close();
}


int RS232Comm::Open( const void* arg )
{	
	if ( arg == NULL)
	{
		return COMM_PARAM_ERR;
	}

    RS232CommArg* comArg = reinterpret_cast<RS232CommArg*>( const_cast<void *>( arg ) );
	if ( !comArg  || comArg->comNum > 255)
	{
		return COMM_PARAM_ERR;
	}

	TCHAR szComStr[16] = _T("");
	_stprintf_s(szComStr, _T("COM%d"), comArg->comNum );

	if (m_comHandle != INVALID_HANDLE_VALUE)
	{
		Close();
	}

	/*打开串口*/
	m_comHandle = ::CreateFile(
								szComStr,
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,	
								NULL
								);
	
	if (m_comHandle == INVALID_HANDLE_VALUE)
	{
		//ASSERT(_T("串口打开失败") );
		return COMM_FAILURE;
	}
	
	if (SetupPort() == FALSE)
	{
        CLOSE_HANDLE( m_comHandle );
		return COMM_FAILURE;
	}

	if ( SetState(comArg->baudRate, comArg->byteSize, comArg->parity, comArg->stopBits, comArg->ctrl) == FALSE )
	{
        CLOSE_HANDLE( m_comHandle );
        return COMM_FAILURE;
	}

	return COMM_SUCCESS;
}


int RS232Comm::Close()
{
    if ( m_comHandle != INVALID_HANDLE_VALUE )
    {
        CLOSE_HANDLE( m_comHandle );
        return COMM_SUCCESS;
    }

    return COMM_FAILURE;
}


int RS232Comm::Read(void* buffer, int len)
{
	if (m_comHandle == INVALID_HANDLE_VALUE)
	{
		return COMM_FAILURE;
	}

	if ( NULL == buffer || len <= 0 )
	{
		return COMM_PARAM_ERR;
	}

	COMSTAT  Stat;
	DWORD dwError;
//	Sleep(1000);
	if(::ClearCommError(m_comHandle, &dwError, &Stat) && dwError > 0)	//	清除错误
	{
		::PurgeComm(m_comHandle, PURGE_RXABORT | PURGE_RXCLEAR);		//	清除输入缓冲区
		return COMM_TIMEOUT;
	}
// 	if(!Stat.cbInQue)													//	缓冲区无数据
// 	{
// 		return COMM_TIMEOUT;
// 	}

	DWORD recvLen = 0; 

	if( ::ReadFile(m_comHandle, buffer, len, &recvLen, NULL) )
	{   
		return static_cast<int>(recvLen);
	}
    else
    {
        return COMM_TIMEOUT;
    }
}


int RS232Comm::Write(const void* buffer, int len)
{	
	if (m_comHandle == INVALID_HANDLE_VALUE)
	{
		return COMM_FAILURE;
	}
	
	if ( NULL == buffer || len < 0)
	{
		return COMM_PARAM_ERR;
	}

	DWORD dwError;
	if(::ClearCommError(m_comHandle, &dwError, NULL) && dwError > 0)	//	清除错误
	{
		::PurgeComm(m_comHandle, PURGE_TXABORT | PURGE_TXCLEAR);
		return COMM_TIMEOUT;
	}

    const char* pTempBuffer = static_cast<const char *>( buffer );
    int tempLen = len;

	DWORD uWritedLength = 0;
	COMSTAT  Stat;
    
    while ( tempLen > 0)
    {
        if ( ::WriteFile( m_comHandle, pTempBuffer, tempLen, &uWritedLength, NULL ) != 0 )
        {
            if ( ::ClearCommError( m_comHandle, &dwError, &Stat ) && dwError > 0 )	//	清除错误
            {
                ::PurgeComm( m_comHandle, PURGE_TXABORT | PURGE_TXCLEAR );
                return COMM_TIMEOUT;
            }

            if ( Stat.cbOutQue )
            {
                return COMM_TIMEOUT;
            }

            tempLen -= uWritedLength;
            pTempBuffer += uWritedLength;
        }
        else
        {
            return COMM_FAILURE;
        }
    }
	
    if (tempLen == 0)
    {
        return COMM_SUCCESS;
    }

	return COMM_TIMEOUT;
}


int RS232Comm::Config(int code, void* param)
{
    int res = COMM_FAILURE;

    if ( param == NULL )
    {
        return res;
    }

    switch ( code )
    {
    case RS232_CFG_INPUTBUFFER:
    {
        m_inputBufferSize = *(DWORD *)param;
        if ( m_comHandle != INVALID_HANDLE_VALUE )
        {
            if ( 0 == ::SetupComm( m_comHandle, m_inputBufferSize, m_outputBufferSize ) )
            {
                break;
            }
        }
        res = COMM_SUCCESS;
    }
    break;
    case RS232_CFG_OUTPUTBUFFER:
    {
        m_outputBufferSize = *(DWORD *)param;
        if (m_comHandle != INVALID_HANDLE_VALUE)
        {
            if ( 0 == ::SetupComm( m_comHandle, m_inputBufferSize, m_outputBufferSize ) )
            {
                break;
            }
        }
        res = COMM_SUCCESS;
    }
    break;
    case RS232_CFG_TIMEOUTS:
    {
        COMMTIMEOUTS * pCommtimeOut = reinterpret_cast<COMMTIMEOUTS *>( param );
        if (m_comHandle != INVALID_HANDLE_VALUE)
        {
            if ( 0 == ::SetCommTimeouts( m_comHandle, &m_commTimeouts ) )
            {
                break;
            }
        }
        res = COMM_SUCCESS;
    }
    break;
    case RS232_CFG_PURE:
    {
        m_pureFlags = *(DWORD *)param;
        if ( m_comHandle != INVALID_HANDLE_VALUE )
        {
            if ( 0 == ::PurgeComm( m_comHandle, m_pureFlags ) )
            {
                break;
            }
        }
        res = COMM_SUCCESS;
    }
    break;

    }

    return res;
}


BOOL RS232Comm::SetupPort()
{
	if (INVALID_HANDLE_VALUE == m_comHandle )
	{
		return FALSE;
	}
 
    //	设置推荐缓冲区
    if ( ::SetupComm( m_comHandle, m_inputBufferSize, m_outputBufferSize ) == 0)
	{
		return FALSE;
	}
	
    //	设置超时时间
    if ( ::SetCommTimeouts( m_comHandle, &m_commTimeouts ) == 0)
	{
		return FALSE;	
	}

	// 清空串口缓冲区
	if( ::PurgeComm(m_comHandle, m_pureFlags ) == 0)
	{
		return FALSE;	
	}

	return TRUE;
}


BOOL RS232Comm::SetState(unsigned long dwBaudRate, 
						 unsigned char dwByteSize, 
						 unsigned char dwParity, 
						 unsigned char dwStopBits,
						 RS232_CONTROL ctrl)
{
	if(m_comHandle != INVALID_HANDLE_VALUE )
	{
		DCB tempDCB;
        if ( !::GetCommState( m_comHandle, &tempDCB ) )
		{
			return FALSE;
		}

        tempDCB.BaudRate = dwBaudRate;
        tempDCB.ByteSize = dwByteSize;
        tempDCB.Parity = dwParity;
        tempDCB.StopBits = dwStopBits;

		if (RS232_RTS == ctrl)								//	硬件流控
		{
            tempDCB.fOutX = FALSE;
            tempDCB.fInX = FALSE;
            tempDCB.fOutxCtsFlow = TRUE;
            tempDCB.fDtrControl = DTR_CONTROL_ENABLE;
            tempDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;
		}
		else if (RS232_DTR == ctrl)							//	软件流控
		{
            tempDCB.fOutxCtsFlow = FALSE;
            tempDCB.fDtrControl = DTR_CONTROL_DISABLE;
            tempDCB.fRtsControl = RTS_CONTROL_DISABLE;
            tempDCB.fOutxDsrFlow = FALSE;

            tempDCB.fOutX = TRUE;
            tempDCB.fInX = TRUE;
            tempDCB.XonLim = 0;
            tempDCB.XoffLim = 0;
            tempDCB.XonChar = 0x11;
            tempDCB.XoffChar = 0x13;

		}
		else if(RS232_NO_C == ctrl || ctrl == RS232_DEFAULT)    //	无流控
		{
            tempDCB.fOutX = FALSE;
            tempDCB.fInX = FALSE;
            tempDCB.fOutxCtsFlow = FALSE;
            tempDCB.fDtrControl = DTR_CONTROL_DISABLE;
            tempDCB.fRtsControl = RTS_CONTROL_DISABLE;
            tempDCB.fOutxDsrFlow = FALSE;
		}

        return ::SetCommState( m_comHandle, &tempDCB );
	}

	return FALSE;
}