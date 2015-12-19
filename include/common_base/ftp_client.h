///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ftp_client.h
/// 	@brief	FTP�ͻ���ʵ��. 
/// 
///		��Ҫʵ��FTP�ͻ��˵Ļ�����������,FileZilla Server����ͨ�� 
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/16  15:11:13
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _FTP_CLIENT_H_
#define     _FTP_CLIENT_H_

#include <vector>
#include <time.h>
#include <string>

#include "types.h"
#include "auto_lock.h"
#include "lock_critical_section.h"
#include "communication_tcp.h"
#include "timer_helper.h"
#include "ftp_config.h"


//  FTP�ͻ��ˣ�ʵ�ֻ����Ĳ���
//  TODO:�ϴ����������������ò������Լ����ػص��Ȳ����������
class CFtpClient
{

public:

    //  �ṩ�����������̵Ļص�����
    //  ��Ӧ�ص�����������Ϊ�û����ݲ���
    CFtpClient( pBeginFuncCallBack beginFuncCallback = NULL, 
                void *beginFuncContext = NULL,
                pDowningFuncCallBack downingFuncCallback = NULL,
                void *downingFuncContext = NULL,
                pDownErrorFuncCallBack downErrorFuncCallback = NULL,
                void *downErrorFuncContext = NULL,
                pDownedFuncCallBack downedFuncCallback = NULL,
                void *downedFucnContext = NULL
                );

    ~CFtpClient();

public:

    /**
    * @brief:		����FTP������
    * @return:		���ӳɹ����
    */
    bool ConnectServer( const char* hostAddr = "127.0.0.1", unsigned int port = 21,
        const char* userName = "anonymous", const char* password = "" );

    /**
    * @brief:		�Ͽ�����FTP������(�����ļ��������أ����˳�����)
    * @param[in]:   watiTime                                �ȴ���ʱ
    * @return:		�Ͽ����ӳɹ����
    */
    bool Disconnect( DWORD watiTime = INFINITE);

    /**
    * @brief:		��ȡԶ���б�ṹ��Ϣ
    * @param[in]:	const char *dirPath						Զ���ļ�Ŀ¼·��
    * @param[out]:	std::vector<FILEINFO> &fileInfoVector	�ļ�Ŀ¼��Ϣ������
    * @return:		���ػ�ȡ�б���Ϣ״̬
    */
    int ListRemoteFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector );

    /**
    * @brief:		����FTP������Ŀ¼���ļ�(�ݲ�֧��ͬʱ���ض���ļ�)
    * @param[in]:	const char* remoteFilePath				Զ���ļ�·����
    * @param[in]:	const char* localFilePath				�����ļ�·����
    * @param[in]:	sync				                    �Ƿ�Ϊͬ������
    * @return:		int  									�����ļ�����״̬
    * @detail:      ���ļ�δ��������ֿ�ʼ�µ����أ����µ������ļ����ȴ������ļ���ɲŻῪʼ
    */
    int DownLoadDirOrFile( const char* remoteFilePath, const char* localFilePath, bool sync = true);

    /**
    * @brief:   	���ز��������������ļ���ͣ��������ȡ�����ز���
    * @param[in]:	DownLoadStates downLoadState			�����ļ�����
    * @return:  	int										��������״̬
    */
    int SetDownLoadState( DownLoadStates downLoadState );

    /**
    * @brief:		ɾ��Զ��ָ���ļ�
    * @param[in]:	const char *dirPath						Զ���ļ�·��
    * @return:		���ػ�ȡ�б���Ϣ״̬
    */
    bool DeleteRemoteFile( const char* remoteFilePath );

    /**
    * @brief:		ɾ��Զ��ָ��Ŀ¼(Ŀǰ��ʱֻ֧��ɾ����Ŀ¼����ʱ��֧�ֱ���ɾ������������)
    * @param[in]:	const char *remoteDirPath				Զ���ļ�Ŀ¼·��
    * @return:		���ػ�ȡ�б���Ϣ״̬
    */
    bool DeleteRemoteDir( const char* remoteDirPath );

