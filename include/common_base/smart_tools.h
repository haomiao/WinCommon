///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   smart_tools.h
/// 	@brief	主要封装一些常用的小巧工具类、
/// 
///		一般用在初始化、加载、卸载、注册等用途.  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/04  14:12:13
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _SMART_TOOLS_H_
#define     _SMART_TOOLS_H_

#include <objbase.h>

//  COM组件初始化与关闭
//  一般只需要作为全局对象或静态对象
class CCOMInit
{
public:

    //  默认加载单线程方式
    CCOMInit()
    {
        m_result = ::CoInitialize( );
    }

    /**
      *  @param  [in] 	dwCoInit    初始化COM并发模型、 线程标识
      */
    CCOMInit( DWORD dwCoInit)
    {
        m_result = ::CoInitializeEx( NULL, dwCoInit );
    }

    ~CCOMInit()
    {
        ::CoUninitialize();
    }

    inline HRESULT GetResult()
    {
        return m_result;
    }

    operator HRESULT()
    {
        return m_result;
    }

private:

    HRESULT     m_result;   //  初始化结果状态(S_OK、S_FALSE、RPC_E_CHANGED_MODE)
};


//  Windsocket DLL 初始化与关闭
class CWinSocketInit
{
public:

    /**
    *  @param  [in] 	wVersionRequested    请求的WinSocket版本
    */
    CWinSocketInit( WORD wVersionRequested = MAKEWORD(2, 2) )
    {
        m_resultErrorCode = ::WSAStartup( wVersionRequested, &m_wsaData );
        //  m_resultErrorCode = WSAGetLastError();
    }

    ~CWinSocketInit()
    {
        if ( m_resultErrorCode == ERROR_SUCCESS)
        {
            ::WSACleanup();
        }
    }

    //  返回初始化结果错误码，ERROR_SUCCESS表示成功
    inline int GetResult()
    {
        return m_resultErrorCode;
    }

    //  返回ERROR_SUCCESS表示成功
    operator int()
    {
        return m_resultErrorCode;
    }

    //  获取当前WinSocket版本信息
    LPWSADATA GetWsaData()
    {
        return &m_wsaData;
    }

private:

    WSADATA m_wsaData;          //  当前WinSocket版本信息
    int     m_resultErrorCode;  //  初始化结果错误码
};


//  应用程序、进程全局唯一实例性控制
//  可采用多种方式(内核资源)：互斥量、共享内存、事件等
//  此处我们采用共享内存
class CAppSingle
{
public:

    /**
    *  @param  [in] 	pAppNameFlag       应用程序、进程名标识
    */
    CAppSingle( LPCTSTR pAppNameFlag )
    {
        m_mapHandle = ::CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 
            0, 4, pAppNameFlag );

        if ( m_mapHandle != NULL && ::GetLastError() != ERROR_ALREADY_EXISTS )
        {
            m_isExisted = FALSE;
        }
        else // 创建失败或已存在均认为已存在
        {
            m_isExisted = TRUE;
        }
    }

    ~CAppSingle()
    {
        if (m_mapHandle != NULL)
        {
            ::CloseHandle( m_mapHandle );
        }
    }

    //  判断是否已存在，True则已存在，False为不存在
    inline BOOL IsExisted()
    {
        return m_isExisted;
    }

    operator BOOL()
    {
        return m_isExisted;
    }

private:

    HANDLE	    m_mapHandle;    //  映射文件句柄
    BOOL        m_isExisted;    //  应用程序、进程是否存在
};


//  设备上下文环境堆栈状态
//  通过该简易实现维护DC状态
class CDCStack
{
public:

    CDCStack(HDC hdc):
        m_hdc(hdc),
        m_saveDCID(::SaveDC( hdc ) )
    {

    }

    ~CDCStack()
    {
        ::RestoreDC( m_hdc, m_saveDCID );
    }

    inline int GetDCID()
    {
        return m_saveDCID;
    }

    operator int()
    {
        return m_saveDCID;
    }

    operator HDC()
    {
        return m_hdc;
    }

private:

    HDC     m_hdc;          //  设备上下文
    int     m_saveDCID;     //  保存的DC对应ID

};

#endif  //  _SMART_TOOLS_H_