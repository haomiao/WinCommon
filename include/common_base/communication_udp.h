///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_udp.h
/// 	@brief	UDP通信. 
/// 
///		主要实现简单的UDP客户端通信接口实现  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/16  10:04:31
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _COMMOUNICATION_UDP_H_
#define     _COMMOUNICATION_UDP_H_

#include <WinSock2.h>
#include "communication_base.h"


/* 配置相关 */
enum COMMON_UDP_CONFIG
{
    UDP_CFG_SENDTIME = 0,           //  发送超时(param:int)(ms)
    UDP_CFG_RECVTIME,               //  接收超时(param:int)(ms)
    UDP_CFG_BOARDCAST,              //  设置广播(param:bool)
    UDP_CFG_REUSEADR,               //  设置可重用地址(param:bool)
};


//  UDP通信(内部使用connect连接服务器,不接受其他发来的数据)
//  默认连接超时:3s
//  默认发送超时:3000ms
//  默认接收超时:5000ms
//  默认广播:false
//  默认可重用地址:true
class UdpComm : public ICommBase
{
public:

    UdpComm();

    virtual ~UdpComm();

public:

	/**
	* @brief:   	建立TCP网络连接
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
	* @brief:   	连接服务器
	* @param[in]:   SOCKET &socket				套接字
	* @param[out]:  serverAddr					服务器地址
	* @return:  	int							返回连接状态
	*/
	int ConnectSever( SOCKET &socket, const struct sockaddr * serverAddr );

    /**
    * @brief:   	默认配置相关参数
    */
    int SetDefaultCommParameters();

private:

	SOCKET          m_socket;                   //  网络连接套接字
	int             m_timeout;                  //  网络超时(s)

private:

    int             m_timeWaitRecv;             //  数据接收超时(ms)
    int             m_timeWaitSend;             //  数据发送超时(ms)
    struct linger   m_linger;                   //  延迟释放
    int             m_nodelay;                  //  无延时
    int             m_keepAlive;                //  保活
    bool            m_boardCast;                //  是否广播
    bool            m_reuseAddr;                //  是否重用地址

};  //  class UdpComm


#endif		//  _COMMOUNICATION_UDP_H_