private:

    /**
    * @brief:			��¼FTP������
    */
    bool ConnectFtpServer();

    /**
    * @brief:			��¼FTP������
    * @pararm[in]:		const char * userName					��¼�û���
    * @pararm[in]:		const char * password					��¼����
    * @return:			BOOL  									���ص�¼״̬
    */
    bool Login();

    /**
    * @brief:   		��������������������
    * @pararm[in]:		const char * commandStr					�����ַ���
    * @pararm[in]:		const char * arg						�������
    * @return:  		BOOL									���ط���״̬
    */
    bool SendCommand( const char * commandStr, const char *arg );

    /**
    * @brief:			��ȡ��������Ӧ��Ϣ
    * @param[out]:		unsigned int &resCode					��Ӧ��
    * @return:			std::string								��Ӧ�ַ���
    */
    std::string RecvRespond(unsigned int &resCode );

    /**
    * @brief:   		��ȡ������Ӧ�벢�ж��Ƿ������Ӧ���
    * @param[in]:		const char *respondInfo					��Ӧ�ַ���
    * @param[in]:		unsigned int &respondCode				��Ӧ��
    * @return:  		BOOL									���ؽ�����Ӧ״̬
    */
    bool IsRecvFinish( const char *respondInfo, unsigned int &respondCode );

    /**
    * @brief:   		��ȡ�б�ͨ�ú���
    * @param[in]:		const char * dirPath					Զ��Ŀ¼·��
    * @param[in/out]:	fileInfoVector							Ŀ¼�ļ���Ϣ������
    * @return:  		BOOL									���ػ�ȡĿ¼��Ϣ״̬
    */
    bool ListFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector );

    /**
    * @brief:   		��ȡ������Ӧ�ַ����е�IP��ַ�Ͷ˿�
    * @pararm[in]:		const char * respondInfo				��Ӧ�ַ���
    * @pararm[in/out]:	char * hostAddr							������IP
    * @pararm[in/out]:	unsigned int &port						�������˿�
    */
    void GetPasvInfo( const char * respondInfo, char * hostAddr, unsigned int &port );

    /**
    * @brief:			��ȡԶ��Ŀ¼�б���Ϣ
    * @return:			std::string  							���ػ�ȡ��Ŀ¼��Ϣ��
    */
    std::string RecvRemoteList();

    /**
    * @brief:			����Զ��Ŀ¼�б���Ϣ
    * @param[in]:		const char * remoteListInfo				Զ���ļ�Ŀ¼��Ϣ��
    * @param[in/out]:	fileInfoVector							������Ŀ¼��Ϣ
    */
    void AnalyzeListInfo( const char *remoteListInfo, std::vector<FILEINFO> &fileInfoVector );

private:

    /**
    * @brief:			Ĭ�ϴ�����
    */
    void ErrorDefaultDeal();

    /**
    * @brief:   		�ж��Ƿ���Ҫ����
    */
    void IsFtpOnline();

    /**
    * @brief:   		���ؽ��̺���
    */
    void DownThreadExec( void *param );

    /**
    * @brief:   		����Ŀ¼�����������µ���Ӧ���ļ����ļ���
    * @param[in]:	    char * remoteDir						    Զ��Ŀ¼·����
    * @param[in]:		char * localDir						        ����Ŀ¼·����
    * @param[in]:		int & dirFlag							    Ŀ¼������ʶ
    * @return:  		bool										���ر���Ŀ¼״̬
    */
    bool ErgodicDir( char * remoteDir, char * localDir, int &dirFlag );

    /**
    * @brief:   		����Ŀ¼��ǰ�������
    * @param[in/out]:   char * Dir									Զ��Ŀ¼·����
    * @param[out]:		const char * block							��һ��Ŀ¼
    * @param[in]:		bool flag									Ŀ¼��ʶ
    * TODO��			flagΪtrue,��Ŀ¼ǰ��,Ϊfalse��ΪĿ¼����
    */
    void ChangeDir( char * Dir, const char * block, bool flag );

    /**
    * @brief:			����FTP�������ļ�
    * @return:			bool                ���������ļ�״̬
    */
    bool DownLoadFile( const char* remoteFilePath, const char* localFilePath );

    /**
    * @brief:			���ļ����غ���
    * @param[in]:		PFTPTHREADPARAM pParam
    */
    bool DownLoad( PFTPTHREADPARAM pParam );

private:

    //  ������ػص��Լ�����

    pBeginFuncCallBack      m_beginFuncCallback;
    void                    *m_beginFuncContext;
    pDowningFuncCallBack    m_downingFuncCallback;
    void                    *m_downingFuncContext;
    pDownErrorFuncCallBack  m_downErrorFuncCallback;
    void                    *m_downErrorFuncContext;
    pDownedFuncCallBack     m_downedFuncCallback;
    void                    *m_downedFucnContext;

private:

    /* ��¼��ַ���û��������� */
    std::string				m_hostAddr;
    std::string				m_userName;
    std::string				m_password;

    unsigned int			m_ctlPort;          //  �������Ӷ˿�
    time_t					m_lastTime;         //  ��¼��������ʱ��

private:

    TcpComm                 m_tcpComm;          //  TCP���ӿ��ƶ���
    TcpComm                 m_tcpDataComm;      //  TCP������·����
    volatile bool           m_isValidConnect;   //  �Ƿ���Ч����
    HANDLE                  m_eventHandle;      //  �����¼����
    volatile DownLoadStates m_downLoadState;    //  ���ز���״̬

    typedef task_timer <AutoTask<void, void> > AutoTaskProc;
    AutoTaskProc            m_updateTask;       //  �Զ����������Ƿ��ѶϿ���������

private:

    CThreadControl          m_threadControl;    //  �߳̿��ƶ���
    std::string             m_remoteFilePath;   //  Զ���ļ�·����
    std::string             m_localFilePath;    //  �����ļ�·����

};  //  class CFtpClient

#endif  //  _FTP_CLIENT_H_
