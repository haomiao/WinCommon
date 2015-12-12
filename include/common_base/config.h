///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   config.h
/// 	@brief	配置相关. 
/// 
///		主要提供工程配置相关、结构、宏等  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/07  9:56:19
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _CONFIG_H_
#define     _CONFIG_H_

//
// The Win Common Base version.
//
//  A.B.C.D : major minor patch building
#define WIN_COMMON_BASE_STRING_VERSION "0.0.1.0"    // "A.B.C.D"
#define WIN_COMMON_BASE_UNIT_VERSION 00000100       // AABBCCDD (十六进制表示)

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#if defined(WIN_COMMON_API_EXPORTS)
#define WIN_COMMON_API DLL_EXPORT
#elif defined(WIN_COMMON_STATIC_LIBS)
#define WIN_COMMON_API
#else
#define WIN_COMMON_API DLL_IMPORT
#endif

// 
// By deriving from this class, other classes are made non-copyable.
//
class WIN_COMMON_API NoneCopyable
{
protected:

    NoneCopyable()
    {

    }

    ~NoneCopyable()
    {

    }

private:

    NoneCopyable( const NoneCopyable& );
    const NoneCopyable& operator=( const NoneCopyable& );
};


#define DISALLOW_COPY_AND_ASSIGN(className) \
private:\
    className(const className &); \
    void operator=(const className &) const

#define ARRAYSIZE_UNSAFE(a) \
	((sizeof(a) / sizeof(*(a))) / \
	static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#endif  //  _CONFIG_H_