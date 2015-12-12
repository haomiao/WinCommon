
// CommonBaseDemoDlg.h : header file
//

#pragma once

#include "delegate.h"
#include "task_manager.h"
#include "smart_string.h"
#include "process_heap.h"
#include "file_mapping.h"
#include "file_helper.h"
#include "string_helper.h"
#include "windows_helper.h"
#include "mem_buffer.h"
#include "thread.h"

#include "afxwin.h"


// CCommonBaseDemoDlg dialog
class CCommonBaseDemoDlg : public CDialog
{
// Construction
public:
	CCommonBaseDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

public:

    void InsertItem(int itemIndex);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedButtonExcute();
	DECLARE_MESSAGE_MAP()

    BOOL AddItem1(void * pParamter);
    BOOL AddItem2(void * pParamter);
    BOOL AddItem3(void * pParamter);

    BOOL ExcuteProcess1(const CTaskBase*);
    void ExcuteNotify1(const CTaskBase*, CTaskBase::TEvent);

    BOOL ExcuteProcess2(const CTaskBase*);
    void ExcuteNotify2(const CTaskBase*, CTaskBase::TEvent);

    LRESULT ExcuteSendUIMsg3(UINT Msg, WPARAM wParam, LPARAM lParam);
    LRESULT ExcutePostUIMsg3(UINT Msg, WPARAM wParam, LPARAM lParam);

    afx_msg LRESULT OnSendMessage(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnPostMessage(WPARAM wParam, LPARAM lParam);

private:

    CDelegateEventSource    m_eventsSource;

    CTaskManager            m_taskManager;

    CSmartStringA           m_smartStringA;

    CProcessHeap            m_processHeap;

    CRWSharedMemory         m_rwSharedMemory;

public:

    CListBox                m_listBox;
    afx_msg void OnBnClickedTaskExcute();
    afx_msg void OnBnClickedDumpCrash();
    afx_msg void OnBnClickedSmartString();
    afx_msg void OnBnClickedHeap();
    afx_msg void OnBnClickedSharedMemBtn();
    CString m_sharedMemStr;
    afx_msg void OnBnClickedThreadBtn();
};
