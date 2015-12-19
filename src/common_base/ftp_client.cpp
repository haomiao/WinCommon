#include "stdafx.h"
#include "ftp_client.h"

namespace
{

/********************************************************************************************
                            服务器响应码
*********************************************************************************************/

    #define FTP_RETCODE_RESTART_ACK       110                     /*  重新启动标记应答            */
    #define FTP_RETCODE_READY_MIN         120                     /*  服务在nnn分钟内准备好       */
    #define FTP_RETCODE_DATALINK_READY    125                     /*  数据连接已准备好            */
    #define FTP_RETCODE_FILE_OK           150                     /*  文件状态良好，打开数据连接  */
    #define FTP_RETCODE_CMD_OK            200                     /*  命令成功                    */
    #define FTP_RETCODE_CMD_UNSUPPORT     202                     /*  命令未实现                  */
    #define FTP_RETCODE_STATUS_HELP       211                     /*  系统状态或系统帮助响应      */
    #define FTP_RETCODE_DIR_STATUS        212                     /*  目录状态                    */
    #define FTP_RETCODE_FILE_STATUS       213                     /*  文件状态                    */
    #define FTP_RETCODE_HELP_INFO         214                     /*  帮助信息, 仅对人类用户有用  */
    #define FTP_RETCODE_NAME_SYS_TYPE     215                     /*  名字系统类型                */
    #define FTP_RETCODE_READY             220                     /*  服务器就绪                  */
    #define FTP_RETCODE_BYEBYE            221                     /*  服务关闭控制连接可以退出登录*/
    #define FTP_RETCODE_DATALINK_NODATA   225                     /*  数据连接打开，无传输正在进行*/
    #define FTP_RETCODE_DATACLOSE_NOERR   226                     /*  关闭数据连接, 文件操作成功  */
    #define FTP_RETCODE_INTO_PASV         227                     /*  进入被动模式                */
    #define FTP_RETCODE_USER_LOGIN        230                     /*  用户登录                    */
    #define FTP_RETCODE_FILE_OP_OK        250                     /*  请求的文件操作完成          */
    #define FTP_RETCODE_MAKE_DIR_OK       257                     /*  创建目录成功                */
    #define FTP_RETCODE_PW_REQ            331                     /*  用户名正确，需要口令        */
    #define FTP_RETCODE_NEED_INFO         350                     /*  请求需要进一步的信息        */
    #define FTP_RETCODE_CONNET_TOO_MANY   421                     /*  连接数太多                  */
    #define FTP_RETCODE_DATALINK_FAILED   425                     /*  不能打开数据连接            */
    #define FTP_RETCODE_DATALINK_ABORT    426                     /*  关闭连接，中止传输          */
    #define FTP_RETCODE_REQ_NOT_RUN       450                     /*  请求命令未执行              */
    #define FTP_RETCODE_REQ_ABORT         451                     /*  中止请求的操作 本地有错     */
    #define FTP_RETCODE_DONOT_RUN_REQ     452                     /*  未执行请求的操作,空间不足   */
    #define FTP_RETCODE_CMD_ERROR         500                     /*  命令不可识别                */
    #define FTP_RETCODE_SYNTAX_ERR        501                     /*  参数语法错误                */
    #define FTP_RETCODE_UNSUP_WITH_ARG    504                     /*  此参数下的命令功能未实现    */
    #define FTP_RETCODE_LOGIN_FAILED      530                     /*  用户登录失败                */
    #define FTP_RETCODE_REQ_FAILED        550                     /*  未执行请求的操作            */
    #define FTP_RETCODE_DREQ_ABORT        551                     /*  数据请求中止                */
    #define FTP_RETCODE_FILE_NAME_ERROR   553                     /*  文件名不合法                */


