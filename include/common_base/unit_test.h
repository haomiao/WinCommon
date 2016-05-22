///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   unit_test.h
/// 	@brief	��Ԫ����. 
/// 
///		��Ҫʵ�ּ��׵ĵ�Ԫ���ԣ��ṩʵ�á����׵Ĳ��Բ���  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/11  9:41:46
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _UNIT_TEST_H_
#define     _UNIT_TEST_H_


#include "config.h"
#include "output_sink.h"
#include "string_helper.h"
#include <list>
#include <string>


//  �������Խ��
enum TestResult
{
    TR_SUCCESSED,       //  ������ɲ�ͨ������
    TR_FAILED,          //  ���������ʧ��,�����쳣
    TR_ABORT,           //  ����δ�����ʧ��
};


//  ����ʧ�ܴ���ʽ
enum TestFailureHandle
{
    TFH_CONTINUE,       //  ����ʧ��ʱ���������в���
    TFH_ABORT,          //  ����ʧ��ʱ��������ֹʣ�����
};


//  ���Խ��ͳ����Ϣ
struct TestResultInfo
{
    unsigned int totalCounts;       //  �����ܼ���
    unsigned int successCounts;     //  ���Խ���ɹ�����
    unsigned int failedCounts;      //  ���Խ��ʧ�ܼ���
    bool         isFinished;        //  �����Ƿ����
};


//  ǰ������
class CTestBaseCase;

//  ��Ԫ����
//  ����ʵ�֣��ⲿͳһʹ�ø���ʵ�ֵ�Ԫ����
//  Ĭ�����Ϊ����̨,���޸����Ŀ��
class CUnitTest
{
public:

    virtual ~CUnitTest();

    static CUnitTest * GetInstance();

public:

    /**
    *  @brief  ִ�����еĲ�������
    */
    void RunAllTests();

    /**
    *  @brief  ��ӵ�Ԫ����
    *  @param  [in] 	pTestBase           //  ������������
    */
    void AddUnitTest( CTestBaseCase *pTestBase );

    /**
    *  @brief  �����ظ����Դ���(���ٲ���һ��)
    *  @param  [in] 	repeatCounts        //  �ظ�����
    */
    void SetRepeatCouns( unsigned int repeatCounts );

    /**
    *  @brief  ���ò�������ʧ��ʱ����ʽ
    *  @param  [in] 	testFailureHandle   //  ����ʧ�ܴ���ʽ
    */
    void SetFailureHandle( TestFailureHandle testFailureHandle );

    /**
    *  @brief  ���ò��Խ�����Ŀ��
    *  @param  [in] 	pOutPutSink        //  ���Ŀ��
    */
    void SetResultSink( COutPutBaseSink *pOutPutSink );

private:

    /**
    *  @brief  ���Խ�����Ŀ��
    */
    void OutPutResultToSink( CTestBaseCase *pTestBaseCase);
    void OutPutTotalResultInfoToSink();

private:

    CUnitTest();

    DISALLOW_COPY_AND_ASSIGN( CUnitTest );

private:

    std::list<CTestBaseCase *>  m_testBaseList;         //  ���������б����
    unsigned int                m_repeatCounts;         //  �ظ����Դ���
    COutPutBaseSink *           m_pOutPutSink;          //  ���Խ�����Ŀ��
    TestResultInfo              m_testResultInfo;       //  ���Խ��ͳ����Ϣ
};



//  ��Ԫ������������
//  �ṩ���������ӿں͹���ʵ��
class CTestBaseCase
{
public:

    CTestBaseCase( std::string messageStr);

    virtual ~CTestBaseCase();

public:

    /**
    *  @brief  ������������
    */
    virtual void Test() = 0;

    /**
    *  @brief  ִ�в�������
    */
    void Run();

    /**
    *  @brief  ����/��ȡ��������ʧ��ʱ����ʽ
    *  @param  [in] 	testFailureHandle   //  ����ʧ�ܴ���ʽ
    */
    void SetFailureHandle( TestFailureHandle testFailureHandle );
    TestFailureHandle GetFailureHandle( );

    /**
    *  @brief  ��ȡ���Խ��
    */
    TestResult GetTestResult();

