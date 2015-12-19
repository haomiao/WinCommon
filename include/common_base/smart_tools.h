///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   smart_tools.h
/// 	@brief	��Ҫ��װһЩ���õ�С�ɹ����ࡢ
/// 
///		һ�����ڳ�ʼ�������ء�ж�ء�ע�����;.  
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

//  COM�����ʼ����ر�
//  һ��ֻ��Ҫ��Ϊȫ�ֶ����̬����
class CCOMInit
{
public:

    //  Ĭ�ϼ��ص��̷߳�ʽ
    CCOMInit()
    {
        m_result = ::CoInitialize( );
    }

    /**
      *  @param  [in] 	dwCoInit    ��ʼ��COM����ģ�͡� �̱߳�ʶ
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

    HRESULT     m_result;   //  ��ʼ�����״̬(S_OK��S_FALSE��RPC_E_CHANGED_MODE)
};


//  Windsocket DLL ��ʼ����ر�
class CWinSocketInit
{
public:

    /**
    *  @param  [in] 	wVersionRequested    �����WinSocket�汾
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

    //  ���س�ʼ����������룬ERROR_SUCCESS��ʾ�ɹ�
    inline int GetResult()
    {
        return m_resultErrorCode;
    }

    //  ����ERROR_SUCCESS��ʾ�ɹ�
    operator int()
    {
        return m_resultErrorCode;
    }

    //  ��ȡ��ǰWinSocket�汾��Ϣ
    LPWSADATA GetWsaData()
    {
        return &m_wsaData;
    }

private:

    WSADATA m_wsaData;          //  ��ǰWinSocket�汾��Ϣ
    int     m_resultErrorCode;  //  ��ʼ�����������
};


//  Ӧ�ó��򡢽���ȫ��Ψһʵ���Կ���
//  �ɲ��ö��ַ�ʽ(�ں���Դ)���������������ڴ桢�¼���
//  �˴����ǲ��ù����ڴ�
class CAppSingle
{
public:

    /**
    *  @param  [in] 	pAppNameFlag       Ӧ�ó��򡢽�������ʶ
    */
    CAppSingle( LPCTSTR pAppNameFlag )
    {
        m_mapHandle = ::CreateFileMapping( INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 
            0, 4, pAppNameFlag );

        if ( m_mapHandle != NULL && ::GetLastError() != ERROR_ALREADY_EXISTS )
        {
            m_isExisted = FALSE;
        }
        else // ����ʧ�ܻ��Ѵ��ھ���Ϊ�Ѵ���
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

    //  �ж��Ƿ��Ѵ��ڣ�True���Ѵ��ڣ�FalseΪ������
    inline BOOL IsExisted()
    {
        return m_isExisted;
    }

    operator BOOL()
    {
        return m_isExisted;
    }

private:

    HANDLE	    m_mapHandle;    //  ӳ���ļ����
    BOOL        m_isExisted;    //  Ӧ�ó��򡢽����Ƿ����
};


//  �豸�����Ļ�����ջ״̬
//  ͨ���ü���ʵ��ά��DC״̬
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

    HDC     m_hdc;          //  �豸������
    int     m_saveDCID;     //  �����DC��ӦID

};

#endif  //  _SMART_TOOLS_H_