///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ftp_config.h
/// 	@brief	FTP客户端配置相关. 
/// 
///		主要包含必要的宏、结构等  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/17  9:01:51
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _FTP_CONFIG_H_
#define     _FTP_CONFIG_H_

#include "thread.h"

//  文件信息
typedef struct _FileInfo
{
    bool				type;					//  0 目录，1 文件
    char				FileModdifyTime[16];	//  文件最近一次修改时间
    char				FileSize[24];			//  文件的大小
    wchar_t				FileName[512];			//  文件的名称

    _FileInfo() :type( false )
    {
        strcpy_s( FileModdifyTime, "0" );
        strcpy_s( FileSize, "0" );
        wcscpy_s( FileName, L".." );
    }

    _FileInfo( const _FileInfo & fileInfo )
    {
        type = fileInfo.type;
        strcpy_s( FileModdifyTime, fileInfo.FileModdifyTime );
        strcpy_s( FileSize, fileInfo.FileSize );
        wcscpy_s( FileName, fileInfo.FileName );
    }

} FILEINFO, *PFILEINFO;


//  操作结果
enum FM_RESUlT
{
    FM_SUCCESS = 0,						//  返回成功
    FM_FAILD = 1,						//  返回末知失败
    FM_CONNECTCUT,						//  网络异常
    FM_FILE_NO_EXIST,					//  文件不存在于队列中
    FM_OBSERVER_ERR,                    //  观察者设置或移除失败
};


//  下载操作方式
enum DownLoadStates
{
    PAUSEDOWNLOAD = 1,                  //  停止下载
    CANCELDOWNLOAD,                     //  取消下载
    STARTINGDOWNLOAD = -1,              //  继续下载
};


//  文件节点信息
struct FMNode
{
    bool			type;				//  类型，0 目录，1 文件
    std::string		name;				//  文件名
    std::string		moddifyTime;		//  文件最后修改时间
    std::string		size;				//  文件大小
};


//  文件下载状态
enum FileDownLoadSate
{
    Downloading = 0,                    //  文件正在下载
    StopedDownload,                     //  文件已停止下载
    PauseedDownload,                    //  文件已暂停下载
    FinishedDownload,                   //  文件已完成下载
    CrishBySocketDownload,              //  网络异常，下载停止
    LackOfDiskSpace,                    //  磁盘空间不足
    OtherStates,                        //  其他异常
};


//  文件节点信息
//  用在下载操作中
struct FileNode
{
    std::string		    remotePath;		//	远程文件
    std::string		    localPath;		//	本地文件
    long long int	    fileLength;		//	文件长度
    double			    progress;		//	下载进度(0-100%)
    FileDownLoadSate	state;			//	文件下载状态
    int				    dirFlag;		//	目录级数标识
    bool			    IsFinished;		//	下载完成标识

    FileNode( std::string s_remotePath = "", std::string s_localPath = "",
        long long int s_fileLength = 0, double s_progress = 0.0, FileDownLoadSate s_state = OtherStates,
        int s_dirFlag = 0, bool s_IsFinished = false )
    {
        localPath = s_localPath;
        remotePath = s_remotePath;
        fileLength = s_fileLength;
        progress = s_progress;
        state = s_state;
        dirFlag = s_dirFlag;
        IsFinished = s_IsFinished;
    }

    FileNode( const FileNode & that )
    {
        localPath = that.localPath;
        remotePath = that.remotePath;
        fileLength = that.fileLength;
        progress = that.progress;
        state = that.state;
        dirFlag = that.dirFlag;
        IsFinished = that.IsFinished;
    }

    FileNode & operator = ( const FileNode &that )
    {
        localPath = ( that.localPath == "" ? localPath : that.localPath );
        remotePath = ( that.remotePath == "" ? remotePath : that.remotePath );
        fileLength = ( that.fileLength == -1 ) ? fileLength : that.fileLength;
        progress = ( ( fabs( that.progress + 1 ) < 1.0001 ) ? progress : that.progress );
        state = ( that.state == OtherStates ? state : that.state );
        dirFlag = ( that.dirFlag == -1 ? dirFlag : that.dirFlag );
        IsFinished = that.IsFinished;
        return *this;
    }

    bool operator == ( const FileNode &that )
    {
        if ( that.dirFlag == dirFlag || dirFlag == -2 )
        {
            return true;
        }
        return false;
    }
};


typedef struct _FTPTHREADPARAM
{
    std::string			szRemoteFile;	//  远程文件名.
    std::string			szLocalFile;	//  本地文件名.
    long long int		resFileLen;		//	剩余文件数据
    long long int		fileLength;		//	文件长度  .
    long				offset;			//  下载偏移量.
}FTPTHREADPARAM, *PFTPTHREADPARAM;


//  下载回调相关声明
typedef void( *pBeginFuncCallBack )( void *context );
typedef void( *pDowningFuncCallBack )( void *context, const FileNode * pFileNode );
typedef void( *pDownErrorFuncCallBack )( void *context, const FileNode * pFileNode );
typedef void( *pDownedFuncCallBack )( void *context, const FileNode * pFileNode );


template<typename T>
class CFtpThread:public CThread
{
    typedef void ( T::*pFuncCallback )(void *param);

public:

    CFtpThread( T * pObject = NULL, pFuncCallback  funcCallback = NULL, void *param = NULL) :
        m_pObject( pObject ),
        m_funcCallback( funcCallback ),
        m_param(param)
    {

    }

    ~CFtpThread()
    {

    }

public:

    inline void SetSetUpCallback( pFuncCallback funcCallback)
    {
        m_funcCallback = funcCallback;
    }

    inline void SetSetUpCallbackParamter( void *param )
    {
        m_param = param;
    }

public:

    virtual void Run()
    {
        if ( m_pObject != NULL && m_funcCallback != NULL )
        {
            ( m_pObject->*m_funcCallback )( m_param );
        }
    }

private:

    T               *m_pObject;
    pFuncCallback   m_funcCallback;
    void            *m_param;
};

#endif  //  _FTP_CONFIG_H_