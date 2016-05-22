///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   log.h  
/// 	@brief	��־����. 
/// 
///		��Ҫ�򵥵���־����,֧�ֶ���Ŀ�궨��;���Ʋ���;���ݱ��ݵ�  
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
#include "output_sink.h"    //  �ڲ�����winsock2.h ����windows.h��ͻ,����ǰ
#include <windows.h>
#include "singleton.h"
#include "auto_lock.h"
#include "lock_critical_section.h"
#include "mem_buffer.h"
#include "thread.h"

//  ��־��¼����,����
enum LOG_LEVEL
{
    LL_INFO = 0,    //  ��¼��Ϣ
    LL_WARNING,     //  ����
    LL_ERROR,       //  ����
    LL_FATAL,       //  ����
};


//  ��־��¼����
enum LOG_STRATEGY
{
    LS_NORMAL = 0,  //  ����
    LS_REALTIME,    //  ��ʱ
};


//  �洢�߳�
class CStoredThread : public CThread
{
public:

    CStoredThread( HANDLE logHandle, MemBufferPtr pBuffer, unsigned int bufferSize, COutPutBaseSink *pOutPutSink );

    ~CStoredThread();

public:

    /**
    *  @brief  ����ִ�еĴ洢����
    */
    virtual void Run();

private:

    HANDLE              m_logHandle;    //  �洢�¼����
    MemBufferPtr        m_pBuffer;      //  ���ݴ洢������
    unsigned int        m_bufferSize;   //  ���ݻ�������Ч�ֽ���
    COutPutBaseSink *   m_pOutPutSink;  //  ���Ŀ�����
};


typedef CSmartPtr<CStoredThread> CStoredThreadPtr;


//  ʵ�ּ���������־ϵͳ,����ʵ��
//  1. ����˫������ 2. ���߳� 3. �洢����(��ʱ������; ��¼�ȼ�) 4. �ض���
//  5. �̰߳�ȫ
//  ��д���̹߳��ڿ��٣����ܻ���ֶ���־��¼������,������
//  ĿǰΪ��������ֶ���������ʱ���õ�ǰ���ԣ�д���̹߳��죬����д���߳���΢�ȴ�һС��ʱ��
//  (�߳̿��ƶ���֮Join()�����ȴ���ȫд��Ŀ���������д���̼߳�������)
//  ��������д���̳߳�ʱ��ȴ����ɵ�����־��¼���������ʵ���С,�Դﵽ����д���Ŀ��
//  ��ʵ��һ������£���־ϵͳ�����¼��ô�����Ϣ
class CSmartLog
{
public:

    /**
    *  @brief  ������־��¼��������С(Ĭ��Ϊ1024 * 20�ֽ�)
    *  @param  [in] 	pOutPutSink         //  ���Ŀ��
    */
    void SetBufferSize( unsigned int bufferSize );

    /**
    *  @brief  ������־��¼���Ŀ��(Ĭ��Ϊ���������̨)
    *  @param  [in] 	pOutPutSink         //  ���Ŀ��
    */
    void SetTargetSink( COutPutBaseSink *pOutPutSink );

    /**
    *  @brief  д��Log��Ϣ
    *  @param  [in] 	logLevel            //  ��־����
    *  @param  [in] 	logStr              //  ��־������Ϣ
    */
    void WriteLog( LOG_LEVEL logLevel, const std::string & logStr );
    void WriteLog( LOG_LEVEL logLevel, const void * logStr, int len );
    void WriteLog( LOG_LEVEL logLevel, const char * logStr);

protected:

    CSmartLog();

    ~CSmartLog();

private:

    /**
    *  @brief  ��־��¼�����Ŀ��
    *  @param  [in] 	tempLogStr         //  ��־��¼��Ϣ
    */
    void LogToSink( const std::string &tempLogStr );

    /**
    *  @brief  ������־��¼�����ʣ�����Ϣд��
    */
    void DealLogStoreFinished();

private:

    unsigned int            m_bufferSize;       //  ��־��¼��������С
    MemBufferPtr            m_logBuffer;        //  ��־��¼������
    MemBufferPtr            m_logBufferBak;     //  ��־���ݻ�����
    CCriticalSectionLock    m_logLock;          //  ��־��
    HANDLE                  m_logHandle;        //  �洢��־�¼����
    HANDLE                  m_logHandleBak;     //  ���ݴ洢��־�¼����
    CThreadControl          m_threadCtr;        //  �洢��־�߳̿�����
    CStoredThreadPtr        m_threadPtr;        //  �洢�߳�
    COutPutBaseSink *       m_pOutPutSink;      //  �洢Ŀ��
    unsigned int            m_logBufferSize;    //  �洢���������ݴ�С
    unsigned int            m_logBufferBakSize; //  ���ݴ洢���������ݴ�С
};


typedef CSingletonManual<CSmartLog> CSingletonSmartLog;


#endif  //  _SMART_LOG_H_