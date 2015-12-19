///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ftp_client.h
/// 	@brief	FTP客户端实现. 
/// 
///		主要实现FTP客户端的基本操作功能,FileZilla Server测试通过 
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


//  FTP客户端，实现基本的操作
//  TODO:上传、更名及其他常用操作，以及下载回调等操作今后完善
class CFtpClient
{

public:

    //  提供下载整个流程的回调函数
    //  对应回调参数上下文为用户数据参数
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
    * @brief:		连接FTP服务器
    * @return:		连接成功与否
    */
    bool ConnectServer( const char* hostAddr = "127.0.0.1", unsigned int port = 21,
        const char* userName = "anonymous", const char* password = "" );

    /**
    * @brief:		断开连接FTP服务器(若有文件正在下载，则退出下载)
    * @param[in]:   watiTime                                等待超时
    * @return:		断开连接成功与否
    */
    bool Disconnect( DWORD watiTime = INFINITE);

    /**
    * @brief:		获取远程列表结构信息
    * @param[in]:	const char *dirPath						远程文件目录路径
    * @param[out]:	std::vector<FILEINFO> &fileInfoVector	文件目录信息缓冲区
    * @return:		返回获取列表信息状态
    */
    int ListRemoteFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector );

    /**
    * @brief:		下载FTP服务器目录或文件(暂不支持同时下载多个文件)
    * @param[in]:	const char* remoteFilePath				远程文件路径名
    * @param[in]:	const char* localFilePath				本地文件路径名
    * @param[in]:	sync				                    是否为同步下载
    * @return:		int  									返回文件下载状态
    * @detail:      若文件未下载完成又开始新的下载，则新的下载文件将等待下载文件完成才会开始
    */
    int DownLoadDirOrFile( const char* remoteFilePath, const char* localFilePath, bool sync = true);

    /**
    * @brief:   	下载操作函数，处理文件暂停、继续、取消下载操作
    * @param[in]:	DownLoadStates downLoadState			处理文件操作
    * @return:  	int										返回设置状态
    */
    int SetDownLoadState( DownLoadStates downLoadState );

    /**
    * @brief:		删除远程指定文件
    * @param[in]:	const char *dirPath						远程文件路径
    * @return:		返回获取列表信息状态
    */
    bool DeleteRemoteFile( const char* remoteFilePath );

    /**
    * @brief:		删除远程指定目录(目前暂时只支持删除空目录，暂时不支持遍历删除，今后待完善)
    * @param[in]:	const char *remoteDirPath				远程文件目录路径
    * @return:		返回获取列表信息状态
    */
    bool DeleteRemoteDir( const char* remoteDirPath );

private:

    /**
    * @brief:			登录FTP服务器
    */
    bool ConnectFtpServer();

    /**
    * @brief:			登录FTP服务器
    * @pararm[in]:		const char * userName					登录用户名
    * @pararm[in]:		const char * password					登录密码
    * @return:			BOOL  									返回登录状态
    */
    bool Login();

    /**
    * @brief:   		发送请求命令至服务器
    * @pararm[in]:		const char * commandStr					命令字符串
    * @pararm[in]:		const char * arg						命令参数
    * @return:  		BOOL									返回发送状态
    */
    bool SendCommand( const char * commandStr, const char *arg );

    /**
    * @brief:			获取服务器响应信息
    * @param[out]:		unsigned int &resCode					响应码
    * @return:			std::string								响应字符串
    */
    std::string RecvRespond(unsigned int &resCode );

    /**
    * @brief:   		获取命令响应码并判断是否接受响应完成
    * @param[in]:		const char *respondInfo					响应字符串
    * @param[in]:		unsigned int &respondCode				响应码
    * @return:  		BOOL									返回接受响应状态
    */
    bool IsRecvFinish( const char *respondInfo, unsigned int &respondCode );

    /**
    * @brief:   		获取列表通用函数
    * @param[in]:		const char * dirPath					远程目录路径
    * @param[in/out]:	fileInfoVector							目录文件信息缓冲区
    * @return:  		BOOL									返回获取目录信息状态
    */
    bool ListFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector );

    /**
    * @brief:   		获取返回响应字符串中的IP地址和端口
    * @pararm[in]:		const char * respondInfo				响应字符串
    * @pararm[in/out]:	char * hostAddr							服务器IP
    * @pararm[in/out]:	unsigned int &port						服务器端口
    */
    void GetPasvInfo( const char * respondInfo, char * hostAddr, unsigned int &port );

    /**
    * @brief:			获取远程目录列表信息
    * @return:			std::string  							返回获取的目录信息串
    */
    std::string RecvRemoteList();

    /**
    * @brief:			解析远程目录列表信息
    * @param[in]:		const char * remoteListInfo				远程文件目录信息串
    * @param[in/out]:	fileInfoVector							解析后目录信息
    */
    void AnalyzeListInfo( const char *remoteListInfo, std::vector<FILEINFO> &fileInfoVector );

