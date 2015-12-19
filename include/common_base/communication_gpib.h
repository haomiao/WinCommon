///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   communication_gpib.h
/// 	@brief	GPIBͨ��. 
/// 
///		��Ҫʵ�ּ򵥵�GPIB�Ӷ�ͨ�Žӿ�ʵ��  
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


/* ������� */
enum COMMON_GPIB_CONFIG
{
    GPIB_CFG_TIME_OUT = 0,           //  ��ʱ(param:int)(ms)

};


//  GPIBͨ��
//  Ĭ�����ӳ�ʱ:T3s
class GpibComm :public ICommBase
{
public:

	GpibComm();

	~GpibComm();

public:

    /**
    * @brief:   	����GPIB����
    * @param[in]:	arg							������豸��ַ
    * @return:		int							������������״̬
    */
    virtual int Open( const void* arg );

    /**
    * @brief:   	�ر�����
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

	/*
	\brief  ��GPIB�豸
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
	\brief   ִ�й�������һ�������Ƿ��д�
	\return  bool  true   ִ���д�
	false  ִ����ȷ
	*/
	bool hasError();

private:

	int					            m_hHandle;		//  �豸���

    static CCriticalSectionLock	    m_lock;			//  �������

private:

    int                             m_timeout;      //  ���ӳ�ʱ(s)

};  //  class GpibComm


#endif  //  _COMMOUNICATION_GPIB_H_