    /* 客户端常用命令 */
    static const char* kFtpUserName = "USER";   // 用户名
    static const char* kFtpPass     = "PASS";   //  密码口令
    static const char* kFtpPasv     = "PASV";   //  被动模式
    static const char* kFtpType     = "TYPE";   //  定义文件类型
    static const char* kFtpRetr     = "RETR";   //  下载
    static const char* kFtpRest     = "REST";   //  重新开始
    static const char* kFtpQuit     = "QUIT";   //  退出
    static const char* kFtpList     = "LIST";   //  列表
    static const char* kFtpMlsd     = "MLSD";   //  列表
    static const char* kFtpCwd      = "CWD";    //  切换目录
    static const char* kFtpSize     = "SIZE";   //  获取大小
    static const char* kFtpDele     = "DELE";   //  删除文件
    static const char* kFtpRdm      = "RDM";    //  删除目录
    static const char* kFtpRnfr     = "RNFR";   //  重命名旧
    static const char* kFtpRnto     = "RNTO";   //  重命名新
    static const char* kFtpStor     = "STOR";   //  上传


    #define NORMAL_LENGTH	  512					//	512 bytes
    #define MAX_RECV_LENGTH   1024					//	1KB
    #define MAX_RECV_LIST	  1024*1024*10			//	10M
    #define SOCKET_BUFFER	  1024*4				//	接收缓冲区大小
    #define TIME_LINK_LAST	  8						//	8s连接流逝时间
    #define DIR_BUFFER		  1024					//  目录长度
    #define TEMP_BUFFER		  1024					//	局部临时缓存大小
    #define COMMAND_BUFFER	  512					//	命令串长度
    #define SOCKET_NUMBER	  300					//	最大套接字数量
    #define DEFALUT_CTL_PORT  21					//	默认FTP控制链路端口
    #define DEFALUT_DATA_PORT 20					//	默认FTP数据链路端口
    #define THARED_TIMEOUT	  200					//	默认等待线程超时时间
    #define DOWNLOAD_SLEEP_TIME  0                  //  下载线程间歇性休眠时间

    int FindFirstof( const char *srcStr, unsigned int start, char ch )
    {
        if ( srcStr == NULL || ch == '\0' || strlen( srcStr ) < start )
        {
            return -1;
        }
        unsigned int len = strlen( srcStr );

        for ( unsigned int i = start; i < len; ++i )
        {
            if ( srcStr[i] == ch )
            {
                return i;
            }
        }
        return -1;
    }


    void GetStringBuf( const char * srcStr, unsigned int start, unsigned int end, char * desStr )
    {
        if ( srcStr == NULL || desStr == NULL || start > end )
        {
            return;
        }
        unsigned int size = strlen( srcStr );
        unsigned int endSize = strlen( desStr );
        unsigned int j = 0;
        for ( unsigned int i = start; i < size && i < end; ++i )
        {
            desStr[j++] = srcStr[i];
        }
        desStr[j] = '\0';
    }

}



CFtpClient::CFtpClient( pBeginFuncCallBack beginFuncCallback,
    void *beginFuncContext,
    pDowningFuncCallBack downingFuncCallback,
    void *downingFuncContext,
    pDownErrorFuncCallBack downErrorFuncCallback,
    void *downErrorFuncContext,
    pDownedFuncCallBack downedFuncCallback,
    void *downedFucnContext ):
    m_beginFuncCallback( beginFuncCallback ),
    m_beginFuncContext( beginFuncContext ),
    m_downingFuncCallback( downingFuncCallback ),
    m_downingFuncContext( downingFuncContext ),
    m_downErrorFuncCallback( downErrorFuncCallback ),
    m_downErrorFuncContext( downErrorFuncContext ),
    m_downedFuncCallback( downedFuncCallback ),
    m_downedFucnContext( downedFucnContext ),

    m_userName(""),
    m_password(""),
    m_hostAddr(""),
    m_ctlPort(0),
    m_lastTime(0),
    m_isValidConnect(false),
    m_updateTask(500)
{
    m_eventHandle = ::CreateEvent( NULL, TRUE, FALSE, NULL );
}


CFtpClient::~CFtpClient()
{
    ErrorDefaultDeal();
    m_updateTask.clear_task();
    if ( m_eventHandle != NULL )
    {
        ::CloseHandle( m_eventHandle );
    }
}


bool CFtpClient::ConnectServer( const char* hostAddr, unsigned int port,
    const char* userName, const char* password )
{
    if ( NULL == userName || NULL == password || NULL == hostAddr || port <= 0 )
    {
        return false;
    }

    m_userName      = userName;
    m_password      = password;
    m_hostAddr      = hostAddr;
    m_ctlPort       = port;
    m_lastTime      = time( NULL );

    if (m_isValidConnect == true)
    {
        Disconnect();
    }

    return ConnectFtpServer();
}


