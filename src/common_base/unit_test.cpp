#include "stdafx.h"
#include "unit_test.h"

CUnitTest::CUnitTest():
m_repeatCounts(1),
m_pOutPutSink( COutPutConsoleSink::GetInstance() )
{

}


CUnitTest::~CUnitTest()
{
    std::list<CTestBaseCase *>::iterator it;
    while (!m_testBaseList.empty() )
    {
        it = m_testBaseList.begin();
        delete (*it);
        m_testBaseList.erase( it );
    }
}


CUnitTest * CUnitTest::GetInstance()
{
    static CUnitTest s_unitTest;
    return &s_unitTest;
}


void CUnitTest::RunAllTests()
{
    memset( &m_testResultInfo, 0, sizeof( m_testResultInfo ) );
    std::list<CTestBaseCase *>::iterator it = m_testBaseList.begin();
    std::list<CTestBaseCase *>::iterator end = m_testBaseList.end();

    m_testResultInfo.totalCounts = m_testBaseList.size() * m_repeatCounts;
    unsigned int repeatCounts = m_repeatCounts;
    while ( repeatCounts-- )
    {
        it = m_testBaseList.begin();
        for ( ; it != end; ++it )
        {
            ( *it )->Run();
            TestResult result = ( *it )->GetTestResult();
            OutPutResultToSink( (*it) );

            //  统计信息
            if (result == TR_SUCCESSED)
            {
                ++m_testResultInfo.successCounts;
            }
            else
            {
                ++m_testResultInfo.failedCounts;
            }

            if ( result == TR_ABORT )
            {
                goto label;
            }
        }
    }

label:

    if ( repeatCounts != unsigned int(-1) || it != end)
    {
        m_testResultInfo.isFinished = false;
    }
    else
    {
        m_testResultInfo.isFinished = true;
    }

    OutPutTotalResultInfoToSink();
}


void CUnitTest::SetResultSink( COutPutBaseSink *pOutPutSink )
{
    m_pOutPutSink = pOutPutSink;
}


void CUnitTest::OutPutResultToSink( CTestBaseCase *pTestBaseCase )
{
    if ( m_pOutPutSink != nullptr )
    {
        m_pOutPutSink->Write( pTestBaseCase->GetTestResultInfoStr() );
    }
}


void CUnitTest::OutPutTotalResultInfoToSink()
{
    //  格式化统计信息，输出至目标
    if ( m_pOutPutSink != nullptr )
    {
        std::string totalInfoStr = StringFormatA( "\nSuccessed Couts: %d, Failed Couts: %d, Total Couts: %d\n", m_testResultInfo.successCounts, m_testResultInfo.failedCounts, m_testResultInfo.totalCounts);
        if (m_testResultInfo.isFinished == false)
        {
            totalInfoStr += "Test Exception Failed!\n";
        }
        else
        {
            totalInfoStr += "Test All Finished!\n";
        }
        m_pOutPutSink->Write( totalInfoStr );
    }
}


void CUnitTest::AddUnitTest( CTestBaseCase *pTestBase )
{
    m_testBaseList.push_back( pTestBase );
}


void CUnitTest::SetRepeatCouns( unsigned int repeatCounts )
{
    if ( repeatCounts < 1)
    {
        repeatCounts = 1;
    }

    m_repeatCounts = repeatCounts;
}


void CUnitTest::SetFailureHandle( TestFailureHandle testFailureHandle )
{
    std::list<CTestBaseCase *>::iterator it = m_testBaseList.begin();
    std::list<CTestBaseCase *>::iterator end = m_testBaseList.end();
    for ( ; it != end; ++it )
    {
        ( *it )->SetFailureHandle( testFailureHandle );
    }
}


CTestBaseCase::CTestBaseCase( std::string messageStr ) :
m_caseResult( TR_SUCCESSED ),
m_testFailureHandle( TFH_CONTINUE ),
m_messageStr( messageStr ),
m_describeTion( "" ),
m_subCaseCounts(0)
{

}


CTestBaseCase::~CTestBaseCase()
{

}


void CTestBaseCase::Run()
{
    m_subCaseCounts = 0;
    m_describeTion  = "";
    Test();
}


void CTestBaseCase::SetFailureHandle( TestFailureHandle testFailureHandle )
{
    m_testFailureHandle = testFailureHandle;
}


TestFailureHandle CTestBaseCase::GetFailureHandle()
{
    return m_testFailureHandle;
}


TestResult CTestBaseCase::GetTestResult()
{
    return m_caseResult;
}


std::string CTestBaseCase::GetTestResultInfoStr()
{
    //  格式化后的测试结果 
    //  通过的情形:      [PASS] 当前测试用例对象名 
    //  未通过的情形:    [FAILURE] 当前测试用例对象名 所在函数/行列/错误原因
    std::string  resultStr;
    static char formatSuccessStr[] = "[PASSED] %s\n";
    static char formatFailureStr[] = "[FAILED] %s %s\n";
    if ( m_caseResult == TR_SUCCESSED )
    {
        resultStr = StringFormatA( formatSuccessStr, m_messageStr.c_str() );
    }
    else
    {
        resultStr = StringFormatA( formatFailureStr, m_messageStr.c_str(), 
            m_describeTion.c_str() );
    }
      
     return resultStr;
}