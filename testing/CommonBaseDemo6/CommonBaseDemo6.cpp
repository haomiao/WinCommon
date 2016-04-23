// CommonBaseDemo6.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "smart_log.h"
#include "output_sink.h"

void TestConsoleLog()
{
    //std::cout << "........TestConsoleLog........." << std::endl;

    CSmartLog *pSmartLog = CSingletonSmartLog::GetInstance();
    pSmartLog->SetTargetSink( COutPutConsoleSink::GetInstance() );
    int counts = 1000;
    while (counts--)
    {
        //std::cout << "test number: " << 10 - counts << std::endl;
        pSmartLog->WriteLog( LL_INFO, "Info log str, come from TestConsoleLog function!" );
        pSmartLog->WriteLog( LL_WARNING, "Warning log str, come from TestConsoleLog function!" );
        pSmartLog->WriteLog( LL_ERROR, "Error log str, come from TestConsoleLog function!" );
        pSmartLog->WriteLog( LL_FATAL, "Fatal log str, come from TestConsoleLog function!" );
        //Sleep( 10 );
    }
}


void TestFileLog()
{
    //std::cout << "........TestFileLog........." << std::endl;

    CSmartLog *pSmartLog = CSingletonSmartLog::GetInstance();
    COutPutFileSink *pFileSink = COutPutFileSink::GetInstance();
    pFileSink->SetOutPutFilePath( "./LogInfo.log" );
    pSmartLog->SetTargetSink( pFileSink );

    int counts = 200000;
    while ( counts-- )
    {
        //std::cout << "test number: " << 10 - counts << std::endl;
        pSmartLog->WriteLog( LL_INFO, "Info log str, come from TestFileLog function!" );
        pSmartLog->WriteLog( LL_WARNING, "Warning log str, come from TestFileLog function!" );
        pSmartLog->WriteLog( LL_ERROR, "Error log str, come from TestFileLog function!" );
        pSmartLog->WriteLog( LL_FATAL, "Fatal log str, come from TestFileLog function!" );
        //Sleep( 10 );
    }
}


void TestTcpLog()
{
    //std::cout << "........TestTcpLog........." << std::endl;

    CSmartLog *pSmartLog = CSingletonSmartLog::GetInstance();
    COutPutTcpSink *pTcpSink = COutPutTcpSink::GetInstance();
    pTcpSink->SetOutPutTarget( "127.0.0.1", 5555 );
    pSmartLog->SetTargetSink( pTcpSink );
    int counts = 1000;
    while ( counts-- )
    {
        //std::cout << "test number: " << 10 - counts << std::endl;
        pSmartLog->WriteLog( LL_INFO, "Info log str, come from TestTcpLog function!" );
        pSmartLog->WriteLog( LL_WARNING, "Warning log str, come from TestTcpLog function!" );
        pSmartLog->WriteLog( LL_ERROR, "Error log str, come from TestTcpLog function!" );
        pSmartLog->WriteLog( LL_FATAL, "Fatal log str, come from TestTcpLog function!" );
        //Sleep( 10 );
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
    CSmartLog *pSmartLog = CSingletonSmartLog::GetInstance();
    pSmartLog->SetBufferSize( 1024 * 10 );
    TestConsoleLog();
    TestFileLog();
    TestTcpLog();

    //  提前释放,避免日志线程中因可能目标输出对象先释放，而导致异常
    CSingletonSmartLog::DestoryInstance();
	return 0;
}