bool CFtpClient::Disconnect( DWORD watiTime )
{
    ErrorDefaultDeal();
    m_threadControl.Join( watiTime );
    return true;
}


bool CFtpClient::ConnectFtpServer()
{
    EtherCommArg commArg = { m_hostAddr, m_ctlPort };

    /* 若连接失败，尝试登录三次服务器 */
    for ( unsigned int i = 0; i < 3; ++i )
    {
        if ( i != 0 )
        {
            Sleep( 500 );
        }

        int res = m_tcpComm.Open( &commArg );
        if ( res != COMM_SUCCESS )
        {
            continue;
        }

        unsigned int resCode = 0;
        std::string recvStr = RecvRespond( resCode );
        if ( resCode != FTP_RETCODE_READY )
        {
            ErrorDefaultDeal();
            continue;
        }

        if ( Login() == false)
        {
            ErrorDefaultDeal();
            continue;
        }

        m_isValidConnect = true;
        //m_updateTask.clear_task();
        //m_updateTask.add_task( std::tr1::bind( &CFtpClient::IsFtpOnline, this ) );

        return true;
    }
    return false;
}


bool CFtpClient::Login()
{
    unsigned int resCode = 0;
    if ( SendCommand( kFtpUserName, m_userName.c_str() ) )
    {
        RecvRespond( resCode );
        if ( resCode == FTP_RETCODE_PW_REQ )
        {
            if ( SendCommand( kFtpPass, m_password.c_str() ) )
            {
                RecvRespond( resCode );
                if ( resCode == FTP_RETCODE_USER_LOGIN /*|| resCode == FTP_RETCODE_CONNET_TOO_MANY*/ )
                {
                    return TRUE;
                }
                if ( resCode == FTP_RETCODE_CONNET_TOO_MANY )
                {
                    return FALSE;
                }
            }
        }
    }
    return FALSE;
}


bool CFtpClient::SendCommand( const char * commandStr, const char *arg )
{
    if ( commandStr == NULL )
    {
        return FALSE;
    }

    char buffer[COMMAND_BUFFER + 1] = "";
    if ( arg )
    {
        _snprintf_s( buffer, COMMAND_BUFFER + 1, COMMAND_BUFFER, "%s %s\r\n", commandStr, arg );
    }
    else
    {
        _snprintf_s( buffer, COMMAND_BUFFER + 1, COMMAND_BUFFER, "%s\r\n", commandStr );
    }

    int res = m_tcpComm.Write( buffer, strlen( buffer ) );
    if ( res == COMM_SUCCESS )
    {
        return true;
    }
    return false;
}


std::string CFtpClient::RecvRespond( unsigned int &resCode )
{
    //  以下获取响应码待完善优化
    char recvStr[MAX_RECV_LENGTH] = "";
    int recvLen = -1;
    while ( true )
    {
        memset( recvStr, 0, sizeof( recvStr ) );
        recvLen = m_tcpComm.Read(recvStr, MAX_RECV_LENGTH);
        if ( recvLen <= 0 )
        {
            return "";
        }

        recvStr[recvLen] = '\0';
        if ( IsRecvFinish( recvStr, resCode ) )
        {
            break;
        }
    }

    return recvStr;
}


bool CFtpClient::IsRecvFinish( const char *respondInfo, unsigned int &respondCode )
{
    if ( respondInfo == NULL || ( respondInfo && strlen( respondInfo ) < 3 ) )
    {
        return TRUE;
    }

    if ( respondInfo[3] == ' ' )
    {
        respondCode = atoi( respondInfo );
        return TRUE;
    }

    const char * lastLine = respondInfo;
    const char * tempStr = respondInfo;

    int line = 0;
    int nLen = strlen( respondInfo );

    while ( true )
    {
        respondCode = atoi( tempStr );
        lastLine = strchr( tempStr, '\r' );

        if ( lastLine - respondInfo == nLen - 2 )   // 判断串结束
        {
            break;
        }

        lastLine += 2;
        respondCode = atoi( lastLine );
        lastLine += 3;

        if ( *( lastLine ) == ' ' )
        {
            return TRUE;
        }

        tempStr = lastLine;
    }
    return FALSE;
}


