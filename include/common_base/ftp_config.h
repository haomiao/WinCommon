///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ftp_config.h
/// 	@brief	FTP�ͻ����������. 
/// 
///		��Ҫ������Ҫ�ĺꡢ�ṹ��  
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

//  �ļ���Ϣ
typedef struct _FileInfo
{
    bool				type;					//  0 Ŀ¼��1 �ļ�
    char				FileModdifyTime[16];	//  �ļ����һ���޸�ʱ��
    char				FileSize[24];			//  �ļ��Ĵ�С
    wchar_t				FileName[512];			//  �ļ�������

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


//  �������
enum FM_RESUlT
{
    FM_SUCCESS = 0,						//  ���سɹ�
    FM_FAILD = 1,						//  ����ĩ֪ʧ��
    FM_CONNECTCUT,						//  �����쳣
    FM_FILE_NO_EXIST,					//  �ļ��������ڶ�����
    FM_OBSERVER_ERR,                    //  �۲������û��Ƴ�ʧ��
};


//  ���ز�����ʽ
enum DownLoadStates
{
    PAUSEDOWNLOAD = 1,                  //  ֹͣ����
    CANCELDOWNLOAD,                     //  ȡ������
    STARTINGDOWNLOAD = -1,              //  ��������
};


//  �ļ��ڵ���Ϣ
struct FMNode
{
    bool			type;				//  ���ͣ�0 Ŀ¼��1 �ļ�
    std::string		name;				//  �ļ���
    std::string		moddifyTime;		//  �ļ�����޸�ʱ��
    std::string		size;				//  �ļ���С
};


//  �ļ�����״̬
enum FileDownLoadSate
{
    Downloading = 0,                    //  �ļ���������
    StopedDownload,                     //  �ļ���ֹͣ����
    PauseedDownload,                    //  �ļ�����ͣ����
    FinishedDownload,                   //  �ļ����������
    CrishBySocketDownload,              //  �����쳣������ֹͣ
    LackOfDiskSpace,                    //  ���̿ռ䲻��
    OtherStates,                        //  �����쳣
};


//  �ļ��ڵ���Ϣ
//  �������ز�����
struct FileNode
{
    std::string		    remotePath;		//	Զ���ļ�
    std::string		    localPath;		//	�����ļ�
    long long int	    fileLength;		//	�ļ�����
    double			    progress;		//	���ؽ���(0-100%)
    FileDownLoadSate	state;			//	�ļ�����״̬
    int				    dirFlag;		//	Ŀ¼������ʶ
    bool			    IsFinished;		//	������ɱ�ʶ

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
    std::string			szRemoteFile;	//  Զ���ļ���.
    std::string			szLocalFile;	//  �����ļ���.
    long long int		resFileLen;		//	ʣ���ļ�����
    long long int		fileLength;		//	�ļ�����  .
    long				offset;			//  ����ƫ����.
}FTPTHREADPARAM, *PFTPTHREADPARAM;


//  ���ػص��������
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