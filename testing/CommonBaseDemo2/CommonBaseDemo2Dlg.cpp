XAccessible

// CommonBaseDemo2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommonBaseDemo2.h"
#include "CommonBaseDemo2Dlg.h"
#include "afxdialogex.h"

#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


namespace
{ 
    const static  TCHAR *pSharedMemName = _T( "MySharedMem" );
    HHOOK   hGHook = NULL;
    HWND    hGHwnd = NULL;
    CPoint  point;
    #define WM_HOOK_MSG     WM_USER + 101
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCommonBaseDemo2Dlg dialog



CCommonBaseDemo2Dlg::CCommonBaseDemo2Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCommonBaseDemo2Dlg::IDD, pParent)
    , m_sharedMemStr( _T( "点击\"获取共享内存数据\"，即可获取其他进程分享的共享内存数据!" ) ),
    m_rwSharedMemory(pSharedMemName)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    WSADATA wsaData;
    WSAStartup( MAKEWORD( 2, 2 ), &wsaData );
}


CCommonBaseDemo2Dlg::~CCommonBaseDemo2Dlg()
{
    WSACleanup();
}


void CCommonBaseDemo2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_SHARED_MEM_EDIT, m_sharedMemStr );
}

BEGIN_MESSAGE_MAP(CCommonBaseDemo2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
    ON_WM_CLOSE()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED( IDC_SHARED_MEM_BTN, &CCommonBaseDemo2Dlg::OnBnClickedSharedMemBtn )
    ON_BN_CLICKED( IDC_COMMON_BTN, &CCommonBaseDemo2Dlg::OnBnClickedCommonBtn )
    ON_BN_CLICKED( IDC_EXEC_SERVER, &CCommonBaseDemo2Dlg::OnBnClickedExecServer )
    ON_BN_CLICKED( IDC_FTP_BTN, &CCommonBaseDemo2Dlg::OnBnClickedFtpBtn )
    ON_BN_CLICKED( IDC_HOOK_BTN, &CCommonBaseDemo2Dlg::OnBnClickedHookBtn )
    ON_BN_CLICKED( IDC_UNHOOK_BTN, &CCommonBaseDemo2Dlg::OnBnClickedUnhookBtn )
    ON_MESSAGE(WM_HOOK_MSG, &CCommonBaseDemo2Dlg::OnUpdateHookText)
    ON_BN_CLICKED( IDC_CREATE_WND, &CCommonBaseDemo2Dlg::OnBnClickedCreateWnd )
END_MESSAGE_MAP()


// CCommonBaseDemo2Dlg message handlers

BOOL CCommonBaseDemo2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCommonBaseDemo2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCommonBaseDemo2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCommonBaseDemo2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCommonBaseDemo2Dlg::OnClose()
{
    if (m_windowWnd.IsWindowValid() == TRUE)
    {
        m_windowWnd.CloseWindow( 0 );
    }
    
    CDialogEx::OnClose();
}


void CCommonBaseDemo2Dlg::OnBnClickedSharedMemBtn()
{
    // TODO: Add your control notification handler code here

    LPTSTR lpszStr = m_sharedMemStr.GetBuffer( 1024 );
    m_rwSharedMemory.ReadMemory( lpszStr, 1024, 0, 1024 );
    m_sharedMemStr.ReleaseBuffer();

    UpdateData( FALSE );
}


void CCommonBaseDemo2Dlg::RunTcpServer()
{
    SOCKET socketHandle = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
    if ( socketHandle == INVALID_SOCKET )
    {
        return;
    }

    bool reuseAddr = true;
    int res = ::setsockopt( socketHandle, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseAddr,
        sizeof( bool ) );

    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    saServer.sin_port = htons(5151);

    res = ::bind( socketHandle, (SOCKADDR*)&saServer, sizeof( saServer ) );
    if ( res == SOCKET_ERROR )
    {
        ::closesocket( socketHandle );
        return;
    }

    res = ::listen( socketHandle, 5 );
    if ( res == SOCKET_ERROR )
    {
        ::closesocket( socketHandle );
        return;
    }

    printf( "TCPServer Started!\n" );

    char RecvBuf[1024];
    int  BufLen = 1024;
    char sendBuf[1024] = "Hello,Welcome to TCPServer!";
    while ( true )
    {
        SOCKET clientSocketHandle = ::accept( socketHandle, NULL, NULL );
        memset( RecvBuf, 0, sizeof( RecvBuf ) );
        int rees = recv( clientSocketHandle, RecvBuf, BufLen, 0 );
        if ( rees == SOCKET_ERROR )
        {
            ::closesocket( clientSocketHandle );
            break;
        }
        printf( RecvBuf );
        printf( "\n" );

        rees = send( clientSocketHandle, sendBuf, strlen( sendBuf ), 0 );
        if ( rees == SOCKET_ERROR )
        {
            ::closesocket( clientSocketHandle );
            break;
        }
        ::closesocket( clientSocketHandle );
    }
}


void CCommonBaseDemo2Dlg::RunUdpServer()
{
    SOCKET socketHandle = ::socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if ( socketHandle == INVALID_SOCKET )
    {
        return;
    }

    bool reuseAddr = true;
    int res = ::setsockopt( socketHandle, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseAddr,
        sizeof( bool ) );

    struct sockaddr_in saServer;
    saServer.sin_family = AF_INET;
    saServer.sin_addr.s_addr = inet_addr( "127.0.0.1" );
    saServer.sin_port = htons( 5150 );

    res = ::bind( socketHandle, (SOCKADDR*)&saServer, sizeof( saServer ) );
    if ( res == SOCKET_ERROR )
    {
        ::closesocket( socketHandle );
        return;
    }

    printf( "UDPServer Started!\n" );

    sockaddr_in SenderAddr;
    char RecvBuf[1024];
    int  BufLen = 1024;
    int SenderAddrSize = sizeof( SenderAddr );
    char sendBuf[1024] = "Hello,Welcome to UDPServer!";
    while ( true )
    {
        memset( RecvBuf, 0, sizeof( RecvBuf ) );
        int rees = recvfrom( socketHandle, RecvBuf, BufLen, 0,
            (SOCKADDR *)&SenderAddr, &SenderAddrSize );
        if ( rees == SOCKET_ERROR )
        {
            ::closesocket( socketHandle );
            break;
        }
        printf( RecvBuf );
        printf( "\n" );

        rees = sendto( socketHandle, sendBuf, strlen( sendBuf ), 0, (SOCKADDR *)&SenderAddr, SenderAddrSize );
        if ( rees == SOCKET_ERROR )
        {
            ::closesocket( socketHandle );
            break;
        }
    }
}


void CCommonBaseDemo2Dlg::RunTcpClient()
{
    EtherCommArg commArg = { "127.0.0.1", 5151 };
    TcpComm tcpComm;
    tcpComm.Open( &commArg );
    char buffer[1024] = "Hello Server, I'm visiting on you!";
    char recvBuffer[1024] = { 0 };
    memset( recvBuffer, 0, sizeof( recvBuffer ) );
    tcpComm.Write( buffer, strlen( buffer ) );
    tcpComm.Read( recvBuffer, sizeof( recvBuffer ) );
    tcpComm.Close();

    AutoLock lock(&m_lock);
    m_sharedMemStr += MBCS2Unicode( recvBuffer ).c_str();
    m_sharedMemStr += _T( "\r\n" );
}


void CCommonBaseDemo2Dlg::RunUdpClient()
{
    EtherCommArg commArg = { "127.0.0.1", 5150 };
    UdpComm dupComm;
    dupComm.Open( &commArg );
    char buffer[1024] = "Hello Server, I'm visiting on you!";
    char recvBuffer[1024] = { 0 };
    memset( recvBuffer, 0, sizeof( recvBuffer ) );
    dupComm.Write( buffer, strlen( buffer ) );
    dupComm.Read( recvBuffer, sizeof( recvBuffer ) );
    dupComm.Close();

    AutoLock lock( &m_lock );
    m_sharedMemStr += MBCS2Unicode( recvBuffer ).c_str();
    m_sharedMemStr += _T( "\r\n" );
}


void CCommonBaseDemo2Dlg::OnBnClickedCommonBtn()
{
    // TODO: Add your control notification handler code here

    CMyThread<CCommonBaseDemo2Dlg> *tcpClientThread =
        new CMyThread<CCommonBaseDemo2Dlg>( this );
    CMyThread<CCommonBaseDemo2Dlg> *udpClientThread =
        new CMyThread<CCommonBaseDemo2Dlg>( this );

    tcpClientThread->SetSetUpFunc( &CCommonBaseDemo2Dlg::RunTcpClient );
    udpClientThread->SetSetUpFunc( &CCommonBaseDemo2Dlg::RunUdpClient );

    CThreadControl tcpControl = tcpClientThread->Start();
    CThreadControl udpControl = udpClientThread->Start();

    tcpControl.Join();
    udpControl.Join();
    UpdateData( FALSE );
}


void CCommonBaseDemo2Dlg::OnBnClickedExecServer()
{
    // TODO: Add your control notification handler code here
    CMyThread<CCommonBaseDemo2Dlg> *tcpServerThread =
        new CMyThread<CCommonBaseDemo2Dlg>( this );
    CMyThread<CCommonBaseDemo2Dlg> *udpServerThread =
        new CMyThread<CCommonBaseDemo2Dlg>( this );

    tcpServerThread->SetSetUpFunc( &CCommonBaseDemo2Dlg::RunTcpServer );
    udpServerThread->SetSetUpFunc( &CCommonBaseDemo2Dlg::RunUdpServer );

    tcpServerThread->Start();
    udpServerThread->Start();
}


void CCommonBaseDemo2Dlg::OnBnClickedFtpBtn()
{
    // TODO: Add your control notification handler code here
    CFtpClient ftpClient;
    if (ftpClient.ConnectServer("127.0.0.1", 21) != true)
    {
        return;
    }
    
    std::vector<FILEINFO> fileInfoVector;
    char path[MAX_PATH] = ".";
    int res = ftpClient.ListRemoteFile( path, fileInfoVector );
    if (res != FM_SUCCESS)
    {
        return;
    }

    strcpy_s( path, "\\Release" );
    res = ftpClient.ListRemoteFile( path, fileInfoVector );
    if ( res != FM_SUCCESS )
    {
        return;
    }

    std::string remoteFile = "\\Release\\";
    remoteFile += Unicode2MBCS( fileInfoVector[0].FileName );

    std::string localFile = "C:\\Users\\zhouhao\\Desktop\\NewFile_";
    localFile += Unicode2MBCS( fileInfoVector[0].FileName );;
    res = ftpClient.DownLoadDirOrFile( remoteFile.c_str(), localFile.c_str(), true);
    if (res != FM_SUCCESS)
    {
        return;
    }

    remoteFile = "\\pyvisa-master\\";
    localFile = "C:\\Users\\zhouhao\\Desktop\\pyvisa-master\\";
    res = ftpClient.DownLoadDirOrFile( remoteFile.c_str(), localFile.c_str(), true );
    if ( res != FM_SUCCESS )
    {
        MessageBox( _T( "下载异常!" ), _T( "提示" ), MB_OK );
        return;
    }
    MessageBox( _T( "下载完成!" ), _T( "提示" ), MB_OK );
}


LRESULT CALLBACK MouseProc( int nCode, WPARAM wParam, LPARAM lParam )
{
    if ( wParam == WM_MOUSEMOVE || wParam == WM_NCMOUSEMOVE )
    {
        point = ( (MOUSEHOOKSTRUCT *)lParam )->pt;
        ::SendMessage( hGHwnd, WM_HOOK_MSG , WPARAM(&point), lParam);
    }

    return CallNextHookEx( hGHook, nCode, wParam, lParam );
}


LRESULT CCommonBaseDemo2Dlg::OnUpdateHookText( WPARAM wParam, LPARAM lParam )
{
    CPoint *pPoint = (CPoint *)wParam;
    ScreenToClient( pPoint );
    CString tempStr;
    tempStr.Format( _T( "pt: x = %d, y = %d" ), pPoint->x, pPoint->y );
    GetDlgItem( IDC_STATIC_HOOK )->SetWindowText( tempStr );
    return 0;
}


void CCommonBaseDemo2Dlg::OnBnClickedHookBtn()
{
    // TODO: Add your control notification handler code here
    hGHwnd = GetSafeHwnd();
    hGHook = CSingletonHook::GetInstance()->AddHook( WH_MOUSE, MouseProc, 0, GetCurrentThreadId() );
}


void CCommonBaseDemo2Dlg::OnBnClickedUnhookBtn()
{
    // TODO: Add your control notification handler code here

    CSingletonHook::GetInstance()->ClearAllHook();
}


void CCommonBaseDemo2Dlg::OnBnClickedCreateWnd()
{
    // TODO: Add your control notification handler code here

    //  使用子类窗口处理主窗体消息过程函数,不允许再调用MessageLoop!
    m_winSubWnd.SubClassWnd( m_hWnd );
    m_winSubWnd.ResizeClient( 500, 500 );
    m_winSubWnd.CenterWindow();


    if (m_windowWnd.IsWindowValid() == TRUE)
    {
        return;
    }

    //  创建窗口
    HWND hWnd = m_windowWnd.CreateWnd(_T( "创建的新窗口" ), m_hWnd , WS_VISIBLE | WS_OVERLAPPEDWINDOW, WS_EX_WINDOWEDGE);
    if (hWnd == NULL)
    {
        return;
    }

    m_windowWnd.ResizeClient( 300, 300 );
    m_windowWnd.ShowWindow( SW_SHOW );
    m_windowWnd.CenterWindow();
    m_windowWnd.MessageLoop();
}