void CFtpClient::ErrorDefaultDeal()
{
    m_updateTask.clear_task();
    m_isValidConnect = false;
    m_tcpComm.Close();
}


void CFtpClient::IsFtpOnline()
{
    if ( m_isValidConnect == false )
    {
        return;
    }

    /* 判断是否已经超时 */
    time_t tempTime = time( NULL );
    if ( tempTime - m_lastTime < TIME_LINK_LAST )
    {
        return;
    }

    m_lastTime = tempTime;

    /* 虽超时，但是再判断是否已经断开连接 */
    if ( false == SendCommand("Test onLine Packets", "" ) )
    {
        ErrorDefaultDeal();
    }

    unsigned int resCode = 0;
    RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_CMD_ERROR )
    {
        ErrorDefaultDeal();
    }

    m_isValidConnect = true;
}



int CFtpClient::ListRemoteFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector )
{
    if (m_isValidConnect == false)
    {
        if (ConnectFtpServer() == false)
        {
            return FM_CONNECTCUT;
        }
    }

    if ( ListFile(dirPath, fileInfoVector) == true)
    {
        return FM_SUCCESS;
    }
    return FM_CONNECTCUT;
}


bool CFtpClient::ListFile( const char *dirPath, std::vector<FILEINFO> &fileInfoVector )
{
    if ( !SendCommand( kFtpCwd, dirPath ) )
    {
        return false;
    }

    unsigned int resCode = 0;
    RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_FILE_OP_OK )
    {
        return false;
    }

    if ( !SendCommand( kFtpType, "I" ) )
    {
        return false;
    }
    RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_CMD_OK )
    {
        return false;
    }

    if ( !SendCommand( kFtpPasv, "" ) )
    {
        return false;
    }
    std::string recvStr = RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_INTO_PASV )
    {
        return false;
    }

    char hostAddr[NORMAL_LENGTH] = { 0 };
    unsigned int dataPort = DEFALUT_DATA_PORT;
    GetPasvInfo( recvStr.c_str(), hostAddr, dataPort );

    EtherCommArg commArg = { hostAddr, dataPort };
    int res = m_tcpDataComm.Open( & commArg );
    if ( res == COMM_SUCCESS )
    {
        if ( !SendCommand( kFtpMlsd, "" ) )
        {
            m_tcpDataComm.Close();
            return false;
        }

        recvStr = RecvRespond( resCode );
        if ( recvStr == "" )
        {
            m_tcpDataComm.Close();
            return false;
        }

        std::string recvListStr = RecvRemoteList( );
        if ( recvListStr != "" )
        {
            AnalyzeListInfo( recvListStr.c_str(), fileInfoVector );
        }

        if ( recvStr.size() > 25 )
        {
            m_tcpDataComm.Close();
            return true;
        }

        recvStr = RecvRespond( resCode );
        if ( recvStr == "" && fileInfoVector.size() == 0 )
        {
            m_tcpDataComm.Close();
            return false;
        }
    }

    m_tcpDataComm.Close();
    return true;
}


void CFtpClient::GetPasvInfo( const char * respondInfo, char * hostAddr, unsigned int &port )
{
    if ( !respondInfo )	return;
    unsigned int len = strlen( respondInfo );

    char SerPort1[4] = { 0 };
    char SerPort2[4] = { 0 };
    unsigned int j = 0;
    unsigned int count = 0;
    bool flag = false;

    for ( unsigned int i = 0; i < len; ++i )
    {
        if ( respondInfo[i] == '(' )
        {
            flag = true;
            continue;
        }
        else if ( respondInfo[i] == ')' )
        {
            break;
        }
        if ( !flag )	continue;

        if ( respondInfo[i] == ',' )
        {
            ++count;
            if ( count < 4 )
            {
                hostAddr[j++] = '.';
            }
            else if ( count == 4 || count == 5 )
            {
                j = 0;
            }
            else continue;
        }
        else if ( count < 4 )
        {
            hostAddr[j++] = respondInfo[i];
        }
        else if ( count == 4 )
        {
            SerPort1[j++] = respondInfo[i];
        }
        else if ( count == 5 )
        {
            SerPort2[j++] = respondInfo[i];
        }
    }
    port = atoi( SerPort1 ) * 256 + atoi( SerPort2 );
}


