///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   gdiplus_guard.h 
/// 	@brief	GDI+封装. 
/// 
///		主要实现基本GDI+加载、卸载封装  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/03  15:16:02
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _GDIPLUS_GUARD_H_
#define     _GDIPLUS_GUARD_H_


#include <GdiPlus.h>

class CGdiplusGuard
{
public:

    CGdiplusGuard()
    {
        m_isOk = ::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL) == Gdiplus::Ok? TRUE : FALSE;
    }

    virtual ~CGdiplusGuard()
    {
        ::GdiplusShutdown(m_gdiplusToken);
    }

    inline BOOL isOk()
    {
        return m_isOk;
    }

private:

    CGdiplusGuard(const CGdiplusGuard &);

    CGdiplusGuard & operator = (const CGdiplusGuard &);

private:
    
    GdiplusStartupInput     m_gdiplusStartupInput;

    ULONG_PTR               m_gdiplusToken;

    BOOL                    m_isOk;
};

#endif  //  _GDIPLUS_GUARD_H_