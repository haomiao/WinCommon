#include "stdafx.h"
#include "string_helper.h"

bool SplitStringA(const std::string& strSource, const std::string& strDelims, std::vector<std::string>& arrResult)
{
	arrResult.clear();
    std::string::size_type begIndex = strSource.find_first_not_of( strDelims );
    std::string::size_type endIndex;
    while ( begIndex != std::string::npos )
	{
		endIndex = strSource.find_first_of(strDelims, begIndex);
        if ( std::string::npos == endIndex )
		{
			endIndex = strSource.length();
		}

		arrResult.push_back(strSource.substr( begIndex, endIndex - begIndex ));

		begIndex = strSource.find_first_not_of(strDelims, endIndex);
	}
		
	return true;
}


bool SplitStringW(const std::wstring& strSource, const std::wstring& strDelims, std::vector<std::wstring>& arrResult)
{
	arrResult.clear();
    std::wstring::size_type begIndex = strSource.find_first_not_of( strDelims );
    std::wstring::size_type endIndex;
    while ( begIndex != std::wstring::npos )
	{
		endIndex = strSource.find_first_of(strDelims, begIndex);
        if ( std::wstring::npos == endIndex )
		{
			endIndex = strSource.length();
		}

		arrResult.push_back(strSource.substr( begIndex, endIndex - begIndex ));

		begIndex = strSource.find_first_not_of(strDelims, endIndex);
	}

	return true;
}


std::string JoinStringA(const std::vector<std::string> &arrStr, const std::string &strDelimiter)
{
    std::ostringstream out;
    for ( std::vector<std::string>::size_type i = 0; i < arrStr.size(); i++ )
	{
		if(i != 0)
		{
			out << strDelimiter;
		}
		out << arrStr[i];
	}
	return out.str();
}


std::wstring JoinStringW(const std::vector<std::wstring> &arrStr, const std::wstring &strDelimiter)
{
    std::wostringstream out;
    for ( std::vector<std::wstring>::size_type i = 0; i < arrStr.size(); i++ )
	{
		if(i != 0)
		{
			out << strDelimiter;
		}
		out << arrStr[i];
	}
	return out.str();
}


std::wstring TrimW(const std::wstring &str)
{
    static const std::wstring strDest = L" \t\r\n";
	std::wstring::size_type begIndex = str.find_first_not_of(strDest);
	if ( std::wstring::npos == begIndex )
	{
		return L"";
	}
	else
	{
		return str.substr(begIndex, str.find_last_not_of(strDest) - begIndex + 1 );
	}
}


std::string TrimA(const std::string &str)
{
    static const std::string strDest = " \t\r\n";
	std::string::size_type begIndex = str.find_first_not_of(strDest);
	if ( std::string::npos == begIndex )
	{
		return "";
	}
	else
	{
		return str.substr(begIndex, str.find_last_not_of(strDest) - begIndex + 1 );
	}
}


std::wstring TrimLeftW(const std::wstring &str)
{
    static const std::wstring strDest = L" \t\r\n";
	std::wstring::size_type begIndex = str.find_first_not_of(strDest);
	if ( std::wstring::npos == begIndex )
	{
		return L"";
	}
	else
	{
		return str.substr( begIndex );
	}
}


std::string TrimLeftA(const std::string &str)
{
    static const std::string strDest = " \t\r\n";
	std::string::size_type begIndex = str.find_first_not_of(strDest);
	if ( std::string::npos == begIndex )
	{
		return "";
	}
	else
	{
		return str.substr( begIndex );
	}
}


std::wstring TrimRightW( const std::wstring &str )
{
    static const std::wstring strDest = L" \t\r\n";
	std::wstring::size_type endIndex = str.find_last_not_of(strDest);
	if ( std::wstring::npos == endIndex )
	{
		return L"";
	}
	else
	{
		return str.substr( 0, endIndex + 1);
	}
}


std::string TrimRightA( const std::string &str )
{
    static const std::string strDest = " \t\r\n";
	std::string::size_type endIndex = str.find_last_not_of(strDest);
	if ( std::string::npos == endIndex )
	{
		return "";
	}
	else
	{
		return str.substr( 0, endIndex + 1);
	}
}


