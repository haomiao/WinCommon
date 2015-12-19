///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_udp.h
/// 	@brief	UDPͨ��. 
/// 
///		��Ҫʵ�ּ򵥵�UDP�ͻ���ͨ�Žӿ�ʵ��  
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


/* ������� */
enum COMMON_UDP_CONFIG
{
    UDP_CFG_SENDTIME = 0,           //  ���ͳ�ʱ(param:int)(ms)
    UDP_CFG_RECVTIME,               //  ���ճ�ʱ(param:int)(ms)
    UDP_CFG_BOARDCAST,              //  ���ù㲥(param:bool)
    UDP_CFG_REUSEADR,               //  ���ÿ����õ�ַ(param:bool)
};


//  UDPͨ��(�ڲ�ʹ��connect���ӷ�����,��������������������)
//  Ĭ�����ӳ�ʱ:3s
//  Ĭ�Ϸ��ͳ�ʱ:3000ms
//  Ĭ�Ͻ��ճ�ʱ:5000ms
//  Ĭ�Ϲ㲥:false
//  Ĭ�Ͽ����õ�ַ:true
class UdpComm : public ICommBase
{
public:

    UdpComm();

    virtual ~UdpComm();

public:

	/**
	* @brief:   	����TCP��������
	* @param[in]:	arg							������豸��ַ 
	* @return:		int							������������״̬
	*/
    virtual int Open( const void* arg );

    /**
    * @brief:   	�ر���������
    * @return:  	int							���عر�����״̬
    */
    virtual int Close();

	/**
	* @brief:   	��ȡ��������
	* @param[in]:   void *buffer				�������ݻ�����
	* @param[int]:  int len						��������ֽ���
	* @return:		int							����ʵ�������ֽ���
	*/											//ʧ�ܷ������Ӵ�����
	virtual int Read(void* buffer, int len);

	/**
	* @brief:   	������������
	* @param[in]:   const void * buffer			�������ݻ�����
	* @param[int]:  int len						���������ֽ���
	* @return:  	int							���ط���״̬
	*/
	virtual int Write(const void* buffer, int len);

	/**
	* @brief:   	����������ز���
	* @param[in]:   int code					������
	* @param[out]:  void * param				����ֵ
	* @return:  	int							��������״̬
	*/
	virtual int Config(int code, void* param);

private:

	/**
	* @brief:   	���ӷ�����
	* @param[in]:   SOCKET &socket				�׽���
	* @param[out]:  serverAddr					��������ַ
	* @return:  	int							��������״̬
	*/
	int ConnectSever( SOCKET &socket, const struct sockaddr * serverAddr );

    /**
    * @brief:   	Ĭ��������ز���
    */
    int SetDefaultCommParameters();

private:

	SOCKET          m_socket;                   //  ���������׽���
	int             m_timeout;                  //  ���糬ʱ(s)

private:

    int             m_timeWaitRecv;             //  ���ݽ��ճ�ʱ(ms)
    int             m_timeWaitSend;             //  ���ݷ��ͳ�ʱ(ms)
    struct linger   m_linger;                   //  �ӳ��ͷ�
    int             m_nodelay;                  //  ����ʱ
    int             m_keepAlive;                //  ����
    bool            m_boardCast;                //  �Ƿ�㲥
    bool            m_reuseAddr;                //  �Ƿ����õ�ַ

};  //  class UdpComm


#endif		//  _COMMOUNICATION_UDP_H_