///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_gpib.h
/// 	@brief	GPIB通信. 
/// 
///		主要实现简单的GPIB从端通信接口实现  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/14  17:43:29
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _COMMOUNICATION_GPIB_H_
#define     _COMMOUNICATION_GPIB_H_

#include <string>
#include "communication_base.h"
#include "lock_critical_section.h"

#ifdef	_cplusplus
extern "C" {
#endif
#include "./GPIBDevice/ni488.h"
#ifdef _cplusplus
}
#endif


/* 配置相关 */
enum COMMON_GPIB_CONFIG
{
    GPIB_CFG_TIME_OUT = 0,           //  超时(param:int)(ms)

};


//  GPIB通信
//  默认连接超时:T3s
class GpibComm :public ICommBase
{
public:

	GpibComm();

	~GpibComm();

public:

    /**
    * @brief:   	建立GPIB连接
    * @param[in]:	arg							网络或设备地址
    * @return:		int							返回网络连接状态
    */
    virtual int Open( const void* arg );

    /**
    * @brief:   	关闭连接
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

	/*
	\brief  打开GPIB设备
	\param  iIndex     Board Index (GPIB0 = 0, GPIB1 = 1, ...)
	MainAddrr  Device primary address
	SlaveAdd   Device secondary address
	TimeOut    Timeout setting (T10s = 10 seconds)
	eot        Assert EOI line at end of write
	eos        EOS termination mode
	\return 
	*/
	int	OpenDevice(int iIndex, int MainAddr, int SlaveAddr = 0, int TimeOut = T10s, int eot = 1, int eos = 0);


	/*
	\brief  Take the device offline
	*/
	int CloseDevice(int value);


	/*
	\brief  Clear the device
	\return 
	*/
	int	GPIBClear();


	/*
	\brief   执行过程中上一步操作是否有错
	\return  bool  true   执行有错
	false  执行正确
	*/
	bool hasError();

private:

	int					            m_hHandle;		//  设备句柄

    static CCriticalSectionLock	    m_lock;			//  命令加锁

private:

    int                             m_timeout;      //  连接超时(s)

};  //  class GpibComm


#endif  //  _COMMOUNICATION_GPIB_H_
