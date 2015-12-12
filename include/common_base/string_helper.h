///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   string_helper.h 
/// 	@brief	主要封装字符串相关的实用操作
///  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/06  18:26:17
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _STRING_HELPER_H_
#define     _STRING_HELPER_H_
/**
*	\file string_helper.h
*	\brief  提供字符串相关的实用操作 
*/

#include <sstream>
#include <string>
#include <vector>


#ifdef _UNICODE
	typedef std::wstring TString;
#else
	typedef std::string TString;
#endif//_UNICODE

typedef std::vector<std::wstring> ArrWString;
typedef std::vector<std::string> ArrString;
typedef std::vector<TString> ArrTString;

/*
*   返回删除字符串中所有空格的字符串
*/
std::string RemoveWhitespaceA( const std::string &str );
std::wstring RemoveWhitespaceW(const std::wstring &str);

/**
*   判断字符是否为数字
*/
bool IsDigitA( char c );
bool IsDigitW(wchar_t c);

/**
*   判断整个字符串是否均为数字
*/
bool IsDigitA( const std::string &str );
bool IsDigitW(const std::wstring &str);

/**
*   判断字符是否为字母
*/
bool IsAlphaA( char c );
bool IsAlphaW(wchar_t c);

/**
*   返回大写的字符串,不是ASCII的字符忽略不进行改变
*/
std::string ToUpperA(const std::string &str);
std::wstring ToUpperW( const std::wstring &str );

/**
*   返回小写的字符串,不是ASCII的字符忽略不进行改变
*/
std::string ToLowerA(const std::string &str);
std::wstring ToLowerW(const std::wstring &str);

/**
*   将给定字符串的右边的空格、制表符、换行符去掉
*   如：给定字符串：" \r\tmystring\n "
*   则会返回：" \r\tmystring"
*/
std::string TrimRightA( const std::string &str );
std::wstring TrimRightW(const std::wstring &str);

/**
*   将给定字符串的左边的空格、制表符、换行符去掉
*   如：给定字符串：" \r\tmystring\n "
*   则会返回："mystring\n "
*/
std::string TrimLeftA( const std::string &str );
std::wstring TrimLeftW(const std::wstring &str);

/**
*   将给定字符串的前尾空格、制表符、换行符去掉
*   如：给定字符串：" \r\tmystring\n"
*   则会返回："mystring"
*/
std::string TrimA( const std::string &str );
std::wstring TrimW(const std::wstring &str);

/**
*   将多个字符串按指定分隔符进行合并，并将结果返回。
*   如：字符串数组：{"ab","123","#$%"}分隔符为"--" 那么结果为如下字符串
*   "ab--123--#$%"
*/
std::string JoinStringA( const ArrString &arrStr, const std::string &strDelimiter );
std::wstring JoinStringW(const ArrWString &arrStr, const std::wstring &strDelimiter);
	
/**
*   将字符串按指定分隔符进行切分，并将结果返回。
*   如：字符,"ab,123,#$%" 分隔符为",$" 那么结果为如下四个字符串
*   "ab"  "123" "#" "%"
*/
bool SplitStringA(const std::string& strSource, const std::string& strDelims, ArrString& arrResult);
bool SplitStringW(const std::wstring& strSource, const std::wstring& strDelims, ArrWString& arrResult);


/**
*   判断整个字符串是否均为正整数数字
*/
bool IsPDigitA(const std::string &str);
bool IsPDigitW( const std::wstring &str );

/**
*   判断整个字符串是否为正浮点或正整数数字
*/
bool IsPDoubleDigitA(const std::string &str);
bool IsPDoubleDigitW( const std::wstring &str );

	
/**
*   查找字符子串(subStr)在目标字符串(srcStr)中（不区分大小写）
*/
bool FindSubStrA(const char *srcStr, const char *subStr);
bool FindSubStrW( const wchar_t *srcStr, const wchar_t *subStr );

/**
*   查找字符子串(subStr)在目标字符串(srcStr)是否相等（不区分大小写）
*/
bool FindSubStrAEqualA(const char *srcStr, const char *subStr);
bool FindSubStrAEqualW( const wchar_t *srcStr, const wchar_t *subStr );

/*
* \brief 自定义格式化字符串
*/
std::string StringFormatA( const char *format, ... );
std::wstring StringFormatW( const wchar_t *format, ... );


/*
* \brief 多字节串转化为Unicode宽字符串
* \TODO: 可能会抛出bad_alloc异常
*/
std::wstring MBCS2Unicode( const char * str );


/*
* \brief Unicode宽字符串转化为多字节串
* \TODO: 可能会抛出bad_alloc异常
*/
std::string Unicode2MBCS( const wchar_t * wstr );


/*
* \brief UTF8转成Unicode
* \TODO: 可能会抛出bad_alloc异常
*/
std::wstring UTF8ToUnicode( const char* str );

/*
* \brief Unicode转成UTF8
* \TODO: 可能会抛出bad_alloc异常
*/
std::string UnicodeToUTF8( const wchar_t* wstr );

#ifdef _UNICODE

#define SplitString SplitStringW
#define JoinString JoinStringW
#define Trim TrimW
#define TrimLeft TrimLeftW
#define TrimRight TrimRightW
#define IsDigit IsDigitW
#define IsAlpha	IsAlphaW
#define RemoveWhitespace RemoveWhitespaceW
#define ToUpper ToUpperW
#define ToLower ToLowerW
#define IsPDigit IsPDigitW
#define IsPDoubleDigit IsPDoubleDigitW
#define FindSubStr FindSubStrW
#define FindSubStrAEqual FindSubStrAEqualW

#else

#define SplitString SplitStringA
#define JoinString JoinStringA
#define Trim TrimA
#define TrimLeft TrimLeftA
#define TrimRight TrimRightA
#define IsDigit	IsDigitA
#define IsAlpha IsAlphaA
#define RemoveWhitespace RemoveWhitespaceA
#define ToUpper ToUpperA
#define ToLower ToLowerA
#define IsPDigit IsPDigitA
#define IsPDoubleDigit IsPDoubleDigitA
#define FindSubStr FindSubStrA
#define FindSubStrAEqual FindSubStrAEqualA

#endif

#endif  //      _STRING_HELPER_H_