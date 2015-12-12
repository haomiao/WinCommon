///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   log.h  
/// 	@brief	日志工具. 
/// 
///		主要简单的日志工具  
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
* \brief Log类型，对应的字符串保存在CLog::m_TypeNames中
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
	* \brief 创建新的Log文件
	* \param[in] szLogFile log文件名称,不要填写路径，路径固定为可执行文件目录的log文件夹下
	* \param[in] bTruncate TRUE每次都重新创建文件，FALSE如果文件存在，不修改之前的内容
	* \param[out] 
	*/
	CLog(LPCTSTR szLogFile, BOOL bTruncate = TRUE);

    ~CLog();

	/** 
	* \brief 写入Log信息
	* \param[in] logType Log类型
	* \param[in] szModule 模块名称，用来显示这条Log是哪个模块写入的
	* \param[in] szLog Log内容
	*/
	bool  WriteString(LOG_TYPE logType, LPCTSTR szModule, LPCTSTR szLog);
	
private:

    CFile   m_logFile;

	CCriticalSection	m_Section;

	std::map<LOG_TYPE, CString> m_TypeNames;

};

#endif  //  _LOG_H_