private:

    /**
    * @brief:			默认错误处理
    */
    void ErrorDefaultDeal();

    /**
    * @brief:   		判断是否需要重连
    */
    void IsFtpOnline();

    /**
    * @brief:   		下载进程函数
    */
    void DownThreadExec( void *param );

    /**
    * @brief:   		遍历目录，并下载其下的相应的文件或文件夹
    * @param[in]:	    char * remoteDir						    远程目录路径名
    * @param[in]:		char * localDir						        本地目录路径名
    * @param[in]:		int & dirFlag							    目录级数标识
    * @return:  		bool										返回遍历目录状态
    */
    bool ErgodicDir( char * remoteDir, char * localDir, int &dirFlag );

    /**
    * @brief:   		更改目录，前进与后退
    * @param[in/out]:   char * Dir									远程目录路径名
    * @param[out]:		const char * block							下一级目录
    * @param[in]:		bool flag									目录标识
    * TODO：			flag为true,则目录前进,为false则为目录后退
    */
    void ChangeDir( char * Dir, const char * block, bool flag );

    /**
    * @brief:			下载FTP服务器文件
    * @return:			bool                返回下载文件状态
    */
    bool DownLoadFile( const char* remoteFilePath, const char* localFilePath );

    /**
    * @brief:			单文件下载函数
    * @param[in]:		PFTPTHREADPARAM pParam
    */
    bool DownLoad( PFTPTHREADPARAM pParam );

private:

    //  下载相关回调以及参数

    pBeginFuncCallBack      m_beginFuncCallback;
    void                    *m_beginFuncContext;
    pDowningFuncCallBack    m_downingFuncCallback;
    void                    *m_downingFuncContext;
    pDownErrorFuncCallBack  m_downErrorFuncCallback;
    void                    *m_downErrorFuncContext;
    pDownedFuncCallBack     m_downedFuncCallback;
    void                    *m_downedFucnContext;

private:

    /* 登录地址、用户名、密码 */
    std::string				m_hostAddr;
    std::string				m_userName;
    std::string				m_password;

    unsigned int			m_ctlPort;          //  控制连接端口
    time_t					m_lastTime;         //  记录连接运行时间

private:

    TcpComm                 m_tcpComm;          //  TCP连接控制对象
    TcpComm                 m_tcpDataComm;      //  TCP数据链路对象
    volatile bool           m_isValidConnect;   //  是否有效连接
    HANDLE                  m_eventHandle;      //  下载事件句柄
    volatile DownLoadStates m_downLoadState;    //  下载操作状态

    typedef task_timer <AutoTask<void, void> > AutoTaskProc;
    AutoTaskProc            m_updateTask;       //  自动检查服务器是否已断开连接任务

private:

    CThreadControl          m_threadControl;    //  线程控制对象
    std::string             m_remoteFilePath;   //  远程文件路径名
    std::string             m_localFilePath;    //  本地文件路径名

};  //  class CFtpClient

#endif  //  _FTP_CLIENT_H_
