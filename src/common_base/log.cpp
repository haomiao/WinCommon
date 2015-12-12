#include "stdafx.h"
#include "Log.h"
#include "string_helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
* \brief 创建新的Log文件
* \param[in] szLogFile log文件名称,不要填写路径，路径固定为可执行文件目录的log文件夹下
* \param[in] bTruncate TRUE每次都重新创建文件，FALSE如果文件存在，不修改之前的内容
* \param[out]
*/
CLog::CLog(LPCTSTR szLogFile, BOOL bTruncate/* = TRUE*/)
{
	m_TypeNames[LOG_TYPE_ERROR] = _T("Error");
	m_TypeNames[LOG_TYPE_INFO] = _T("Info");

	CString strLogPath = szLogFile;
	UINT nOpenFlags = CFile::modeCreate |
		CFile::typeBinary |
		CFile::modeWrite |
		CFile::shareDenyWrite;

	if (!bTruncate)
	{
		nOpenFlags |= CFile::modeNoTruncate;
	}

	BOOL bRet = m_logFile.Open(strLogPath, nOpenFlags);
	ASSERT(bRet);
}


CLog::~CLog()
{
    //m_logStrVec.clear();
    if ( m_logFile.m_hFile && m_logFile.m_hFile != INVALID_HANDLE_VALUE )
    {
        m_logFile.Close();
    }
}


/**
* \brief 写入Log信息
* \param[in] logType Log类型
* \param[in] szModule 模块名称，用来显示这条Log是哪个模块写入的
* \param[in] szLog Log内容
*/
bool  CLog::WriteString(LOG_TYPE logType, LPCTSTR szModule, LPCTSTR szLog)
{
    if (m_logFile.m_hFile && m_logFile.m_hFile != INVALID_HANDLE_VALUE)
    {
		m_Section.Lock();

		CString strLog;
		CString strTypeName = _T("");
		if (m_TypeNames.find(logType) != m_TypeNames.end() )
		{
			strTypeName = m_TypeNames[logType];
		}

		CTime t = CTime::GetCurrentTime();
		strLog.Format(_T("[%04d/%02d/%02d %02d:%02d:%02d][%s][%s]%s\r\n"),
			t.GetYear(),
			t.GetMonth(),
			t.GetDay(),
			t.GetHour(),
			t.GetMinute(),
			t.GetSecond(),
			strTypeName,
			szModule,
			szLog);

        std::string strTemp;
		strTemp = Unicode2MBCS(strLog);
        m_logFile.Write(strTemp.c_str(),(UINT)strTemp.length());
        m_logFile.Flush();
		m_Section.Unlock();
        return true;
    }

    return true;
}