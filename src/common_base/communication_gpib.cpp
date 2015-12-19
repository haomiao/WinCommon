#include "StdAfx.h"

#include "auto_lock.h"
#include "communication_gpib.h"

/*
/* 通信在传统模式下，全局变量ibsta、iberr、ibcntl
/* 在多设备数据交互下变得不可用,需加锁
*/


CCriticalSectionLock GpibComm::m_lock;

GpibComm::GpibComm():
m_hHandle(-1),
m_timeout( T3s )
{

}


GpibComm::~GpibComm()
{

}


int GpibComm::Open( const void* arg )
{
	if ( arg == NULL)
	{
		return COMM_PARAM_ERR;
	}

    GpibCommArg * comArg  = reinterpret_cast<GpibCommArg*>( const_cast<void *>( arg ) );
	if ( comArg != NULL)
	{
		return COMM_PARAM_ERR;
	}

	return OpenDevice(	comArg->BoardIndex,
						comArg->PrimaryAddr,
						comArg->SecondAddr,
						comArg->TimeOut,
						comArg->Eot,
						comArg->Eos);

}


int GpibComm::Read(void* buffer, int len)
{
	int res = COMM_SUCCESS;
	if ( NULL == buffer || len <= 0)
	{
		return COMM_PARAM_ERR;
	}

	AutoLock lock(&m_lock);
	ibrd(m_hHandle, buffer, len);
	if (hasError() )
	{
        //GPIBClear();
		return COMM_UNKNOWN_ERR;
	}
	return ibcnt;	//  返回获取长度
}


int GpibComm::Write(const void* buffer, int len)
{
	int res = COMM_FAILURE;
	void * pBuffer = const_cast<void *>(buffer);
	if (NULL == pBuffer)
	{
		return COMM_PARAM_ERR;
	}

    AutoLock lock( &m_lock );
    int tempLen = len;
    const char* pTempBuffer = static_cast<const char *>( buffer );
    while (tempLen > 0)
    {
        ibwrt( m_hHandle, (void *)pTempBuffer, tempLen );
	    if (hasError() )
	    {
            //GPIBClear();
		    res =  COMM_UNKNOWN_ERR;
            break;
	    }

        tempLen -= ibcnt;
        pTempBuffer += ibcnt;
    }
	
    if ( tempLen == 0)
    {
        res = COMM_SUCCESS;
    }

	return res;
}


int GpibComm::Config(int code, void* param)
{
	int res = COMM_FAILURE;
    if ( param == NULL)
    {
        return res;
    }

	switch (code)
	{
    case GPIB_CFG_TIME_OUT:
    {
        m_timeout = *(int*)param;
        res = COMM_SUCCESS;
        if (m_hHandle != -1)
        {
            ibtmo( m_hHandle, m_timeout );
            if ( hasError() )
            {
                res = COMM_UNKNOWN_ERR;
            }
        }
    }
    break;

    }

    return res;
}


int GpibComm::Close()
{
	int res = COMM_SUCCESS;
    if (m_hHandle != -1)
    {
        CloseDevice(0);
        m_hHandle = -1;
    }
	
	if (hasError() )
	{
		res = COMM_UNKNOWN_ERR;
	}
	return res;
}


int GpibComm::OpenDevice( int boardID, int pad, int sad, int tmo, int eot, int eos )
{
	if (m_hHandle != -1)
	{
		int res		= Close();
		m_hHandle	= -1;
		if (res == COMM_UNKNOWN_ERR)
		{
			return res;
		}
	}

	m_hHandle = ibdev(boardID, pad, sad, tmo, eot, eos);
	
	if (hasError() )
	{
		return COMM_UNKNOWN_ERR;
	}

	//ibclr(m_hHandle);
	//ibconfig(m_hHandle, IbcEOSchar, '\0');

    ibtmo( m_hHandle, m_timeout );
    if ( hasError() )
    {
        return COMM_UNKNOWN_ERR;
    }

	return	COMM_SUCCESS;
}


int GpibComm::CloseDevice( int value )
{
	return	ibonl(m_hHandle, value);
}


int	GpibComm::GPIBClear()
{
	return ibclr(m_hHandle);
}


bool GpibComm::hasError()
{
    return ibsta & ERR ? true : false;
}

