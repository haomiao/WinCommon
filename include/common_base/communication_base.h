///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_base.h
/// 	@brief	ͨ�Ż���--�ͻ���. 
/// 
///		�ṩ����ͨ�ż�ʵ��  
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

/* ͨ������ */
//  Ŀǰ���ṩ��̫����GPIB��RS232��USB��UDPͨ�ŷ�ʽ
//  GPIB�谲װNI����
enum BASE_COMM_TYPE
{
    ETHERNET_COMM = 0,          //  ��̫��ͨ��
    GPIB_COMM,                  //  GPIBͨ��
    RS232_COMM,                 //  RS232ͨ��
    USB_COMM,                   //  USBͨ��
    UDP_COMM,                   //  UDPͨ��
};


/* ��������״̬ */
//  ����������������ͨ��WSAGetLastError��ȡ;
//  ���������Ϣ����鿴Sockets Error Codes,��ͨ��GetSystemErrorDesc��ȡ
enum  COMMUNICATION_STATE
{
    COMM_SUCCESS = 0,           //  ��������
    COMM_FAILURE = -1,          //  ���������쳣
    COMM_TIMEOUT = -2,          //  ���糬ʱ(���ݽ��ա����͡�����)
    COMM_PARAM_ERR = -3,        //  ��������
    COMM_UNKNOWN_ERR = -4,      //  δ֪����
};


/* ENET���� */
struct EtherCommArg
{
    std::string     ip;             //  IP��ַ
    unsigned int    port;           //  �˿ں�
};


/* GPIB���� */
struct GpibCommArg
{
    unsigned int BoardIndex;
    unsigned int PrimaryAddr;
    unsigned int SecondAddr;
    unsigned int TimeOut;
    unsigned int Eot;
    unsigned int Eos;
};


/* RS232ͨ�Ų��� */

/* ���ط�ʽ */
enum RS232_CONTROL
{
    RS232_RTS = 0,					//	Ӳ������
    RS232_DTR,						//	�������	
    RS232_NO_C,						//	������
    RS232_DEFAULT					//	Ĭ�Ϸ�ʽ
};

struct RS232CommArg
{
    unsigned int  comNum;			//	COM���ں�
    unsigned long baudRate;			//	������
    unsigned char byteSize;			//	����λ
    unsigned char parity;			//	У��λ
    unsigned char stopBits;			//	ֹͣλ
    RS232_CONTROL ctrl;				//	���ط�ʽ
};


/* USBͨ�Ų��� */
struct USBCommArg
{
    int                claimNum;     // USB�����ӿں�
    unsigned short int vendorID;     // USB��Ӧ��ID
    unsigned short int productID;    // USB��ƷID
};


//////////////////////////////////////////////////////////////////////////


//  ͨ�Ż��࣬�ṩ����ļ򵥽ӿڵ���
//  ��֧�ּ򵥵�������ͬ����������Ҫ���Ӧ�ò�һ��������ճ��������Ȳ���
class ICommBase
{

public:

    /**
    * @brief:   	��������
    * @param[in]:	arg							������豸��ַ(EtherCommArg/GpibCommArg/
    *                                           RS232CommArg)
    * @return:		int							������������״̬
    */
    virtual int Open( const void* arg ) = 0;

    /**
    * @brief:   	�ر�����
    * @return:  	int							���عر�����״̬
    */
    virtual int Close() = 0;

    /**
    * @brief:   	��ȡ����
    * @param[in]:   void *buffer				�������ݻ�����
    * @param[int]:  int len						���յ���������ֽ���
    * @return:		int							����ʵ�������ֽ���
    * @detail:                                  ����֤��ȡ���������ݣ�������Ҫ���Ӧ�ò�Э���ζ�ȡ
    */
    virtual int Read( void* buffer, int len ) = 0;

    /**
    * @brief:   	��������
    * @param[in]:   const void * buffer			�������ݻ�����
    * @param[int]:  int len						���������ֽ���
    * @return:  	int							���ط���״̬
    * @detail:                                  ��ᱣ֤�����������ݵ�socket���ͻ�����
    */
    virtual int Write( const void* buffer, int len) = 0;

    /**
    * @brief:   	����ͨ����ز���
    * @param[in]:   int code					������
    * @param[out]:  void * param				����ֵ
    * @return:  	int							��������״̬
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