std::string CFtpClient::RecvRemoteList()
{
    char *recvStr = new char[MAX_RECV_LIST + 1];
    char *pTempBuffer = recvStr;

    int recvTime = 3000; // 3s
    m_tcpDataComm.Config( TCP_CFG_RECVTIME, &recvTime );

    int recvLen = 0;
    while ( true )
    {
        recvLen = m_tcpDataComm.Read( pTempBuffer, SOCKET_BUFFER );
        if ( recvLen <= 0 )
        {
            break;
        }
        pTempBuffer += recvLen;
    }

    *pTempBuffer = '\0';
    std::string tempStr = recvStr;
    delete []recvStr;
    return tempStr;
}


void CFtpClient::AnalyzeListInfo( const char *remoteListInfo, std::vector<FILEINFO> &fileInfoVector )
{
    fileInfoVector.clear();

    unsigned int len = strlen( remoteListInfo );
    const char *temp = remoteListInfo;
    char fileNameStr[NORMAL_LENGTH] = { 0 };
    unsigned int start = 0;
    unsigned int end = 0;
    unsigned int j = 0;
    char tempStr[TEMP_BUFFER] = { 0 };
    char typeStr[TEMP_BUFFER] = { 0 };
    char WideStrData[NORMAL_LENGTH] = { 0 };
    std::string wideStr;

    while ( *temp != '\0' )
    {
        FILEINFO	fileInfo;
        j = 0;
        start = FindFirstof( temp, 0, '=' );
        end = FindFirstof( temp, 0, ';' );
        GetStringBuf( temp, start + 1, end, typeStr );
        temp += end + 1;

        start = FindFirstof( temp, 0, '=' );
        end = FindFirstof( temp, 0, ';' );
        GetStringBuf( temp, start + 1, end, tempStr );
        temp += end + 1;

        memset( WideStrData, 0, sizeof( WideStrData ) / sizeof( char ) );
        strcpy_s( fileInfo.FileModdifyTime, tempStr );

        if ( strcmp( typeStr, "file" ) == 0 )
        {
            start = FindFirstof( temp, 0, '=' );
            end = FindFirstof( temp, 0, ';' );
            GetStringBuf( temp, start + 1, end, tempStr );
            temp += end + 1;
            fileInfo.type = true;

            memset( WideStrData, 0, sizeof( WideStrData ) / sizeof( char ) );
            strcpy_s( fileInfo.FileSize, tempStr );
        }
        else
        {
            strcpy_s( fileInfo.FileSize, "0" );
            fileInfo.type = false;
        }

        while ( *temp != '\r' && *temp != '\0' )
        {
            fileNameStr[j++] = *( ++temp );
        }

        if ( *temp == '\0' )
        {
            return;
        }

        fileNameStr[j - 1] = '\0';  //  '\r\n' -> '\0'
        std::wstring tempWStr = UTF8ToUnicode(fileNameStr);
        wcscpy_s( fileInfo.FileName, tempWStr.c_str() );
        fileInfoVector.push_back( fileInfo );
        temp += 2;
    }
}


int CFtpClient::DownLoadDirOrFile( const char* remoteFilePath, const char* localFilePath, bool sync )
{
    if ( m_isValidConnect == false )
    {
        if ( ConnectFtpServer() == false )
        {
            return FM_CONNECTCUT;
        }
    }

    m_threadControl.Join();

    m_remoteFilePath = remoteFilePath;
    m_localFilePath  = localFilePath;
    ThreadPtr threadPtr = new CFtpThread<CFtpClient>( this, &CFtpClient::DownThreadExec, NULL );
    m_threadControl = threadPtr->Start();
    if ( sync == true)
    {
        m_threadControl.Join();
    }
    return FM_SUCCESS;
}


