
// CommonBaseDemo2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "CommonBaseDemo2.h"
#include "CommonBaseDemo2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const static  TCHAR *pSharedMemName = _T( "MySharedMem" );

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
}

void CCommonBaseDemo2Dlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange( pDX );
    DDX_Text( pDX, IDC_SHARED_MEM_EDIT, m_sharedMemStr );
}

BEGIN_MESSAGE_MAP(CCommonBaseDemo2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED( IDC_SHARED_MEM_BTN, &CCommonBaseDemo2Dlg::OnBnClickedSharedMemBtn )
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



void CCommonBaseDemo2Dlg::OnBnClickedSharedMemBtn()
{
    // TODO: Add your control notification handler code here

    LPTSTR lpszStr = m_sharedMemStr.GetBuffer( 1024 );
    m_rwSharedMemory.ReadMemory( lpszStr, 1024, 0, 1024 );
    m_sharedMemStr.ReleaseBuffer();

    UpdateData( FALSE );
}
