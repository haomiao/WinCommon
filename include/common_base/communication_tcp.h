///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_tcp.h
/// 	@brief	TCPͨ��. 
/// 
///		��Ҫʵ�ּ򵥵�TCP�ͻ���ͨ�Žӿ�ʵ��  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/14  15:32:31
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _COMMOUNICATION_TCP_H_
#define     _COMMOUNICATION_TCP_H_

#include <WinSock2.h>
#include "communication_base.h"


/* ������� */
enum COMMON_TCP_CONFIG
{
    TCP_CFG_SENDTIME = 0,           //  ���ͳ�ʱ(param:int)(ms)
    TCP_CFG_RECVTIME,               //  ���ճ�ʱ(param:int)(ms)
    TCP_CFG_LINGER,                 //  ���ùر��ӳ�(param:linger)
    TCP_CFG_NODELAY,                //  �������ӳ�(param:int)
    TCP_CFG_KEEPALIVE,              //  ���ñ���(param:int)

};


//  TCPͨ��
//  Ĭ�����ӳ�ʱ:3s
//  Ĭ�Ϸ��ͳ�ʱ:3000ms
//  Ĭ�Ͻ��ճ�ʱ:3000ms
//  Ĭ�Ϲر��ӳ�:{1,3}
//  Ĭ�����ӳ�:1
//  Ĭ�ϱ���:0
class TcpComm : public ICommBase
{
public:

	TcpComm();

	virtual ~TcpComm();

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

};  //  class TcpComm


#endif		//_COMMOUNICATION_TCP_H_