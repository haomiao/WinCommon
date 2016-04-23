///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   unit_test.h
/// 	@brief	单元测试. 
/// 
///		主要实现简易的单元测试，提供实用、简易的测试操作  
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


//  用例测试结果
enum TestResult
{
    TR_SUCCESSED,       //  测试完成并通过测试
    TR_FAILED,          //  测试完成且失败,出现异常
    TR_ABORT,           //  测试未完成且失败
};


//  测试失败处理方式
enum TestFailureHandle
{
    TFH_CONTINUE,       //  测试失败时，继续进行测试
    TFH_ABORT,          //  测试失败时，立即终止剩余测试
};


//  测试结果统计信息
struct TestResultInfo
{
    unsigned int totalCounts;       //  测试总计数
    unsigned int successCounts;     //  测试结果成功计数
    unsigned int failedCounts;      //  测试结果失败计数
    bool         isFinished;        //  测试是否完成
};


//  前向声明
class CTestBaseCase;

//  单元测试
//  单例实现，外部统一使用该类实现单元测试
//  默认输出为控制台,可修改输出目标
class CUnitTest
{
public:

    virtual ~CUnitTest();

    static CUnitTest * GetInstance();

public:

    /**
    *  @brief  执行所有的测试用例
    */
    void RunAllTests();

    /**
    *  @brief  添加单元测试
    *  @param  [in] 	pTestBase           //  测试用例对象
    */
    void AddUnitTest( CTestBaseCase *pTestBase );

    /**
    *  @brief  设置重复测试次数(至少测试一次)
    *  @param  [in] 	repeatCounts        //  重复次数
    */
    void SetRepeatCouns( unsigned int repeatCounts );

    /**
    *  @brief  设置测试用例失败时处理方式
    *  @param  [in] 	testFailureHandle   //  测试失败处理方式
    */
    void SetFailureHandle( TestFailureHandle testFailureHandle );

    /**
    *  @brief  设置测试结果输出目标
    *  @param  [in] 	pOutPutSink        //  输出目标
    */
    void SetResultSink( COutPutBaseSink *pOutPutSink );

private:

    /**
    *  @brief  测试结果输出目标
    */
    void OutPutResultToSink( CTestBaseCase *pTestBaseCase);
    void OutPutTotalResultInfoToSink();

private:

    CUnitTest();

    DISALLOW_COPY_AND_ASSIGN( CUnitTest );

private:

    std::list<CTestBaseCase *>  m_testBaseList;         //  测试用例列表对象
    unsigned int                m_repeatCounts;         //  重复测试次数
    COutPutBaseSink *           m_pOutPutSink;          //  测试结果输出目标
    TestResultInfo              m_testResultInfo;       //  测试结果统计信息
};



//  单元测试用例基类
//  提供基本操作接口和公共实现
class CTestBaseCase
{
public:

    CTestBaseCase( std::string messageStr);

    virtual ~CTestBaseCase();

public:

    /**
    *  @brief  测试用例内容
    */
    virtual void Test() = 0;

    /**
    *  @brief  执行测试用例
    */
    void Run();

    /**
    *  @brief  设置/获取测试用例失败时处理方式
    *  @param  [in] 	testFailureHandle   //  测试失败处理方式
    */
    void SetFailureHandle( TestFailureHandle testFailureHandle );
    TestFailureHandle GetFailureHandle( );

    /**
    *  @brief  获取测试结果
    */
    TestResult GetTestResult();

    /**
    *  @brief  获取测试结果
    */
    std::string GetTestResultInfoStr();

protected:

    std::string                 m_messageStr;           //  测试消息字符串
    std::string                 m_describeTion;         //  测试描述信息
    TestResult                  m_caseResult;           //  测试用例执行结果
    TestFailureHandle           m_testFailureHandle;    //  测试失败处理方式
    unsigned int                m_subCaseCounts;        //  测试子集数(即宏测试计数)
};


//  测试结果目标基类
//  提供基本的操作接口、实现
//  由子类继承实现具体目标实现
class CTestBaseSink
{
public:

    virtual ~CTestBaseSink();

   /**
    *  @brief  输出结果信息
    *  @param  [in] 	resultInfo   //  结果信息
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