std::string ToLowerA(const std::string &str)
{
    std::string result;
	result.reserve(str.size());
    for ( std::string::size_type i = 0; i < str.length(); ++i )
	{
		if(isascii(str[i]))
		{
			result += tolower(static_cast<unsigned char>(str[i]));
		}
		else
		{
			result += str[i];
		}
	}
	return result;
}


std::wstring ToLowerW(const std::wstring &str)
{
    std::wstring result;
	result.reserve(str.size());
    for ( std::wstring::size_type i = 0; i < str.length(); ++i )
	{
		if(iswascii(str[i]) )
		{
			result += towlower(str[i]);
		}
		else
		{
			result += str[i];
		}
	}
	return result;
}


std::string ToUpperA(const std::string &str)
{
    std::string result;
	result.reserve(str.size());
    for ( std::string::size_type i = 0; i < str.length(); ++i )
	{
		if(isascii(str[i]))
		{
			result += toupper(static_cast<unsigned char>(str[i]));
		}
		else
		{
			result += str[i];
		}
	}
	return result;
}


std::wstring ToUpperW( const std::wstring &str )
{
    std::wstring result;
    result.reserve( str.size() );
    for ( std::wstring::size_type i = 0; i < str.length(); ++i )
    {
        if ( iswascii( str[i] ) )
        {
            result += towupper( static_cast<wchar_t>( str[i] ) );
        }
        else
        {
            result += str[i];
        }
    }
    return result;
}


