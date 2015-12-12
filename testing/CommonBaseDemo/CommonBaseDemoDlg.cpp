
// CommonBaseDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommonBaseDemo.h"
#include "CommonBaseDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma comment(lib, "Version.lib")

const static  TCHAR *pSharedMemName = _T( "MySharedMem" );

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CCommonBaseDemoDlg dialog



CCommonBaseDemoDlg::CCommonBaseDemoDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CCommonBaseDemoDlg::IDD, pParent), m_rwSharedMemory(pSharedMemName)
    , m_sharedMemStr( _T( "点击\"执行分享内存\"，即可分享编辑框中的数据!" ) )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCommonBaseDemoDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange( pDX );
    DDX_Control( pDX, IDC_LIST_ITEM, m_listBox );
    DDX_Text( pDX, IDC_SHARED_MEM_EDIT, m_sharedMemStr );
}

#define WM_MY_MSG1 WM_USER + 101
#define WM_MY_MSG2 WM_USER + 102

BEGIN_MESSAGE_MAP(CCommonBaseDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_EXCUTE, &CCommonBaseDemoDlg::OnBnClickedButtonExcute)
    ON_BN_CLICKED(IDC_TASK_EXCUTE, &CCommonBaseDemoDlg::OnBnClickedTaskExcute)
    ON_MESSAGE(WM_MY_MSG1, &CCommonBaseDemoDlg::OnSendMessage)
    ON_MESSAGE(WM_MY_MSG2, &CCommonBaseDemoDlg::OnPostMessage)
    ON_BN_CLICKED(IDC_DUMP_CRASH, &CCommonBaseDemoDlg::OnBnClickedDumpCrash)
    ON_BN_CLICKED( IDC_SMART_STRING, &CCommonBaseDemoDlg::OnBnClickedSmartString )
    ON_BN_CLICKED( IDC_HEAP, &CCommonBaseDemoDlg::OnBnClickedHeap )
    ON_BN_CLICKED( IDC_SHARED_MEM_BTN, &CCommonBaseDemoDlg::OnBnClickedSharedMemBtn )
    ON_BN_CLICKED( IDC_THREAD_BTN, &CCommonBaseDemoDlg::OnBnClickedThreadBtn )
END_MESSAGE_MAP()


// CCommonBaseDemoDlg message handlers

BOOL CCommonBaseDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

void CCommonBaseDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCommonBaseDemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCommonBaseDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCommonBaseDemoDlg::InsertItem(int itemIndex)
{
    CString tempStr;
    for (unsigned int index = 0; index < 10; ++index)
    {
        tempStr.Format(_T("AddItem%d_%d"), itemIndex, index + 1);
        m_listBox.InsertString(-1, tempStr);
    }
}


CCommonBaseDemoDlg * globalDemoObj = NULL;


BOOL CCommonBaseDemoDlg::AddItem1(void * pParamter)
{
    InsertItem(1);
    return TRUE;
}



BOOL CCommonBaseDemoDlg::AddItem2(void * pParamter)
{
    InsertItem(2);
    return TRUE;
}


BOOL CCommonBaseDemoDlg::AddItem3(void * pParamter)
{
    InsertItem(3);
    return TRUE;
}


BOOL AddItem4(void * pParamter)
{
    if (globalDemoObj == NULL)
    {
        return FALSE;
    }

    globalDemoObj->InsertItem(4);
    return TRUE;
}


BOOL AddItem5(void * pParamter)
{
    if (globalDemoObj == NULL)
    {
        return FALSE;
    }

    globalDemoObj->InsertItem(5);
    return TRUE;
}


void CCommonBaseDemoDlg::OnBnClickedButtonExcute()
{
    // TODO: Add your control notification handler code here

    globalDemoObj = this;

    m_eventsSource.ClearAll();

    m_eventsSource.AppendDelegate(MakeDelegateT2(this, &CCommonBaseDemoDlg::AddItem1) );
    m_eventsSource += MakeDelegateT2(this, &CCommonBaseDemoDlg::AddItem2);
    m_eventsSource += MakeDelegateT2(this, &CCommonBaseDemoDlg::AddItem3);
    m_eventsSource.AppendDelegate(MakeDelegateNoneObject2(AddItem4) );
    m_eventsSource += MakeDelegateNoneObject2(AddItem5);
    
    m_eventsSource -= MakeDelegateT2(this, &CCommonBaseDemoDlg::AddItem2);
    m_eventsSource -= MakeDelegateNoneObject2(AddItem5);

    m_eventsSource(NULL);
}


BOOL CCommonBaseDemoDlg::ExcuteProcess1(const CTaskBase*)
{
    InsertItem(10);
    return TRUE;
}


