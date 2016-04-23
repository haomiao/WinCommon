///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   debug_assist.h
/// 	@brief	调试助手. 
/// 
///		主要实现基于空目标、文件目标、控制台目标、网络等目标，实现调试信息记录或定向输出  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/06  16:13:06
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _DEBUG_ASSIST_H_
#define     _DEBUG_ASSIST_H_

#include<string>
#include "config.h"
#include "output_sink.h"

//  调试级别
enum DebugLevel
{
    DL_FORBID = 0,                  //  禁止调试
    DL_ERROR_EXCEPTION,             //  错误信息或异常
    DL_OPERATOR,                    //  操作流程、细节描述
    DL_USERINFO,                    //  用户信息
    DL_ALL,                         //  所有信息，内部状态
};


//  调试助手，提供外部使用的简单使用操作
//  一般情况下为单例模式
//  此外若需要，可使用工厂模式创建设置目标输出对象
class CDebugAssist
{
public:

    ~CDebugAssist();

public:

    static CDebugAssist * GetInstance();

    /**
    *  @brief  设置目标输出对象
    *  @param  [in] 	pDebugSink         调试信息输出目标对象
    */
    void SetSink( COutPutBaseSink * pDebugSink );

    /**
    *  @brief  设置/获取调试输出级别
    *  @param  [in] 	debugLevel         调试输出级别
    */
    void SetDebugLevel( DebugLevel debugLevel );
    DebugLevel GetDebugLevel();

    /**
    *  @brief  调试信息输出定向(每调用Write则将调试信息换行输出)
    *  @param  [in] 	debugInfoStr          调试信息串
    *  @param  [in] 	debugInfoC            单个调试信息字符
    *  @param  [in] 	curDebugLevel         当前调试级别(若小于m_debugLevel，则不输出调试信息)
    */
    void Write( const std::string & debugInfoStr, DebugLevel curDebugLevel =  DL_ALL);
    void Write( const char debugInfoC, DebugLevel curDebugLevel = DL_ALL );

    /**
    *  @brief  刷新缓冲区，调试信息输出
    */
    void Flush();

private:

    CDebugAssist();

    DISALLOW_COPY_AND_ASSIGN( CDebugAssist );

private:

    COutPutBaseSink *   m_pDebugSink;   //  调试输出目标对象
    DebugLevel          m_debugLevel;   //  调试级别
};

#endif  //  _DEBUG_ASSIST_H_