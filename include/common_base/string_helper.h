///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   string_helper.h 
/// 	@brief	��Ҫ��װ�ַ�����ص�ʵ�ò���
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
*	\brief  �ṩ�ַ�����ص�ʵ�ò��� 
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
*   ����ɾ���ַ��������пո���ַ���
*/
std::string RemoveWhitespaceA( const std::string &str );
std::wstring RemoveWhitespaceW(const std::wstring &str);

/**
*   �ж��ַ��Ƿ�Ϊ����
*/
bool IsDigitA( char c );
bool IsDigitW(wchar_t c);

/**
*   �ж������ַ����Ƿ��Ϊ����
*/
bool IsDigitA( const std::string &str );
bool IsDigitW(const std::wstring &str);

/**
*   �ж��ַ��Ƿ�Ϊ��ĸ
*/
bool IsAlphaA( char c );
bool IsAlphaW(wchar_t c);

/**
*   ���ش�д���ַ���,����ASCII���ַ����Բ����иı�
*/
std::string ToUpperA(const std::string &str);
std::wstring ToUpperW( const std::wstring &str );

/**
*   ����Сд���ַ���,����ASCII���ַ����Բ����иı�
*/
std::string ToLowerA(const std::string &str);
std::wstring ToLowerW(const std::wstring &str);

/**
*   �������ַ������ұߵĿո��Ʊ�������з�ȥ��
*   �磺�����ַ�����" \r\tmystring\n "
*   ��᷵�أ�" \r\tmystring"
*/
std::string TrimRightA( const std::string &str );
std::wstring TrimRightW(const std::wstring &str);

/**
*   �������ַ�������ߵĿո��Ʊ�������з�ȥ��
*   �磺�����ַ�����" \r\tmystring\n "
*   ��᷵�أ�"mystring\n "
*/
std::string TrimLeftA( const std::string &str );
std::wstring TrimLeftW(const std::wstring &str);

/**
*   �������ַ�����ǰβ�ո��Ʊ�������з�ȥ��
*   �磺�����ַ�����" \r\tmystring\n"
*   ��᷵�أ�"mystring"
*/
std::string TrimA( const std::string &str );
std::wstring TrimW(const std::wstring &str);

/**
*   ������ַ�����ָ���ָ������кϲ�������������ء�
*   �磺�ַ������飺{"ab","123","#$%"}�ָ���Ϊ"--" ��ô���Ϊ�����ַ���
*   "ab--123--#$%"
*/
std::string JoinStringA( const ArrString &arrStr, const std::string &strDelimiter );
std::wstring JoinStringW(const ArrWString &arrStr, const std::wstring &strDelimiter);
	
/**
*   ���ַ�����ָ���ָ��������з֣�����������ء�
*   �磺�ַ�,"ab,123,#$%" �ָ���Ϊ",$" ��ô���Ϊ�����ĸ��ַ���
*   "ab"  "123" "#" "%"
*/
bool SplitStringA(const std::string& strSource, const std::string& strDelims, ArrString& arrResult);
bool SplitStringW(const std::wstring& strSource, const std::wstring& strDelims, ArrWString& arrResult);


/**
*   �ж������ַ����Ƿ��Ϊ����������
*/
bool IsPDigitA(const std::string &str);
bool IsPDigitW( const std::wstring &str );

/**
*   �ж������ַ����Ƿ�Ϊ�����������������
*/
bool IsPDoubleDigitA(const std::string &str);
bool IsPDoubleDigitW( const std::wstring &str );

	
/**
*   �����ַ��Ӵ�(subStr)��Ŀ���ַ���(srcStr)�У������ִ�Сд��
*/
bool FindSubStrA(const char *srcStr, const char *subStr);
bool FindSubStrW( const wchar_t *srcStr, const wchar_t *subStr );

/**
*   �����ַ��Ӵ�(subStr)��Ŀ���ַ���(srcStr)�Ƿ���ȣ������ִ�Сд��
*/
bool FindSubStrAEqualA(const char *srcStr, const char *subStr);
bool FindSubStrAEqualW( const wchar_t *srcStr, const wchar_t *subStr );

/*
* \brief �Զ����ʽ���ַ���
*/
std::string StringFormatA( const char *format, ... );
std::wstring StringFormatW( const wchar_t *format, ... );


/*
* \brief ���ֽڴ�ת��ΪUnicode���ַ���
* \TODO: ���ܻ��׳�bad_alloc�쳣
*/
std::wstring MBCS2Unicode( const char * str );


/*
* \brief Unicode���ַ���ת��Ϊ���ֽڴ�
* \TODO: ���ܻ��׳�bad_alloc�쳣
*/
std::string Unicode2MBCS( const wchar_t * wstr );


/*
* \brief UTF8ת��Unicode
* \TODO: ���ܻ��׳�bad_alloc�쳣
*/
std::wstring UTF8ToUnicode( const char* str );

/*
* \brief Unicodeת��UTF8
* \TODO: ���ܻ��׳�bad_alloc�쳣
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