//////////////////////////////////////////////////////////////////////////  
///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   utility.h 
/// 	@brief	提供必要的宏. 
///  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/11/01  10:38:45
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _UTILITY_H_
#define     _UTILITY_H_


#include <string>
#include <xstring>
#include <sstream>
#include <limits>
#include <cmath>
#include <cassert>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include "Types.h"

/**  
*  \brief  macros
*/  


/** \brief  return when expression is false */
#define RET_IF_FALSE(f) if (!(f)) {return;}

/** \brief  return var when expression is false */
#define RET_VAR_IF_FALSE(f, var) if (!(f)) {return (var);}

/** \brief  assert fail and return when expression is false */
#define RET_IF_FAIL(f) if (!(f)) {assert(0); return;}

/** \brief  assert fail and return var when expression is false */
#define RET_VAR_IF_FAIL(f, var) if (!(f)) {assert(0); return (var);}

#define DELETE_POINTER(p) if (p){ delete p; p=nullptr;}
#define DELETE_ARRY_POINTER(p) if (p){ delete[] p; p=nullptr; }


#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif


#ifdef UNICODE
typedef std::wstring stlstring;
typedef const std::wstring stlstring_const;

typedef std::wstringstream stlstrstream;
#else
typedef std::string stlstring;
typedef const std::string stlstring_const;

typedef std::stringstream stlstrstream;
#endif

#endif  //  _UTILITY_H_
