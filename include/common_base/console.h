///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   Console.h
/// 	@brief	�򵥵Ŀ���̨ 
/// 
///		��Ҫʵ�ֿ���̨���ڣ����ڹ۲�Win32��MFC���������Ϣ  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/09/02  8:51:39
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _CONSOLE_H_
#define     _CONSOLE_H_

#include <io.h>
#include <fcntl.h>
#include <stdio.h>
#include <iostream>

//  �Զ������̨����
class CConsole
{
public:

    CConsole();

    CConsole(LPCTSTR lpszTitle, SHORT ConsoleHeight = 300, SHORT ConsoleWidth = 80);

    ~CConsole();

private:

    void Attach(SHORT ConsoleHeight, SHORT ConsoleWidth);

    static BOOL IsExistent;
};

#endif  //  _CONSOLE_H_