void CFtpClient::DownThreadExec( void *param )
{
    UNREFERENCED_PARAMETER( param );

    if (m_eventHandle != NULL)
    {
        ::ResetEvent( m_eventHandle );
    }

    m_downLoadState = STARTINGDOWNLOAD;

    if ( m_beginFuncCallback != NULL)
    {
        m_beginFuncCallback( m_beginFuncContext );
    }

    //  确定为目录(通过切换目录，尝试是否为目录)
    if ( !SendCommand( kFtpCwd, m_remoteFilePath.c_str() ) )
    {
        return;
    }

    unsigned int resCode = 0;
    RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_FILE_OP_OK )
    {
        bool res = DownLoadFile( m_remoteFilePath.c_str(), m_localFilePath.c_str() );
    }
    else
    {
        int dirFlag = 1;
        char tempRemoteDir[MAX_PATH] = { 0 };
        char tempLocalDir[MAX_PATH] = { 0 };
        strcpy_s( tempRemoteDir, MAX_PATH, m_remoteFilePath.c_str() );
        strcpy_s( tempLocalDir, MAX_PATH, m_localFilePath.c_str() );

        bool res = ErgodicDir( tempRemoteDir, tempLocalDir, dirFlag );
    }

    m_downLoadState = STARTINGDOWNLOAD;
}


bool CFtpClient::ErgodicDir( char * remoteDir, char * localDir, int &dirFlag )
{
    CreateDirectoryA( localDir, NULL );

    std::vector<FILEINFO> listInfo;
    ListFile( remoteDir, listInfo );

    unsigned int resCode = 0;
    int temp = 0;
    unsigned int nSize = listInfo.size();
    bool flag = false;

    for ( unsigned int i = 0; i < nSize; ++i )
    {
        std::string listFileName = Unicode2MBCS( listInfo[i].FileName );
        ChangeDir( remoteDir, listFileName.c_str(), true );
        ChangeDir( localDir, listFileName.c_str(), true );
        if ( listInfo[i].type == true )
        {
            flag = DownLoadFile( remoteDir, localDir );
            ChangeDir( remoteDir, NULL, false );
            ChangeDir( localDir, NULL, false );
            dirFlag += 1;

            //  有异常时则停止下载
            if (flag != true)
            {
                return flag;
            }
        }
        else
        {
            ErgodicDir( remoteDir, localDir, dirFlag );
        }
    }

    ChangeDir( remoteDir, NULL, false );
    ChangeDir( localDir, NULL, false );

    listInfo.clear();
    return flag;
}


void CFtpClient::ChangeDir( char * Dir, const char * block, bool flag )
{
    int oldLength = strlen( Dir );
    char tempStr[MAX_PATH + 1] = { 0 };
    strcpy_s( tempStr, MAX_PATH, Dir );

    memset( Dir, 0, MAX_PATH );
    int blockLength = 0;
    if ( block )
    {
        blockLength = strlen( block );
    }

    if ( flag == true )
    {
        int length = oldLength + blockLength;
        if ( tempStr[oldLength - 1] == '\\' || tempStr[oldLength - 1] == '/' )
        {
            sprintf_s( Dir, MAX_PATH, "%s%s", tempStr, block );
        }
        else
        {
            sprintf_s( Dir, MAX_PATH, "%s\\%s", tempStr, block );
            length += 1;
        }
        Dir[length] = '\0';
    }
    else
    {
        int i = oldLength - 1;
        for ( ; i > 0; --i )
        {
            if ( tempStr[i] == '\\' || tempStr[i] == '/' )
            {
                break;
            }
        }
        strncpy_s( Dir, MAX_PATH, tempStr, i );
    }
}


