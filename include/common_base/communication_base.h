///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_base.h
/// 	@brief	通信基类--客户端. 
/// 
///		提供基础通信简单实现  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/14  15:08:46
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _COMMOUNICATION_BASE_H_
#define     _COMMOUNICATION_BASE_H_

#include <string>

/* 通信类型 */
//  目前仅提供以太网、GPIB、RS232、USB、UDP通信方式
//  GPIB需安装NI驱动
enum BASE_COMM_TYPE
{
    ETHERNET_COMM = 0,          //  以太网通信
    GPIB_COMM,                  //  GPIB通信
    RS232_COMM,                 //  RS232通信
    USB_COMM,                   //  USB通信
    UDP_COMM,                   //  UDP通信
};


/* 网络连接状态 */
//  返回其他错误，则是通过WSAGetLastError获取;
//  具体错误信息，请查看Sockets Error Codes,或通过GetSystemErrorDesc获取
enum  COMMUNICATION_STATE
{
    COMM_SUCCESS = 0,           //  网络正常
    COMM_FAILURE = -1,          //  网络数据异常
    COMM_TIMEOUT = -2,          //  网络超时(数据接收、发送、连接)
    COMM_PARAM_ERR = -3,        //  参数错误
    COMM_UNKNOWN_ERR = -4,      //  未知错误
};


/* ENET参数 */
struct EtherCommArg
{
    std::string     ip;             //  IP地址
    unsigned int    port;           //  端口号
};


/* GPIB参数 */
struct GpibCommArg
{
    unsigned int BoardIndex;
    unsigned int PrimaryAddr;
    unsigned int SecondAddr;
    unsigned int TimeOut;
    unsigned int Eot;
    unsigned int Eos;
};


/* RS232通信参数 */

/* 流控方式 */
enum RS232_CONTROL
{
    RS232_RTS = 0,					//	硬件流控
    RS232_DTR,						//	软件流控	
    RS232_NO_C,						//	无流控
    RS232_DEFAULT					//	默认方式
};

struct RS232CommArg
{
    unsigned int  comNum;			//	COM串口号
    unsigned long baudRate;			//	波特率
    unsigned char byteSize;			//	数据位
    unsigned char parity;			//	校验位
    unsigned char stopBits;			//	停止位
    RS232_CONTROL ctrl;				//	流控方式
};


/* USB通信参数 */
struct USBCommArg
{
    int                claimNum;     // USB声明接口号
    unsigned short int vendorID;     // USB供应商ID
    unsigned short int productID;    // USB产品ID
};


//////////////////////////////////////////////////////////////////////////


//  通信基类，提供对外的简单接口调用
//  仅支持简单的阻塞、同步，此外需要结合应用层一起处理数据粘包、拆包等操作
class ICommBase
{

public:

    /**
    * @brief:   	建立连接
    * @param[in]:	arg							网络或设备地址(EtherCommArg/GpibCommArg/
    *                                           RS232CommArg)
    * @return:		int							返回网络连接状态
    */
    virtual int Open( const void* arg ) = 0;

    /**
    * @brief:   	关闭连接
    * @return:  	int							返回关闭连接状态
    */
    virtual int Close() = 0;

    /**
    * @brief:   	读取数据
    * @param[in]:   void *buffer				接收数据缓冲区
    * @param[int]:  int len						接收的数据最大字节数
    * @return:		int							返回实际数据字节数
    * @detail:                                  不保证获取到所有数据，可能需要结合应用层协议多次读取
    */
    virtual int Read( void* buffer, int len ) = 0;

    /**
    * @brief:   	发送数据
    * @param[in]:   const void * buffer			发送数据缓冲区
    * @param[int]:  int len						发送数据字节数
    * @return:  	int							返回发送状态
    * @detail:                                  其会保证发送所有数据到socket发送缓冲区
    */
    virtual int Write( const void* buffer, int len) = 0;

    /**
    * @brief:   	配置通信相关参数
    * @param[in]:   int code					配置码
    * @param[out]:  void * param				参数值
    * @return:  	int							返回配置状态
    */
    virtual int Config( int code, void* param ) = 0;

protected:

    ICommBase()
    {

    }

    virtual ~ICommBase()
    {

    }

};  //    class ICommBase

#endif  //  _COMMOUNICATION_BASE_H_