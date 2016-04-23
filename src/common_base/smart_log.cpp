#include "stdafx.h"

#include <time.h>
#include "smart_log.h"
#include "string_helper.h"


namespace
{
    #define     LEVEL_STR_SIZE      4
    static std::string levelStr[LEVEL_STR_SIZE] =
    {
        "INFO",
        "WARNING",
        "ERROR",
        "FATAL",
    };

    #define SAFE_CLOSE_HANDLE(obj) \
    { \
        if (obj != nullptr) \
        { \
            ::CloseHandle( obj ); \
            obj = nullptr; \
        } \
    }
}


CStoredThread::CStoredThread(HANDLE logHandle, MemBufferPtr pBuffer, unsigned int bufferSize, COutPutBaseSink *pOutPutSink) :CThread("StoreLogThread"),
m_logHandle(logHandle),
m_pBuffer(pBuffer),
m_pOutPutSink(pOutPutSink),
m_bufferSize( bufferSize )
{

}


CStoredThread::~CStoredThread()
{

}


void CStoredThread::Run()
{
    if ( m_pOutPutSink != nullptr)
    {
        m_pOutPutSink->Write( m_pBuffer->GetBuffer(), m_bufferSize );
        m_pOutPutSink->Flush();
        m_pBuffer->ReAlloc( 0 );
    }
    
    if (m_logHandle != nullptr)
    {
        ::SetEvent( m_logHandle );
    }
}



CSmartLog::CSmartLog():
m_logBuffer(new CMemBuffer() ),
m_logBufferBak(new CMemBuffer() ),
m_bufferSize( 10 * 1024 ),
m_pOutPutSink(COutPutConsoleSink::GetInstance() ),
m_threadCtr(nullptr),
m_logBufferSize(0),
m_logBufferBakSize(0)
{
    m_logHandle     = ::CreateEvent( NULL, TRUE, TRUE, NULL );
    m_logHandleBak  = ::CreateEvent( NULL, TRUE, TRUE, NULL );
}


CSmartLog::~CSmartLog()
{
    if ( m_threadPtr )
    {
        m_threadCtr.Join();
    }

    DealLogStoreFinished();

    SAFE_CLOSE_HANDLE( m_logHandle );
    SAFE_CLOSE_HANDLE( m_logHandleBak );
}


void CSmartLog::SetBufferSize( unsigned int bufferSize )
{
    m_bufferSize = bufferSize;
    m_logBuffer->Alloc( m_bufferSize );
    m_logBufferBak->Alloc( m_bufferSize );
}


void CSmartLog::SetTargetSink( COutPutBaseSink *pOutPutSink )
{
    DealLogStoreFinished();
    m_pOutPutSink = pOutPutSink;
}


void CSmartLog::WriteLog( LOG_LEVEL logLevel, const void * logStr, int len )
{
    std::string tempLogStr;
    __time64_t ltime;
    _time64( &ltime );
    tm localTm;
    if ( _localtime64_s( &localTm, &ltime ) == 0 )
    {
        tempLogStr = StringFormatA( "[%04d-%02d-%02d %02d:%02d:%02d] [%s]: %s\n",
            localTm.tm_year + 1900, localTm.tm_mon + 1, localTm.tm_mday,
            localTm.tm_hour, localTm.tm_min, localTm.tm_sec,
            levelStr[logLevel].c_str(), std::string( (const char *)logStr ,len).c_str() );
    }

    AutoLock lock( &m_logLock );
    LogToSink( tempLogStr );
}


void CSmartLog::WriteLog( LOG_LEVEL logLevel, const std::string & logStr )
{
    WriteLog( logLevel, logStr.c_str(), logStr.length() );
}


void CSmartLog::WriteLog( LOG_LEVEL logLevel, const char * logStr )
{
    WriteLog( logLevel, logStr, strlen(logStr) );
}


void CSmartLog::LogToSink( const std::string &tempLogStr )
{
    if ( m_logHandle != nullptr && ::WaitForSingleObject( m_logHandle, 0 ) == WAIT_OBJECT_0 )
    {
        size_t logStrSize = tempLogStr.length();
        size_t logBufferSize = m_logBuffer->GetSize();
        if ( logBufferSize <= logStrSize + m_logBufferSize )
        {
            m_logBuffer->ReAlloc( logBufferSize + logStrSize );
        }

        memcpy_s( (void *)( (char *)m_logBuffer->GetBuffer() + m_logBufferSize ), m_logBuffer->GetSize(), tempLogStr.c_str(), logStrSize );
        m_logBufferSize += logStrSize;

        if ( m_logBufferSize >= m_bufferSize )
        {
            ::ResetEvent( m_logHandle );
            m_threadCtr.Join();
            m_threadPtr = new CStoredThread( m_logHandle, m_logBuffer, m_logBufferSize, m_pOutPutSink );
            m_threadCtr = m_threadPtr->Start();
            m_logBufferSize = 0;
        }
    }
    else if ( m_logHandleBak != nullptr && ::WaitForSingleObject( m_logHandleBak, 0 ) == WAIT_OBJECT_0 )
    {
        size_t logStrSize = tempLogStr.length();
        size_t logBufferBakSize = m_logBufferBak->GetSize();
        if ( logBufferBakSize <= logStrSize + m_logBufferBakSize )
        {
            m_logBufferBak->ReAlloc( logBufferBakSize + logStrSize );
        }

        memcpy_s( (void *)( (char *)m_logBufferBak->GetBuffer() + m_logBufferBakSize ), m_logBufferBak->GetSize(), tempLogStr.c_str(), logStrSize );
        m_logBufferBakSize += logStrSize;

        if ( m_logBufferBakSize >= m_bufferSize )
        {
            ::ResetEvent( m_logHandleBak );
            m_threadCtr.Join();
            m_threadPtr = new CStoredThread( m_logHandleBak, m_logBufferBak, m_logBufferBakSize, m_pOutPutSink );
            m_threadCtr = m_threadPtr->Start();
            m_logBufferBakSize = 0;
        }
    }
}


void CSmartLog::DealLogStoreFinished()
{
    if ( m_logHandle != nullptr && ::WaitForSingleObject( m_logHandle, 0 ) == WAIT_OBJECT_0 && m_logBufferSize != 0 )
    {
        ::ResetEvent( m_logHandle );
        m_threadCtr.Join();
        m_threadPtr = new CStoredThread( m_logHandle, m_logBuffer, m_logBufferSize, m_pOutPutSink );
        m_threadCtr = m_threadPtr->Start();
        m_logBufferSize = 0;
    }
    else if ( m_logHandleBak != nullptr && ::WaitForSingleObject( m_logHandleBak, 0 ) == WAIT_OBJECT_0 && m_logBufferBakSize != 0 )
    {
        ::ResetEvent( m_logHandleBak );
        m_threadCtr.Join();
        m_threadPtr = new CStoredThread( m_logHandleBak, m_logBufferBak, m_logBufferBakSize, m_pOutPutSink );
        m_threadCtr = m_threadPtr->Start();
        m_logBufferBakSize = 0;
    }
}