bool CFtpClient::DownLoadFile( const char* remoteFilePath, const char* localFilePath )
{
    if ( !SendCommand( kFtpSize, remoteFilePath ) )
    {
        return false;
    }

    unsigned int resCode = 0;
    std::string recvStr = RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_FILE_STATUS )
    {
        return false;
    }

    const char * tempStr = recvStr.c_str();
    tempStr += 4;
    long long fileLen = atoi( tempStr );

    /* 读取配置文件，实现断点续传位置 */
    FILE *fpread = NULL;
    char temp[NORMAL_LENGTH] = { 0 };
    sprintf_s( temp, "%s%s", localFilePath, ".tmp" );
    fpread = _fsopen( temp, "r+b", _SH_DENYNO );

    temp[0] = '\0';
    if ( fpread != NULL )
    {
        fgets( temp, sizeof( temp ), fpread );
        fclose( fpread );
    }
    else
    {
        DeleteFileA( localFilePath );
    }

    long offset = atoi( temp );

    /* 设置文件读取起始位置,注意此命令所在位置的顺序 */
    char setStr[NORMAL_LENGTH] = { 0 };
    sprintf_s( setStr, "%d", offset );
    if ( !SendCommand( kFtpRest, setStr ) )
    {
        return false;
    }

    RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_NEED_INFO )
    {
        return false;
    }

    long long int resFileLen = fileLen - offset; // 重新计算接收数据大小												
    if ( !SendCommand( kFtpPasv, "" ) )
    {
        return false;
    }

    recvStr = RecvRespond( resCode );
    if ( resCode != FTP_RETCODE_INTO_PASV )
    {
        return false;
    }

    char hostAddr[NORMAL_LENGTH] = { 0 };
    unsigned int dataPort = 20;
    GetPasvInfo( recvStr.c_str(), hostAddr, dataPort );

    FTPTHREADPARAM ftpThread;
    ftpThread.szRemoteFile = remoteFilePath;
    ftpThread.szLocalFile = localFilePath;
    ftpThread.fileLength = fileLen;
    ftpThread.resFileLen = resFileLen;
    ftpThread.offset = offset;

    EtherCommArg commArg = { hostAddr, dataPort };
    int res = m_tcpDataComm.Open( &commArg );
    if ( res == COMM_SUCCESS )
    {
        if ( !SendCommand( kFtpRetr, remoteFilePath ) )
        {
            m_tcpDataComm.Close();
            return false;
        }

        RecvRespond( resCode );
        if ( resCode != FTP_RETCODE_FILE_OK )
        {
            m_tcpDataComm.Close();
            return false;
        }

        BOOL DownLoadFlag = DownLoad( &ftpThread );
        RecvRespond( resCode );
        m_tcpDataComm.Close();
        if ( DownLoadFlag == NULL )
        {
            return false;
        }
        return true;
    }
    return false;
}


