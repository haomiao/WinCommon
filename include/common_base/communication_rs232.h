///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_rs232.h
/// 	@brief	RS232通信. 
/// 
///		主要实现简单的RS232从端通信接口实现  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/14  16:56:15
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _COMMOUNICATION_RS232_H_
#define     _COMMOUNICATION_RS232_H_

#include "communication_base.h"

/* 配置相关 */
enum COMMON_RS232_CONFIG
{
    RS232_CFG_INPUTBUFFER = 0,          //  输入缓冲区(param:DWORD)(bytes)
    RS232_CFG_OUTPUTBUFFER,             //  输出缓冲区(param:DWORD)(bytes)
    RS232_CFG_TIMEOUTS,                 //  通信超时(param:COMMTIMEOUTS)
    RS232_CFG_PURE,                     //  清空标识(param:DWORD)

};


//  RS232通信
//  默认输入缓冲区:2 * 1024 * 1024
//  默认输出缓冲区:2 * 1024 * 1024
//  默认通信超时:{ 0xFFFFFFFF, 0 , 100, 0, 50}
//  默认净化、清空标识:PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
class RS232Comm :public ICommBase
{

public:

	RS232Comm();

	~RS232Comm();

public:

	/**
	* @brief:   	建立RS232串口连接
	* @param[in]:	arg							网络或设备地址 
	* @return:		int							返回网络连接状态
	*/
    virtual int Open( const void* arg );

    /**
    * @brief:   	关闭网络连接
    * @return:  	int							返回关闭连接状态
    */
    virtual int Close();

	/**
	* @brief:   	读取网络数据
	* @param[in]:   void *buffer				接收数据缓冲区
	* @param[int]:  int len						数据最大字节数
	* @return:		int							返回实际数据字节数
	*/											//失败返回连接错误码
	virtual int Read(void* buffer, int len);

	/**
	* @brief:   	发送网络数据
	* @param[in]:   const void * buffer			发送数据缓冲区
	* @param[int]:  int len						发送数据字节数
	* @return:  	int							返回发送状态
	*/
	virtual int Write(const void* buffer, int len);

	/**
	* @brief:   	配置网络相关参数
	* @param[in]:   int code					配置码
	* @param[out]:  void * param				参数值
	* @return:  	int							返回配置状态
	*/
	virtual int Config(int code, void* param);

private:

	/**
	* @brief:   	初始化串口	
	*/
	BOOL SetupPort();

	/**
	* @brief:   	设置串口状态
	* @param[in]:	unsigned long baudRate;			//	波特率
	* @param[in]:	unsigned char byteSize;			//	数据位
	* @param[in]:	unsigned char parity;			//	校验位
	* @param[in]:	unsigned char stopBits,			//	停止位
	* @param[in]:	CONTROL		  ctrl;				//	流控方式
	*/
	BOOL SetState(	unsigned long dwBaudRate, 
		unsigned char dwByteSize	= 8, 
		unsigned char dwParity		= NOPARITY, 
		unsigned char dwStopBits	= ONESTOPBIT,
		RS232_CONTROL ctrl			= RS232_NO_C );

private:

	HANDLE		m_comHandle;						//	串口操作句柄

private:

    DWORD           m_inputBufferSize;              //  设置输入缓冲区(bytes)
    DWORD           m_outputBufferSize;             //  设置输出缓冲区(bytes)
    COMMTIMEOUTS    m_commTimeouts;                 //  设置通信超时
    DWORD           m_pureFlags;                    //  设置净化、清空标识
};

#endif	//	_COMMOUNICATION_RS232_H_