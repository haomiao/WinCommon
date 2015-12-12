///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   log.h  
/// 	@brief	��־����. 
/// 
///		��Ҫ�򵥵���־����  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/24  15:33:54
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _LOG_H_
#define     _LOG_H_


#include <afxmt.h>
#include <vector>
#include <map>
#include <string>
#include "utility.h"


/**
* \brief Log���ͣ���Ӧ���ַ���������CLog::m_TypeNames��
*/
enum LOG_TYPE
{
	LOG_TYPE_ERROR,
	LOG_TYPE_INFO
};


class CLog
{
public:

	/** 
	* \brief �����µ�Log�ļ�
	* \param[in] szLogFile log�ļ�����,��Ҫ��д·����·���̶�Ϊ��ִ���ļ�Ŀ¼��log�ļ�����
	* \param[in] bTruncate TRUEÿ�ζ����´����ļ���FALSE����ļ����ڣ����޸�֮ǰ������
	* \param[out] 
	*/
	CLog(LPCTSTR szLogFile, BOOL bTruncate = TRUE);

    ~CLog();

	/** 
	* \brief д��Log��Ϣ
	* \param[in] logType Log����
	* \param[in] szModule ģ�����ƣ�������ʾ����Log���ĸ�ģ��д���
	* \param[in] szLog Log����
	*/
	bool  WriteString(LOG_TYPE logType, LPCTSTR szModule, LPCTSTR szLog);
	
private:

    CFile   m_logFile;

	CCriticalSection	m_Section;

	std::map<LOG_TYPE, CString> m_TypeNames;

};

#endif  //  _LOG_H_