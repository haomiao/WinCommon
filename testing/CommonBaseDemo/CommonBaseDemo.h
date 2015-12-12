
// CommonBaseDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "crash_dump.h"

// CCommonBaseDemoApp:
// See CommonBaseDemo.cpp for the implementation of this class
//

class CCommonBaseDemoApp : public CWinAppEx
{
public:
	CCommonBaseDemoApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

private:

    CCrashDump  m_crashDump;
};

extern CCommonBaseDemoApp theApp;