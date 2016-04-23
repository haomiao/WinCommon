///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   log.h  
/// 	@brief	日志工具. 
/// 
///		主要简单的日志工具,支持多种目标定向;定制策略;数据备份等  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/24  15:33:54
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef   _SMART_LOG_H_
#define   _SMART_LOG_H_

#include <vector>
#include <map>
#include <string>
#include "output_sink.h"    //  内部含有winsock2.h 会与windows.h冲突,故置前
#include <windows.h>
#include "singleton.h"
#include "auto_lock.h"
#include "lock_critical_section.h"
#include "mem_buffer.h"
#include "thread.h"

//  日志记录类型,级别
enum LOG_LEVEL
{
    LL_INFO = 0,    //  记录信息
    LL_WARNING,     //  警告
    LL_ERROR,       //  错误
    LL_FATAL,       //  致命
};


//  日志记录策略
enum LOG_STRATEGY
{
    LS_NORMAL = 0,  //  常规
    LS_REALTIME,    //  即时
};


//  存储线程
class CStoredThread : public CThread
{
public:

    CStoredThread( HANDLE logHandle, MemBufferPtr pBuffer, unsigned int bufferSize, COutPutBaseSink *pOutPutSink );

    ~CStoredThread();

public:

    /**
    *  @brief  具体执行的存储任务
    */
    virtual void Run();

private:

    HANDLE              m_logHandle;    //  存储事件句柄
    MemBufferPtr        m_pBuffer;      //  数据存储缓冲区
    unsigned int        m_bufferSize;   //  数据缓冲区有效字节数
    COutPutBaseSink *   m_pOutPutSink;  //  输出目标对象
};


typedef CSmartPtr<CStoredThread> CStoredThreadPtr;


//  实现简易智能日志系统,单例实现
//  1. 采用双缓冲区 2. 多线程 3. 存储策略(即时、常规; 记录等级) 4. 重定向
//  5. 线程安全
//  若写入线程过于快速，可能会出现丢日志记录的现象,待完善
//  目前为不打算出现丢包现象，暂时采用当前策略：写入线程过快，则让写入线程稍微等待一小段时间
//  (线程控制对象之Join()，将等待完全写入目标后再允许写入线程继续操作)
//  若不想让写入线程长时间等待，可调整日志记录缓冲区至适当大小,以达到快速写入的目的
//  事实上一般情况下，日志系统不会记录那么快的信息
class CSmartLog
{
public:

    /**
    *  @brief  设置日志记录缓冲区大小(默认为1024 * 20字节)
    *  @param  [in] 	pOutPutSink         //  输出目标
    */
    void SetBufferSize( unsigned int bufferSize );

    /**
    *  @brief  设置日志记录输出目标(默认为输出至控制台)
    *  @param  [in] 	pOutPutSink         //  输出目标
    */
    void SetTargetSink( COutPutBaseSink *pOutPutSink );

    /**
    *  @brief  写入Log信息
    *  @param  [in] 	logLevel            //  日志级别
    *  @param  [in] 	logStr              //  日志内容信息
    */
    void WriteLog( LOG_LEVEL logLevel, const std::string & logStr );
    void WriteLog( LOG_LEVEL logLevel, const void * logStr, int len );
    void WriteLog( LOG_LEVEL logLevel, const char * logStr);

protected:

    CSmartLog();

    ~CSmartLog();

private:

    /**
    *  @brief  日志记录输出至目标
    *  @param  [in] 	tempLogStr         //  日志记录信息
    */
    void LogToSink( const std::string &tempLogStr );

    /**
    *  @brief  处理日志记录，完成剩余的信息写入
    */
    void DealLogStoreFinished();

private:

    unsigned int            m_bufferSize;       //  日志记录缓冲区大小
    MemBufferPtr            m_logBuffer;        //  日志记录缓冲区
    MemBufferPtr            m_logBufferBak;     //  日志备份缓冲区
    CCriticalSectionLock    m_logLock;          //  日志锁
    HANDLE                  m_logHandle;        //  存储日志事件句柄
    HANDLE                  m_logHandleBak;     //  备份存储日志事件句柄
    CThreadControl          m_threadCtr;        //  存储日志线程控制器
    CStoredThreadPtr        m_threadPtr;        //  存储线程
    COutPutBaseSink *       m_pOutPutSink;      //  存储目标
    unsigned int            m_logBufferSize;    //  存储缓冲区内容大小
    unsigned int            m_logBufferBakSize; //  备份存储缓冲区内容大小
};


typedef CSingletonManual<CSmartLog> CSingletonSmartLog;


#endif  //  _SMART_LOG_H_