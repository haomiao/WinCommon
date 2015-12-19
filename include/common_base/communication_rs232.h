///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_rs232.h
/// 	@brief	RS232ͨ��. 
/// 
///		��Ҫʵ�ּ򵥵�RS232�Ӷ�ͨ�Žӿ�ʵ��  
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

/* ������� */
enum COMMON_RS232_CONFIG
{
    RS232_CFG_INPUTBUFFER = 0,          //  ���뻺����(param:DWORD)(bytes)
    RS232_CFG_OUTPUTBUFFER,             //  ���������(param:DWORD)(bytes)
    RS232_CFG_TIMEOUTS,                 //  ͨ�ų�ʱ(param:COMMTIMEOUTS)
    RS232_CFG_PURE,                     //  ��ձ�ʶ(param:DWORD)

};


//  RS232ͨ��
//  Ĭ�����뻺����:2 * 1024 * 1024
//  Ĭ�����������:2 * 1024 * 1024
//  Ĭ��ͨ�ų�ʱ:{ 0xFFFFFFFF, 0 , 100, 0, 50}
//  Ĭ�Ͼ�������ձ�ʶ:PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
class RS232Comm :public ICommBase
{

public:

	RS232Comm();

	~RS232Comm();

public:

	/**
	* @brief:   	����RS232��������
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
	* @brief:   	��ʼ������	
	*/
	BOOL SetupPort();

	/**
	* @brief:   	���ô���״̬
	* @param[in]:	unsigned long baudRate;			//	������
	* @param[in]:	unsigned char byteSize;			//	����λ
	* @param[in]:	unsigned char parity;			//	У��λ
	* @param[in]:	unsigned char stopBits,			//	ֹͣλ
	* @param[in]:	CONTROL		  ctrl;				//	���ط�ʽ
	*/
	BOOL SetState(	unsigned long dwBaudRate, 
		unsigned char dwByteSize	= 8, 
		unsigned char dwParity		= NOPARITY, 
		unsigned char dwStopBits	= ONESTOPBIT,
		RS232_CONTROL ctrl			= RS232_NO_C );

private:

	HANDLE		m_comHandle;						//	���ڲ������

private:

    DWORD           m_inputBufferSize;              //  �������뻺����(bytes)
    DWORD           m_outputBufferSize;             //  �������������(bytes)
    COMMTIMEOUTS    m_commTimeouts;                 //  ����ͨ�ų�ʱ
    DWORD           m_pureFlags;                    //  ���þ�������ձ�ʶ
};

#endif	//	_COMMOUNICATION_RS232_H_