bool IsAlphaW(wchar_t c)
{
		return (c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z');
}


bool IsAlphaA(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


bool IsDigitW(wchar_t c)
{
	return c >= L'0' && c <= L'9';
}


bool IsDigitA(char c)
{
	return c >= '0' && c <= '9';
}


bool IsDigitW( const std::wstring &str )
{
    for ( std::wstring::size_type i = 0; i < str.size(); i++ )
	{
		if ( 0 == i && str[0] == L'-')
			continue;
		if ( !IsDigitW(str[i] ) )
			return false;
	}
	return true;
}


bool IsDigitA( const std::string &str )
{
    for ( std::string::size_type i = 0; i < str.size(); i++ )
	{
		if ( 0 == i && str[0] == '-')
			continue;
		if ( !IsDigitA(str[i] ) )
			return false;
	}
	return true;
}


bool IsPDigitA(const std::string &str)
{
	for ( std::string::size_type i = 0; i < str.size(); i ++ )
	{
		if ( !IsDigitA(str[i] ) )
			return false;
	}
	return true;
}


bool IsPDigitW( const std::wstring &str )
{
    for ( std::wstring::size_type i = 0; i < str.size(); i++ )
    {
        if ( !IsDigitW( str[i] ) )
            return false;
    }
    return true;
}


bool IsPDoubleDigitA(const std::string &str)
{
	int pointFlag = 0;
	int zeroFlag  = 0;
	for ( std::string::size_type i = 0; i < str.size(); ++i)
	{
		if (str[i] == '.' && i != 0)
		{
			++pointFlag;
			if (pointFlag > 1)	return false;
			continue;
		}
		if (str[i] == '0' && (i == 0 || (i == 1 && str[i - 1] == '0') ) )
		{
			++zeroFlag;
			if (zeroFlag > 1)	return false;
		}
		if ( !IsDigitA(str[i]) )	return false;
	}
	return true;
}


bool IsPDoubleDigitW( const std::wstring &str )
{
    int pointFlag = 0;
    int zeroFlag = 0;
    for ( std::wstring::size_type i = 0; i < str.size(); ++i )
    {
        if ( str[i] == L'.' && i != 0 )
        {
            ++pointFlag;
            if ( pointFlag > 1 )	return false;
            continue;
        }
        if ( str[i] == L'0' && ( i == 0 || ( i == 1 && str[i - 1] == L'0' ) ) )
        {
            ++zeroFlag;
            if ( zeroFlag > 1 )	return false;
        }
        if ( !IsDigitW( str[i] ) )	return false;
    }
    return true;
}


std::wstring RemoveWhitespaceW(const std::wstring &str)
{
    std::wstring result;
    for ( std::wstring::size_type i = 0; i < str.length(); ++i )
	{
		if(!iswspace(static_cast<wint_t>(str[i])))
		{
			result += str[i];
		}
	}
	return result;
}


std::string RemoveWhitespaceA(const std::string &str)
{
    std::string result;
    for ( std::string::size_type i = 0; i < str.length(); ++i )
	{
		if(!isspace(static_cast<unsigned char>(str[i])))
		{
			result += str[i];
		}
	}
	return result;
}


char * MystruprA(char * sourceStr)
{
	if (sourceStr != NULL)
	{
		int SrcLen = strlen(sourceStr);
		for (int index = 0; index < SrcLen; ++index)
		{
			sourceStr[index] = toupper(sourceStr[index]);
		}
	}
	return sourceStr;
}


wchar_t * MystruprW( wchar_t * sourceStr )
{
    if ( sourceStr != NULL )
    {
        int SrcLen = wcslen( sourceStr );
        for ( int index = 0; index < SrcLen; ++index )
        {
            sourceStr[index] = towupper( sourceStr[index] );
        }
    }
    return sourceStr;
}


bool FindSubStrA(const char *srcStr, const char *subStr)
{
	int srcStrLen = strlen(srcStr);
	int cmpStrLen = strlen(subStr);
	char * tempSrcStr = new char[srcStrLen + 1];
	char * tempCmpStr = new char[cmpStrLen + 1];
	strcpy_s(tempSrcStr, srcStrLen + 1, srcStr);
	strcpy_s(tempCmpStr, cmpStrLen + 1, subStr);

	tempSrcStr = MystruprA(tempSrcStr);
	tempCmpStr = MystruprA(tempCmpStr);
	bool res = false;

    if (strstr(tempSrcStr, tempCmpStr) == NULL)
    {
        res = false;
    }
    else res = true;

	delete []tempSrcStr;
	delete []tempCmpStr;
	return res;
}


bool FindSubStrW( const wchar_t *srcStr, const wchar_t *subStr )
{
    int srcStrLen = wcslen( srcStr );
    int cmpStrLen = wcslen( subStr );
    wchar_t * tempSrcStr = new wchar_t[srcStrLen + 1];
    wchar_t * tempCmpStr = new wchar_t[cmpStrLen + 1];
    wcscpy_s( tempSrcStr, srcStrLen + 1, srcStr );
    wcscpy_s( tempCmpStr, cmpStrLen + 1, subStr );

    tempSrcStr = MystruprW( tempSrcStr );
    tempCmpStr = MystruprW( tempCmpStr );
    bool res = false;

    if ( wcsstr( tempSrcStr, tempCmpStr ) == NULL )
    {
        res = false;
    }
    else res = true;

    delete[]tempSrcStr;
    delete[]tempCmpStr;
    return res;
}


bool FindSubStrAEqualA(const char *srcStr, const char *subStr)
{
    int srcStrLen = strlen(srcStr);
    int cmpStrLen = strlen(subStr);
    char * tempSrcStr = new char[srcStrLen + 1];
    char * tempCmpStr = new char[cmpStrLen + 1];
    strcpy_s(tempSrcStr, srcStrLen + 1, srcStr);
    strcpy_s(tempCmpStr, cmpStrLen + 1, subStr);

    tempSrcStr = MystruprA(tempSrcStr);
    tempCmpStr = MystruprA(tempCmpStr);
    bool res = false;

    if (strcmp(tempSrcStr, tempCmpStr) == 0)
    {
        res = true;
    }
    else
    {
        res = false;
    }

    delete []tempSrcStr;
    delete []tempCmpStr;
    return res;
}


bool FindSubStrAEqualW( const wchar_t *srcStr, const wchar_t *subStr )
{
    int srcStrLen = wcslen( srcStr );
    int cmpStrLen = wcslen( subStr );
    wchar_t * tempSrcStr = new wchar_t[srcStrLen + 1];
    wchar_t * tempCmpStr = new wchar_t[cmpStrLen + 1];
    wcscpy_s( tempSrcStr, srcStrLen + 1, srcStr );
    wcscpy_s( tempCmpStr, cmpStrLen + 1, subStr );

    tempSrcStr = MystruprW( tempSrcStr );
    tempCmpStr = MystruprW( tempCmpStr );
    bool res = false;

    if ( wcscmp( tempSrcStr, tempCmpStr ) == 0 )
    {
        res = true;
    }
    else
    {
        res = false;
    }

    delete[]tempSrcStr;
    delete[]tempCmpStr;
    return res;
}


std::string StringFormatA( const char *format, ... )
{
    if ( NULL == format )
    {
        return "";
    }

    std::vector<char> tmp;
    va_list vl;

    /*获取字符串字符数*/
    va_start( vl, format );
    int count = _vscprintf( format, vl );
    va_end( vl );

    tmp.resize( count + 1 );

    /*将格式化后的字符串写入tmp中*/
    va_start( vl, format );
    vsprintf_s( &tmp[0], count + 1, format, vl );
    va_end( vl );

    return &tmp[0];
}


std::wstring StringFormatW( const wchar_t *format, ... )
{
    if ( NULL == format )
    {
        return L"";
    }

    std::vector<wchar_t> tmp;
    va_list vl;

    /*获取字符串字符数*/
    va_start( vl, format );
    int count = _vscwprintf( format, vl );
    va_end( vl );

    tmp.resize( count + 1 );

    /*将格式化后的字符串写入tmp中*/
    va_start( vl, format );
    vswprintf_s( &tmp[0], count + 1, format, vl );
    va_end( vl );

    return &tmp[0];
}


/*多字节串转化为Unicode宽字符串*/
std::wstring MBCS2Unicode( const char *str )
{
    if ( str == NULL )	return L"";

    int nLen = static_cast<int>( strlen( str ) * 2 + 2 );
    std::vector<wchar_t> tmp(nLen + 1);

    /*转化为wstring格式字符串*/
    MultiByteToWideChar( CP_ACP, 0, str, -1, &tmp[0], nLen );
    return &tmp[0];
}


/*Unicode宽字符串转化为多字节串*/
std::string Unicode2MBCS( const wchar_t * wstr )
{
    if ( wstr == NULL )		return "";
    BOOL bUsedDefaultChar = FALSE;

    /*获取到转化长度*/
    int size = WideCharToMultiByte( CP_ACP, 0, wstr, -1, NULL, 0, NULL, &bUsedDefaultChar );

    if ( size <= 0 )	return "";

    std::vector<char> tmp( size + 1);

    /*转化为string字符串*/
    WideCharToMultiByte( CP_ACP, 0, wstr, -1, &tmp[0], size, NULL, &bUsedDefaultChar );
    return &tmp[0];
}


/*UTF8格式串转化为Unicode宽字符串*/
std::wstring UTF8ToUnicode( const char* str )
{
    if ( str == NULL )	return L"";

    /*获取到转化长度*/
    int  nLen = MultiByteToWideChar( CP_UTF8, 0, str, -1, NULL, 0 );

    if ( nLen <= 0 )	return L"";

    std::vector<wchar_t> tmp( nLen + 1);

    /*转化为UTF8格式字符串*/
    MultiByteToWideChar( CP_UTF8, 0, str, -1, &tmp[0], nLen );
    return &tmp[0];
}


/*Unicode宽字符串转化为UTF8格式串*/
std::string UnicodeToUTF8( const wchar_t* wstr )
{
    if ( wstr == NULL )		return "";
    BOOL bUsedDefaultChar = FALSE;

    /*获取到转化长度*/
    int size = WideCharToMultiByte( CP_UTF8, 0, wstr, -1, NULL, 0, NULL, &bUsedDefaultChar );

    if ( size <= 0 )	return "";

    std::vector<char> tmp( size + 1 );

    /*转化为string字符串*/
    WideCharToMultiByte( CP_UTF8, 0, wstr, -1, &tmp[0], size, NULL, &bUsedDefaultChar );
    return &tmp[0];
}