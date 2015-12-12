///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   types.h 
/// 	@brief	自定义、重声明类型. 
///  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/11/03  09:19:25
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _TYPES_H_
#define     _TYPES_H_

typedef char I8;
typedef short I16;
typedef int I32;
typedef long long I64;

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;
typedef unsigned long long U64;

typedef float F32;
typedef double F64;

typedef union { U32 f; F32 d; } UF32;
typedef union { U64 f; F64 d; } UF64;

#endif  //  _TYPES_H_