bool CFtpClient::DownLoad( PFTPTHREADPARAM pParam )
{
    PFTPTHREADPARAM pFtpThread = pParam;
    if ( pFtpThread->szLocalFile == "" || pFtpThread->fileLength < 0 )
    {
        return FALSE;
    }

    register int recvLen = 0;
    register int res = 0;

    /* 写数据文件 */
    FILE *fpWriteData = NULL;
    fpWriteData = _fsopen( pFtpThread->szLocalFile.c_str(), "a+b", _SH_DENYNO );
    if ( fpWriteData == NULL )
    {
        return false;
    }

    /* 写配置文件，断点续传位置 */
    FILE *fpWriteConfig = NULL;
    char tempPath[DIR_BUFFER] = { 0 };
    sprintf_s( tempPath, "%s%s", pFtpThread->szLocalFile.c_str(), ".tmp" );
    fpWriteConfig = _fsopen( tempPath, "w+b", _SH_DENYNO );
    if ( fpWriteConfig == NULL )
    {
        fclose( fpWriteData );
        return false;
    }

    char tempStr[NORMAL_LENGTH] = { 0 };
    long long int fileLength = pFtpThread->resFileLen;
    double  progress = 0.0;

    //  临时缓冲区，避免频繁写文件
    int reciveBufferSize = SOCKET_BUFFER * 50;
    char *reciveFileData = new char[reciveBufferSize + 1];
    if ( reciveFileData == NULL)
    {
        fclose( fpWriteData );
        fclose( fpWriteConfig );
        return false;
    }

    char *pTempData = reciveFileData;
    int errCode = 0;
    while ( pFtpThread->resFileLen && ::WaitForSingleObject(m_eventHandle, 0) != WAIT_OBJECT_0)
    {
        recvLen = m_tcpDataComm.Read( reciveFileData, SOCKET_BUFFER );
        if ( recvLen <= 0 )
        {
            errCode = WSAGetLastError();
            break;
        }

        reciveFileData += recvLen;

        int tempSize = reciveFileData - pTempData;
        if ( tempSize >= reciveBufferSize - SOCKET_BUFFER)  //  避免对缓冲区移除
        {
            /* 写文件数据 */
            res = fwrite( pTempData, tempSize, 1, fpWriteData );
            if ( res == EOF )
            {
                break;
            }

            reciveFileData = pTempData;

            /* 写配置文件,偏移量 */
            fseek( fpWriteConfig, 0, SEEK_SET );
            pFtpThread->offset += recvLen;
            memset( tempStr, 0, sizeof( tempStr ) );
            sprintf_s( tempStr, "%d*此为下载配置文件，勿删、勿修改!文件下载完成后将自动删除!*", pFtpThread->offset );
            fwrite( tempStr, strlen( tempStr ), 1, fpWriteConfig );
        }

        pFtpThread->resFileLen -= recvLen;
        progress = ( ( pFtpThread->fileLength - pFtpThread->resFileLen ) / (double)pFtpThread->fileLength ) * 100;

        //Sleep( DOWNLOAD_SLEEP_TIME );
    }

    //  拷贝剩下的数据
    int tempSize = reciveFileData - pTempData;
    if ( tempSize > 0 )
    {
        /* 写剩余的文件数据 */
        fwrite( pTempData, tempSize, 1, fpWriteData );

        /* 写配置文件,偏移量 */
        if ( fpWriteConfig != NULL )
        {
            fseek( fpWriteConfig, 0, SEEK_SET );
            pFtpThread->offset += tempSize;
            memset( tempStr, 0, sizeof( tempStr ) );
            sprintf_s( tempStr, "%d*此为下载配置文件，勿删、勿修改!文件下载完成后将自动删除!*", pFtpThread->offset );
            fwrite( tempStr, strlen( tempStr ), 1, fpWriteConfig );
        }
    }

    fflush( fpWriteConfig );
    fclose( fpWriteConfig );
    fflush( fpWriteData );
    fclose( fpWriteData );

    delete []pTempData;

    bool result = false;
    if ( pFtpThread->resFileLen == 0 )
    {
        if ( errCode == 0 )
        {
            DeleteFileA( tempPath );
            result = true;
        }
    }
    else
    {
        if ( m_downLoadState == CANCELDOWNLOAD )
        {
            DeleteFileA( tempPath );
            DeleteFileA( pFtpThread->szLocalFile.c_str() );
            result = true;
        }
    }

    FileNode  fileNode;
    fileNode.remotePath = pFtpThread->szRemoteFile;
    fileNode.localPath = pFtpThread->szLocalFile;
    fileNode.fileLength = pFtpThread->fileLength;
    fileNode.progress = progress;
    fileNode.dirFlag = 0;

    if ( result == true )
    {
        
        fileNode.state = FinishedDownload;
        fileNode.IsFinished = true;
    }
    else
    {
        fileNode.state = StopedDownload;
        fileNode.IsFinished = false;
    }
    
    if ( m_downedFuncCallback != NULL )
    {
        m_downedFuncCallback( m_downedFucnContext, &fileNode );
    }

    return result;
}


int CFtpClient::SetDownLoadState( DownLoadStates downLoadState )
{
    if ( downLoadState == STARTINGDOWNLOAD )
    {
        return FM_FAILD;
    }

    m_downLoadState = downLoadState;

    if ( m_eventHandle != NULL )
    {
        ::SetEvent( m_eventHandle );
    }

    m_threadControl.Join();

    return FM_SUCCESS;
}


bool CFtpClient::DeleteRemoteFile( const char* remoteFilePath )
{
    if ( m_isValidConnect == false )
    {
        if ( ConnectFtpServer() == false )
        {
            return false;
        }
    }

    unsigned int resCode = 0;
    if ( SendCommand( kFtpDele, remoteFilePath ) )
    {
        RecvRespond( resCode );
        if ( resCode == FTP_RETCODE_FILE_OP_OK )
        {
            return TRUE;
        }
    }
    return FALSE;
}


bool CFtpClient::DeleteRemoteDir( const char* remoteDirPath )
{
    if ( m_isValidConnect == false )
    {
        if ( ConnectFtpServer() == false )
        {
            return false;
        }
    }

    unsigned int resCode = 0;
    if ( SendCommand( kFtpRdm, remoteDirPath ) )
    {
        RecvRespond( resCode );
        if ( resCode == FTP_RETCODE_READY )
        {
            return TRUE;
        }
    }
    return FALSE;
}