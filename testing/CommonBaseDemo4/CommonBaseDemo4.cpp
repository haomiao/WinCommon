// CommonBaseDemo4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "debug_assist.h"

void TestNoneDebug()
{
    std::cout << "....Enter TestNoneDebug...." << std::endl;
    CDebugAssist * pDebugAssist = CDebugAssist::GetInstance();
    pDebugAssist->SetSink( COutPutNoneSink::GetInstance() );
    pDebugAssist->Write( "None Output Strings begin!" );
    pDebugAssist->Write( "None Output Strings1" );
    pDebugAssist->Write( "None Output Strings2" );
    pDebugAssist->Write( 'H' );
    pDebugAssist->Write( 'O' );
    pDebugAssist->Write( "None Output Strings end!" );
    std::cout << "....Level TestNoneDebug...." << std::endl << std::endl;
    pDebugAssist->SetSink( nullptr );
}

void TestConsoleDebug()
{
    std::cout << "....Enter TestConsoleDebug...." << std::endl;
    CDebugAssist * pDebugAssist = CDebugAssist::GetInstance();
    pDebugAssist->SetSink( COutPutConsoleSink::GetInstance() );
    pDebugAssist->Write( "Console Output Strings begin!" );
    pDebugAssist->Write( "Console Output Strings1" );
    pDebugAssist->Write( "Console Output Strings2" );
    pDebugAssist->Write( 'H' );
    pDebugAssist->Write( 'O' );
    pDebugAssist->Write( "Console Output Strings end!" );
    std::cout << "....Level TestConsoleDebug...." << std::endl << std::endl;
    pDebugAssist->SetSink( nullptr );
}


void TestFileDebug()
{
    std::cout << "....Enter TestFileDebug...." << std::endl;
    CDebugAssist * pDebugAssist = CDebugAssist::GetInstance();
    COutPutFileSink  *pFileSink = COutPutFileSink::GetInstance();
    pFileSink->SetOutPutFilePath( "./DebugInfo.txt" );
    pDebugAssist->SetSink( pFileSink );
    pDebugAssist->Write( "File Output Strings begin!" );
    pDebugAssist->Write( "File Output Strings1" );
    pDebugAssist->Write( "File Output Strings2" );
    pDebugAssist->Write( 'H' );
    pDebugAssist->Write( 'O' );
    pDebugAssist->Write( "File Output Strings end!" );
    std::cout << "....Level TestFileDebug...." << std::endl << std::endl;
    pDebugAssist->SetSink( nullptr );
}


void TestTcpDebug()
{
    std::cout << "....Enter TestTcpDebug...." << std::endl;
    CDebugAssist * pDebugAssist = CDebugAssist::GetInstance();
    COutPutTcpSink  *pTcpSink = COutPutTcpSink::GetInstance();
    pTcpSink->SetOutPutTarget( "127.0.0.1", 5555 );
    pDebugAssist->SetSink( pTcpSink );
    pDebugAssist->Write( "Tcp Output Strings begin!" );
    pDebugAssist->Write( "Tcp Output Strings1" );
    pDebugAssist->Write( "Tcp Output Strings2" );
    pDebugAssist->Write( 'H' );
    pDebugAssist->Write( 'O' );
    pDebugAssist->Write( "Tcp Output Strings end!" );
    std::cout << "....Level TestTcpDebug...." << std::endl << std::endl;
    pDebugAssist->SetSink( nullptr );
}



int _tmain(int argc, _TCHAR* argv[])
{
    CDebugAssist::GetInstance()->SetDebugLevel( DL_ALL );
    TestNoneDebug();
    TestConsoleDebug();
    TestFileDebug();
    TestTcpDebug();
	return 0;
}