void CCommonBaseDemoDlg::ExcuteNotify1(const CTaskBase*, CTaskBase::TEvent)
{
    InsertItem(11);
}


BOOL CCommonBaseDemoDlg::ExcuteProcess2(const CTaskBase*)
{
    InsertItem(20);
    return FALSE;
}


void CCommonBaseDemoDlg::ExcuteNotify2(const CTaskBase*, CTaskBase::TEvent)
{
    InsertItem(21);
}


LRESULT CCommonBaseDemoDlg::ExcuteSendUIMsg3(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    InsertItem(30);
    return 1;
}


LRESULT CCommonBaseDemoDlg::ExcutePostUIMsg3(UINT Msg, WPARAM wParam, LPARAM lParam)
{
    InsertItem(31);
    return 1;
}


LRESULT CCommonBaseDemoDlg::OnSendMessage(WPARAM wParam, LPARAM lParam)
{
    InsertItem(40);
    return 1;
}


LRESULT CCommonBaseDemoDlg::OnPostMessage(WPARAM wParam, LPARAM lParam)
{
    InsertItem(41);
    return 1;
}


void CCommonBaseDemoDlg::OnBnClickedTaskExcute()
{
    // TODO: Add your control notification handler code here
    m_taskManager.AddTask(MakeTask(1, this, &CCommonBaseDemoDlg::ExcuteProcess1, &CCommonBaseDemoDlg::ExcuteNotify1) );
    
    m_taskManager.AddTask(MakeTask(2, this, &CCommonBaseDemoDlg::ExcuteProcess2, &CCommonBaseDemoDlg::ExcuteNotify2) );

    CTaskBase * taskObj = MakeTask(3, this, TRUE, &CCommonBaseDemoDlg::ExcuteSendUIMsg3);

    //taskObj->SetUITask(TRUE);
    taskObj->SetUITaskFlag(WM_MY_MSG1, TRUE);
    m_taskManager.AddTask(taskObj);

    taskObj = MakeTask(4, this, FALSE, &CCommonBaseDemoDlg::ExcutePostUIMsg3);

    taskObj->SetUITask(TRUE);
    taskObj->SetUITaskFlag(WM_MY_MSG2, TRUE);
    m_taskManager.AddTask(taskObj);

    m_taskManager.Startup();
}


void CCommonBaseDemoDlg::OnBnClickedDumpCrash()
{
    // TODO: Add your control notification handler code here
    
    int a = 1, b = 0, c;
    c = a / b;

    printf(NULL);
}


