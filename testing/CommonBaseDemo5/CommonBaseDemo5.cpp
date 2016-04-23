// CommonBaseDemo5.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include "unit_test.h"
#include "output_sink.h"

unsigned int fun1()
{
    return 5;
}


unsigned int fun2()
{
    return 8;
}


unsigned int fun3()
{
    return 8;
}


FUNC_CASE( UserCase1 )
{
    ASSERT_CASE( 3 == 5 );
    EXPECT_EQ( 3, 4 );
    EXPECT_NEQ( 3, 4 );
    EXPECT_GR( 3, 4 );
    EXPECT_LE( 6, 5 );
    EXPECT_GRE( 6, 8 );
    EXPECT_LEE( 6, 6);
}


FUNC_CASE( UserCase2 )
{
    ASSERT_CASE( 6 == 6 );
    EXPECT_EQ( fun1(), fun2() );
    EXPECT_NEQ( 3, 3 );
    EXPECT_GR( 5, 4 );
    EXPECT_LE(3, 5);
    EXPECT_GRE( fun1(), fun2() );
    EXPECT_LEE( fun2(), fun3() );
}


FUNC_CASE( UserCase3 )
{
    ASSERT_CASE( 0 == 0 );
    EXPECT_EQ( fun3(), fun2() );
}


int _tmain(int argc, _TCHAR* argv[])
{
    CUnitTest *pUintTest = CUnitTest::GetInstance();
    pUintTest->SetFailureHandle( TFH_CONTINUE );
    pUintTest->SetRepeatCouns( 5 );
    pUintTest->SetResultSink( COutPutConsoleSink::GetInstance() );
    pUintTest->RunAllTests();

    COutPutFileSink *pFileSink = COutPutFileSink::GetInstance();
    pFileSink->SetOutPutFilePath( "./UnitTestInfo.txt" );
    pUintTest->SetResultSink( pFileSink );
    pUintTest->SetRepeatCouns( 3 );
    pUintTest->SetFailureHandle( TFH_ABORT );
    pUintTest->RunAllTests();

    COutPutTcpSink *pTcpSink = COutPutTcpSink::GetInstance();
    pTcpSink->SetOutPutTarget( "127.0.0.1", 5555 );
    pUintTest->SetRepeatCouns( 5 );
    pUintTest->SetFailureHandle( TFH_CONTINUE );
    pUintTest->SetResultSink( pTcpSink );
    pUintTest->RunAllTests();

	return 0;
}

