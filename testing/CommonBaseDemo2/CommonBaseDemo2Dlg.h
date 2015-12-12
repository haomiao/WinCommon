
// CommonBaseDemo2Dlg.h : header file
//

#pragma once

#include "file_mapping.h"

// CCommonBaseDemo2Dlg dialog
class CCommonBaseDemo2Dlg : public CDialogEx
{
// Construction
public:
	CCommonBaseDemo2Dlg(CWnd* pParent = NULL);	// standard constructor

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
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedSharedMemBtn();

public:

    CRWSharedMemory         m_rwSharedMemory;

    CString                 m_sharedMemStr;
};