void CCommonBaseDemoDlg::OnBnClickedSmartString()
{
    // TODO: Add your control notification handler code here
    m_smartStringA = "haomiao,take it easy!";
    m_smartStringA += "yeah, i just wanner to test!";
    m_smartStringA += CSmartStringA( "ok, now let it go!" );
    const char * pStr = m_smartStringA.GetStr();

    m_smartStringA.AppendStr( CSmartStringA( "hello!Man,go with me!" ) );
    m_smartStringA.AppendStr( "now test the appendStr!" );
    m_smartStringA.Insert( 8, "so the next world will be realloced!" );

    m_smartStringA.AppendStr( "now we retest it!" );
    m_smartStringA.Insert( 0, "we are beginning!" );
    m_smartStringA.isEmpty();

    const CSmartStringA tempStr("add your name here!");
    m_smartStringA = tempStr;
    int len = m_smartStringA.GetStrLength();
    m_smartStringA.AppendStr( "so we return back and with many sorrows, i have no words to say!" );

    m_smartStringA = tempStr;
    m_smartStringA.ReleaseExtraMem();
    m_smartStringA.AppendStr( "so we return back and with many sorrows, i have no words to say!" );
    m_smartStringA.Format( "no useful words, add add addd adddd adddddd,today you get %d days, and %0.5f dollars!", 20, 12.434354324f );


    CSmartStringW m_smartStringW = L"haomiao,take it easy!";
    m_smartStringW += L"yeah, i just wanner to test!";
    m_smartStringW += CSmartStringW( L"ok, now let it go!" );
    const wchar_t * pWStr = m_smartStringW.GetStr();

    m_smartStringW.AppendStr( CSmartStringW( L"hello!Man,go with me!" ) );
    m_smartStringW.AppendStr( L"now test the appendStr!" );
    m_smartStringW.Insert( 8, L"so the next world will be realloced!" );

    m_smartStringW.AppendStr( L"now we retest it!" );
    m_smartStringW.Insert( 0, L"we are beginning!" );
    m_smartStringW.isEmpty();

    const CSmartStringW tempStrW( L"add your name here!" );
    m_smartStringW = tempStrW;
    int lenW = m_smartStringW.GetStrLength();
    m_smartStringW.AppendStr( L"so we return back and with many sorrows, i have no words to say!" );

    m_smartStringW = tempStrW;
    m_smartStringW.ReleaseExtraMem();
    m_smartStringW.AppendStr( L"so we return back and with many sorrows, i have no words to say!" );
    m_smartStringW.Format( L"no useful words, add add addd adddd adddddd,today you get %d days, and %0.5f dollars!", 20, 12.434354324f );

    CSmartStringA tempStrA1 = "haomiao";
    CSmartStringA tempStrA2 = "haomiao";
    BOOL res = tempStrA1 == tempStrA2;
    res = m_smartStringA == tempStrA1;
    res = m_smartStringA == m_smartStringA;

    CSmartStringW tempStrW1 = L"haomiao";
    CSmartStringW tempStrW2 = L"haomiao";
    res = tempStrW1 == tempStrW2;
    res = m_smartStringW == tempStrW1;
    res = m_smartStringW == m_smartStringW;

    std::string strA = StringFormatA( "now it's %d, %0.5f", 25, 54.4324745f );
    std::wstring strW = StringFormatW( L"now it's %d, %0.5f", 25, 54.43242323f );

    std::wstring strW2 = MBCS2Unicode( strA.c_str() );

    strW2 = GetSystemErrorDesc( WSANOTINITIALISED);

    res = IsProcessExist( 5388 );
    res = IsProcessExist( _T( "PA校准系数更新启动工具" ) );
    res = IsProcessExist( NULL, _T( "TXGuiFoundation" ) );
    res = IsProcessExist2( _T( "QQEIM.exe" ) );

    CString tempCStr = GetModulePath(NULL);
    tempCStr = GetFileName( _T( "C:\\file\\data.dat" ) );
    tempCStr = GetFilePath( _T( "C:\\file\\data.dat" ) );

    HWND hWnd = GetProcessTopWindowWnd(5388);
    hWnd = GetProcessTopWindowWnd( _T( "PA校准系数更新启动工具" ) );
    hWnd = GetProcessTopWindowWnd( NULL, _T( "TXGuiFoundation" ) );
    hWnd = GetProcessTopWindowWnd2( _T( "QQEIM.exe" ) );

    HANDLE handle = GetProcessHandle( 5388 );
    handle = GetProcessHandle( _T( "PA校准系数更新启动工具" ) );
    handle = GetProcessHandle( NULL, _T( "TXGuiFoundation" ) );
    handle = GetProcessHandle2( _T( "QQEIM.exe" ) );

    DWORD nVer[4] = { 0 };
    GetOSInfo( nVer );
}


void CCommonBaseDemoDlg::OnBnClickedHeap()
{
    // TODO: Add your control notification handler code here

    HeapBufferStringA heapStrA(m_processHeap, 32);
    SIZE_T size = heapStrA.Size();
    strcpy_s( heapStrA, 32, "haomiao" );
    heapStrA.ReAlloc( 64 );
    size = heapStrA.Size();
    strcat_s( heapStrA, 64, "-hello!" );

    HeapBufferStringW heapStrW( m_processHeap, 32 );
    size = heapStrW.Size();
    wcscpy_s( heapStrW, 32, L"haomiao" );
    heapStrW.ReAlloc( 64 );
    size = heapStrW.Size();
    wcscat_s( heapStrW, 64, L"-hello!" );

    MemBufferPtr pBufferFar;
    {
        MemBufferPtr pBuffer = new CMemBuffer();
        pBuffer->Alloc( 24 );
        pBuffer->ReAlloc( 36 );
        pBufferFar = pBuffer;
    }
    pBufferFar->ReAlloc( 25 );
}


void CCommonBaseDemoDlg::OnBnClickedSharedMemBtn()
{
    // TODO: Add your control notification handler code here
    UpdateData( TRUE );
    m_rwSharedMemory.WriteMemory( m_sharedMemStr.GetString(), m_sharedMemStr.GetLength() * sizeof(TCHAR), 0 );
}


class MyThread :public CThread
{
public:

    MyThread( const std::string &nameStr ) :CThread( nameStr )
    {

    };

    virtual void Run()
    {
        std::wstring nameStr = L"当前线程: ";
        nameStr += GetThreadNameW();
        globalDemoObj->m_listBox.InsertString( -1, nameStr.c_str() );
        unsigned int count = 10;
        while ( count --)
        {
            globalDemoObj->InsertItem( 100 );
            Sleep( 500 );
        }
    }
};


void CCommonBaseDemoDlg::OnBnClickedThreadBtn()
{
    // TODO: Add your control notification handler code here

    globalDemoObj = this;

    ThreadPtr threadPtr = new MyThread( "ThreadOne" );
    CThreadControl threadCtr =  threadPtr->Start( 0 );
}
