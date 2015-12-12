#include "stdafx.h"
#include "Log.h"
#include "string_helper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
* \brief �����µ�Log�ļ�
* \param[in] szLogFile log�ļ�����,��Ҫ��д·����·���̶�Ϊ��ִ���ļ�Ŀ¼��log�ļ�����
* \param[in] bTruncate TRUEÿ�ζ����´����ļ���FALSE����ļ����ڣ����޸�֮ǰ������
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
* \brief д��Log��Ϣ
* \param[in] logType Log����
* \param[in] szModule ģ�����ƣ�������ʾ����Log���ĸ�ģ��д���
* \param[in] szLog Log����
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