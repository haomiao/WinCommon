///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   output_sink.h
/// 	@brief	���Ŀ��. 
/// 
///		��Ҫʵ�ֻ��������Ŀ��ʵ�֡��ṩ���ּ��׵����Ŀ��ʵ��  
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

//  ���Ŀ�����
//  ��Ҫ�ṩ�����������Ϣ����
//  ����ɸ�����Ҫ�̳С���дʵ���Զ������
class COutPutBaseSink
{
public:

    COutPutBaseSink();

    virtual ~COutPutBaseSink();

    /**
    *  @brief  �����Ϣ�������(ÿ����Write�������Ϣ�������)
    *  @param  [in] 	outPutInfoStr          �����Ϣ��
    *  @param  [in] 	outPutInfoC            ���������Ϣ�ַ�
    *  @param  [in] 	outPutInfo         �����Ϣ
    *  @param  [in] 	len               �����Ϣ����
    */
    virtual void Write( const std::string & outPutInfoStr ) = 0;
    virtual void Write( const char outPutInfoC ) = 0;
    virtual void Write( const void* outPutInfo, int len ) = 0;

    /**
    *  @brief  ˢ�»������������Ϣ���
    */
    virtual void Flush();

    /**
    *  @brief  ��ȡ�����Ϣͷ
    */
    std::string GetOutPutHeaderInfo();

    /**
    *  @brief  ����/��ѯ�����Ϣͷ�����ʶ
    *  @param  [in] 	outPutHeaderFlag       �����Ϣ�����ʶ(true:��������Ϣͷ,false:�����)
    */
    void SetRecordOutPutHeader( bool outPutHeaderFlag = true );
    bool IsRecordOutPutHeader();

protected:

    /**
    *  @brief  �����û��Զ��������Ϣͷ
    *  @param  [in] 	outPutHeaderInfoStr    �����Ϣͷ��
    *  @detail:         Ĭ�������Ϊ��׼��ʱ����Ϣ����Ϊ�����Ϣͷ;�����Զ��壬��̳и��ಢ��д�ú�������
    */
    virtual void SetCustomOutPutHeaderInfo( const std::string & outPutHeaderInfoStr );

private:

    /**
    *  @brief  ��ȡ��׼�������Ϣͷ
    *  @return ������Ϣͷ
    */
    std::string GetStandardHeaderInfo();

private:

    std::string     m_customOutPutHeaderInfo;    //  ���ͷ��Ϣ��һ��Ϊ��ǰ�����¼ʱ����������ʶ
    bool            m_bOutPutHeaderFlag;         //  �Ƿ���������Ϣͷ
};


//  ��Ŀ�����
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


//  ����̨Ŀ�����
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


//  �ļ�Ŀ�����
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
    *  @brief  ���������Ϣ�ļ����·��
    *  @param  [in] 	filePath            �����Ϣ�ļ����·��
    *  @detail:         ��filePathΪnullptr����Ĭ�����������̨��Ч����ͬCOutPutConsoleSink
    */
    void SetOutPutFilePath( const char * filePath = nullptr );

private:

    /**
    *  @brief  ���ļ�
    */
    void OpenFile();

    /**
    *  @brief  ��ȫ�ر��ļ�
    */
    void SafeCloseFile();

private:

    COutPutFileSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutFileSink );

private:

    std::ofstream       m_fstream;          //  �ļ����������
    std::string         m_outPutFilePath;    //  �����Ϣ�ļ����·��
};


//  ����Ŀ�����
//  ��Ȼ�ɽ����̫��(TCP/UDP��)��USB��GPIB��RS232�ȷ�ʽ
//  ����չ���ɲ�����������̫����ʽʵ�ֻ��ع�����������ʽʵ��
//  Ŀǰ���������ʱ����ͬ����ʽ����Ȼ��ʹ���첽������ͨ��ģʽʵ��(������¼���select����ɶ˿ڵ�)
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
    *  @brief  ���������Ϣ���Ŀ���ַ
    *  @param  [in] 	ipAddress          Ŀ������IP��ַ
    *  @param  [in] 	port               Ŀ�������˿�
    */
    void SetOutPutTarget( const char * ipAddress, unsigned short port );

private:

    /**
    * @brief:  Ĭ��������ز���
    */
    void SetDefaultCommParameters();

    /**
    *  @brief  ����Զ��Ŀ��
    */
    void ConnectTarget();

    /**
    *  @brief  ��ȫ�Ͽ�Զ��Ŀ��
    */
    void SafeCloseConnect();

private:

    COutPutTcpSink();

    DISALLOW_COPY_AND_ASSIGN( COutPutTcpSink );

private:

    std::string     m_ipAddress;    //  Ŀ������IP��ַ
    unsigned short  m_port;         //  Ŀ�������˿�
    SOCKET          m_socket;       //  �׽��־��
    int             m_timeWaitSend; //  �������ݳ�ʱ
    bool            m_isNetValid;   //  ��������Ƿ����
};


#endif  //  _OUT_PUT_SINK_H_