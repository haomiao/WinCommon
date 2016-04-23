///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   output_sink.h
/// 	@brief	输出目标. 
/// 
///		主要实现基本的输出目标实现、提供多种简易的输出目标实现  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/11  12:06:00
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _OUT_PUT_SINK_H_
#define     _OUT_PUT_SINK_H_

#include <iostream>
#include <string>
#include <fstream>
#include <WinSock2.h>
#include "config.h"

#pragma comment(lib, "Ws2_32.lib")

//  输出目标基类
//  主要提供基本的输出信息操作
//  子类可根据需要继承、重写实现自定义操作
class COutPutBaseSink
{
public:

    COutPutBaseSink();

    virtual ~COutPutBaseSink();

    /**
    *  @brief  输出信息输出定向(每调用Write则将输出信息换行输出)
    *  @param  [in] 	outPutInfoStr          输出信息串
    *  @param  [in] 	outPutInfoC            单个输出信息字符
    *  @param  [in] 	outPutInfo         输出信息
    *  @param  [in] 	len               输出信息长度
    */
    virtual void Write( const std::string & outPutInfoStr ) = 0;
    virtual void Write( const char outPutInfoC ) = 0;
    virtual void Write( const void* outPutInfo, int len ) = 0;

    /**
    *  @brief  刷新缓冲区，输出信息输出
    */
    virtual void Flush();

    /**
    *  @brief  获取输出信息头
    */
    std::string GetOutPutHeaderInfo();

    /**
    *  @brief  设置/查询输出信息头输出标识
    *  @param  [in] 	outPutHeaderFlag       输出信息输出标识(true:输出输出信息头,false:不输出)
    */
    void SetRecordOutPutHeader( bool outPutHeaderFlag = true );
    bool IsRecordOutPutHeader();

protected:

    /**
    *  @brief  设置用户自定义输出信息头
    *  @param  [in] 	outPutHeaderInfoStr    输出信息头串
    *  @detail:         默认情况下为标准的时间信息串作为输出信息头;若需自定义，则继承该类并重写该函数即可
    */
    virtual void SetCustomOutPutHeaderInfo( const std::string & outPutHeaderInfoStr );

private:

    /**
    *  @brief  获取标准的输出信息头
    *  @return 返回信息头
    */
    std::string GetStandardHeaderInfo();

private:

    std::string     m_customOutPutHeaderInfo;    //  输出头信息，一般为当前输出记录时间点或其他标识
    bool            m_bOutPutHeaderFlag;         //  是否输出输出信息头
};


//  空目标输出
class COutPutNoneSink : public COutPutBaseSink
{
public:

    virtual ~COutPutNoneSink();

public:

    static COutPutNoneSink * GetInstance();

    virtual void Write( const std::string & outPutInfoStr );

    virtual void Write( const char outPutInfoC );

    virtual void Write( const void* outPutInfo, int len );

private:

    COutPutNoneSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutNoneSink );
};


//  控制台目标输出
class COutPutConsoleSink : public COutPutBaseSink
{
public:

    virtual ~COutPutConsoleSink();

public:

    static COutPutConsoleSink * GetInstance();

    virtual void Write( const std::string & outPutInfoStr );

    virtual void Write( const char outPutInfoC );

    virtual void Write( const void* outPutInfo, int len );

    virtual void Flush();

private:

    COutPutConsoleSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutConsoleSink );
};


//  文件目标输出
class COutPutFileSink : public COutPutBaseSink
{
public:

    virtual ~COutPutFileSink();

public:

    static COutPutFileSink * GetInstance();

    virtual void Write( const std::string & outPutInfoStr );

    virtual void Write( const char outPutInfoC );

    virtual void Write( const void* outPutInfo, int len );

    virtual void Flush();

public:

    /**
    *  @brief  设置输出信息文件输出路径
    *  @param  [in] 	filePath            输出信息文件输出路径
    *  @detail:         若filePath为nullptr，则将默认输出至控制台，效果将同COutPutConsoleSink
    */
    void SetOutPutFilePath( const char * filePath = nullptr );

private:

    /**
    *  @brief  打开文件
    */
    void OpenFile();

    /**
    *  @brief  安全关闭文件
    */
    void SafeCloseFile();

private:

    COutPutFileSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutFileSink );

private:

    std::ofstream       m_fstream;          //  文件输出流对象
    std::string         m_outPutFilePath;    //  输出信息文件输出路径
};


//  网络目标输出
//  当然可结合以太网(TCP/UDP等)、USB、GPIB、RS232等方式
//  若扩展，可参照类似于以太网方式实现或重构采用其他方式实现
//  目前网络输出暂时采用同步方式，当然可使用异步或其他通信模式实现(如基于事件、select、完成端口等)
class COutPutTcpSink : public COutPutBaseSink
{
public:

    virtual ~COutPutTcpSink();

public:

    static COutPutTcpSink * GetInstance();

    virtual void Write( const std::string & outPutInfoStr );

    virtual void Write( const char outPutInfoC );

    virtual void Write( const void* outPutInfo, int len );

public:

    /**
    *  @brief  设置输出信息输出目标地址
    *  @param  [in] 	ipAddress          目标主机IP地址
    *  @param  [in] 	port               目标主机端口
    */
    void SetOutPutTarget( const char * ipAddress, unsigned short port );

private:

    /**
    * @brief:  默认配置相关参数
    */
    void SetDefaultCommParameters();

    /**
    *  @brief  连接远程目标
    */
    void ConnectTarget();

    /**
    *  @brief  安全断开远程目标
    */
    void SafeCloseConnect();

private:

    COutPutTcpSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutTcpSink );

private:

    std::string     m_ipAddress;    //  目标主机IP地址
    unsigned short  m_port;         //  目标主机端口
    SOCKET          m_socket;       //  套接字句柄
    int             m_timeWaitSend; //  发送数据超时
    bool            m_isNetValid;   //  网络组件是否可用
};


#endif  //  _OUT_PUT_SINK_H_