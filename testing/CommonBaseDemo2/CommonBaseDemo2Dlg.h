
// CommonBaseDemo2Dlg.h : header file
//

#pragma once

#include "file_mapping.h"
#include "communication_tcp.h"
#include "communication_udp.h"
#include "string_helper.h"
#include "thread.h"
#include "lock_event.h"
#include "auto_lock.h"
#include "ftp_client.h"
#include "hook.h"
#include "windows_ui_base.h"

template<typename T>
class CMyThread: public CThread
{
public:

    CMyThread( T *pDlg ) :
        m_pDlg(pDlg)
    {

    }

    ~CMyThread()
    {

    }

    typedef void (T::*pFunc)();

public:

    void  SetSetUpFunc( pFunc func )
    {
        m_func = func;
    }

public:

    virtual void Run()
    {
        if (m_pDlg != NULL)
        {
            (m_pDlg->*m_func)();
        }
        
    }

private:

    T * m_pDlg;
    pFunc m_func;
};


// CCommonBaseDemo2Dlg dialog
class CCommonBaseDemo2Dlg : public CDialogEx
{
// Construction
public:
	CCommonBaseDemo2Dlg(CWnd* pParent = NULL);	// standard constructor

    virtual ~CCommonBaseDemo2Dlg();

// Dialog Data
	enum { IDD = IDD_DEMO2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg LRESULT OnUpdateHookText(WPARAM wParam, LPARAM lParam);
    afx_msg void OnClose();
	DECLARE_MESSAGE_MAP()

public:

    afx_msg void OnBnClickedSharedMemBtn();
    afx_msg void OnBnClickedCommonBtn();
    afx_msg void OnBnClickedExecServer();

public:

    void RunTcpServer();
    void RunUdpServer();
    void RunTcpClient();
    void RunUdpClient();

private:

    CRWSharedMemory         m_rwSharedMemory;
    CString                 m_sharedMemStr;
    CEventLock              m_lock;
    CWindowWndBase          m_windowWnd;
    CWindowWndBase          m_winSubWnd;

public:

    afx_msg void OnBnClickedFtpBtn();
    afx_msg void OnBnClickedHookBtn();
    afx_msg void OnBnClickedUnhookBtn();
    afx_msg void OnBnClickedCreateWnd();
};
