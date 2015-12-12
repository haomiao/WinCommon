///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   debug.h
/// 	@brief	�������. 
/// 
///		��Ҫ�ṩ������صĺꡢ�ӿڡ�������Ϣ 
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/11/27  16:11:19
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _DEBUG_H_
#define     _DEBUG_H_

#define FILE_NAME _T("WinCommonDebug")
#define DBG_MSG_BUFF_SIZE 1024

#define _INF_(fmt,...) \
{ \
    TCHAR dmsg[DBG_MSG_BUFF_SIZE]; \
    _stprintf_s(dmsg, DBG_MSG_BUFF_SIZE, FILE_NAME _T(" : INF : ") fmt _T("\n"), ##__VA_ARGS__); \
    OutputDebugString(dmsg); \
}

#define _WRN_(fmt,...) \
{ \
    TCHAR dmsg[DBG_MSG_BUFF_SIZE]; \
    _stprintf_s(dmsg, DBG_MSG_BUFF_SIZE, FILE_NAME _T(" : WRN : ") __FUNCTIONW__ _T("<%d> : ") fmt _T("\n"), __LINE__, ##__VA_ARGS__); \
    OutputDebugString(dmsg); \
}

#define _ERR_(fmt,...) \
{ \
    TCHAR dmsg[DBG_MSG_BUFF_SIZE]; \
    _stprintf_s(dmsg, DBG_MSG_BUFF_SIZE, FILE_NAME _T(" : ERR : ") __FUNCTIONW__ _T("<%d> : ") fmt _T("\n"), __LINE__, ##__VA_ARGS__); \
    OutputDebugString(dmsg); \
}

#if defined(_DEBUG) || defined(DBG_REL)
#define _DBG_(fmt,...) \
{ \
    TCHAR dmsg[DBG_MSG_BUFF_SIZE]; \
    _stprintf_s(dmsg, DBG_MSG_BUFF_SIZE, FILE_NAME _T(" : DBG : ") __FUNCTIONW__ _T("<%d> : ") fmt _T("\n"), __LINE__, ##__VA_ARGS__); \
    OutputDebugString(dmsg); \
}
#else
#define _DBG_(fmt,...)
#endif

// ����ʱ��Output��ʾ��Ϣ
#define COMPILE_MSG(msg) #pragma message(msg)

#endif  //  _DEBUG_H_
