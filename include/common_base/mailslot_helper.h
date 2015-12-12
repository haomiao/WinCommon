///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   mailslot_helper.h 
/// 	@brief	油槽工具. 
/// 
///		主要实现简单的油槽通信封装  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/28  17:13:54
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _MAILSLOT_HELPER_H_
#define     _MAILSLOT_HELPER_H_

#include <atlbase.h>

class CStringMailslotServerHelper
{
	enum Const
	{
		MAX_BUFFER_SIZE = 512
	};

	char m_buffer[MAX_BUFFER_SIZE];

	HANDLE m_hMailslot;

public:

	CStringMailslotServerHelper(const TCHAR* pszMailslotName, DWORD lReadTimeout):m_hMailslot(INVALID_HANDLE_VALUE)
	{
        std::wstring cstrMailslotPath = _T("\\\\.\\mailslot\\");
        cstrMailslotPath += pszMailslotName;
		m_hMailslot = ::CreateMailslot(cstrMailslotPath.c_str(), 0, 
				lReadTimeout, NULL);
	}

	~CStringMailslotServerHelper()
	{
		if(INVALID_HANDLE_VALUE != m_hMailslot)
			::CloseHandle(m_hMailslot);
	}

    BOOL ReadString(std::wstring& pcstrString)
	{

		USES_CONVERSION;
		
		if(INVALID_HANDLE_VALUE == m_hMailslot)
			return FALSE;

		DWORD nRead;
		if(!ReadFile(m_hMailslot, m_buffer, MAX_BUFFER_SIZE, &nRead, NULL))
			return FALSE;

		m_buffer[MAX_BUFFER_SIZE-1] = '\0';  // 保证结束

		pcstrString = A2W(m_buffer);

		return TRUE;
	}

};


class CStringMailslotClientHelper
{
	HANDLE m_hMailslot;

public:

	CStringMailslotClientHelper(const TCHAR* pszMailslotName, DWORD lReadTimeout):m_hMailslot(INVALID_HANDLE_VALUE)
	{
        std::wstring cstrMailslotPath = _T("\\\\.\\mailslot\\");
        cstrMailslotPath += pszMailslotName;
		m_hMailslot = ::CreateFile(cstrMailslotPath.c_str(), GENERIC_WRITE,
				FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			
	}

	~CStringMailslotClientHelper()
	{
		if(INVALID_HANDLE_VALUE != m_hMailslot)
			::CloseHandle(m_hMailslot);
	}

	BOOL WriteString(const TCHAR* pszString)
	{
		ASSERT(pszString);

		USES_CONVERSION;
		
		if(INVALID_HANDLE_VALUE == m_hMailslot)
			return FALSE;

		DWORD nWrite;
		char* pszCmd = W2A(pszString);
		if(!WriteFile(m_hMailslot, pszCmd, static_cast<DWORD>(strlen(pszCmd)+1), &nWrite, NULL))
			return FALSE;

		return TRUE;
	}

};

#endif // _MAILSLOT_HELPER_H_