    /**
    *  @brief  ��ȡ���Խ��
    */
    std::string GetTestResultInfoStr();

protected:

    std::string                 m_messageStr;           //  ������Ϣ�ַ���
    std::string                 m_describeTion;         //  ����������Ϣ
    TestResult                  m_caseResult;           //  ��������ִ�н��
    TestFailureHandle           m_testFailureHandle;    //  ����ʧ�ܴ���ʽ
    unsigned int                m_subCaseCounts;        //  �����Ӽ���(������Լ���)
};


//  ���Խ��Ŀ�����
//  �ṩ�����Ĳ����ӿڡ�ʵ��
//  ������̳�ʵ�־���Ŀ��ʵ��
class CTestBaseSink
{
public:

    virtual ~CTestBaseSink();

   /**
    *  @brief  ��������Ϣ
    *  @param  [in] 	resultInfo   //  �����Ϣ
    */
    void Write( const std::string & resultInfo );

private:

    CTestBaseSink();

    DISALLOW_COPY_AND_ASSIGN( CTestBaseSink );
};


#define  FUNC_CASE(funcCase) class funcCase: public CTestBaseCase  \
{ \
public:\
    funcCase( std::string caseMessageStr = #funcCase):CTestBaseCase(caseMessageStr)\
    { \
    }; \
    virtual ~funcCase(){}; \
public: \
    virtual void Test(); \
private: \
    static char dummyFun() \
    { \
        CUnitTest::GetInstance()->AddUnitTest( new funcCase( #funcCase ) ); \
        return 0; \
    } \
private: \
    static char  m_dummyStruct; \
}; \
char funcCase::m_dummyStruct = funcCase::dummyFun(); \
void funcCase::Test()

#define ASSERT_IMPL(condition, conditionStr) \
{ \
    ++m_subCaseCounts; \
    if ( !(condition) ) \
    { \
        static char formatFailureStr[] = "\n     [Macro:%d] File: %s Function: %s Line: %d Failed Reason: %s is not true"; \
m_describeTion += StringFormatA( formatFailureStr, m_subCaseCounts, __FILE__, __FUNCTION__, __LINE__, conditionStr); \
        if (m_testFailureHandle == TFH_ABORT) \
        { \
            m_caseResult = TR_ABORT; \
            return; \
        } \
        else \
        { \
            m_caseResult = TR_FAILED; \
        } \
    } \
}

#define EXPECT_IMPL(condition, conditionStr1, conditionStr2, compareStr) \
{ \
    ++m_subCaseCounts; \
    if ( !(condition) ) \
    { \
        static char formatFailureStr[] = "\n     [Macro:%d] File: %s Function: %s Line: %d Failed Reason: %s %s %s"; \
        m_describeTion += StringFormatA( formatFailureStr, m_subCaseCounts, __FILE__, __FUNCTION__, __LINE__, conditionStr1, compareStr, conditionStr2); \
        if (m_testFailureHandle == TFH_ABORT) \
        { \
            m_caseResult = TR_ABORT; \
            return; \
        } \
        else \
        { \
            m_caseResult = TR_FAILED; \
        } \
    } \
}

#define ASSERT_CASE(condition) ASSERT_IMPL(condition, #condition)

#define EXPECT_EQ(condition1, condition2) EXPECT_IMPL( ( (condition1) == (condition2) ), #condition1, #condition2, "is not equal to")

#define EXPECT_NEQ(condition1, condition2) EXPECT_IMPL( ( (condition1) != (condition2) ), #condition1, #condition2, "is equal to")

#define EXPECT_GR(condition1, condition2) EXPECT_IMPL( ( (condition1) > (condition2) ), #condition1, #condition2, "is not greater than")

#define EXPECT_LE(condition1, condition2) EXPECT_IMPL( ( (condition1) < (condition2) ), #condition1, #condition2, "is not less than")

#define EXPECT_GRE(condition1, condition2) EXPECT_IMPL( ( (condition1) >= (condition2) ), #condition1, #condition2, "is not greater than or equal to")

#define EXPECT_LEE(condition1, condition2) EXPECT_IMPL( ( (condition1) <= (condition2) ), #condition1, #condition2, "is not less than or equal to")

#endif  //  _UNIT_TEST_H_