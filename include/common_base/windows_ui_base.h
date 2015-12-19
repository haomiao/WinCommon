///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   windows_ui_base.h
/// 	@brief	����UI�� 
/// 
///		��Ҫʵ�ֻ��ڻ���Win32���ڴ�������Ϣ���Ƶļ򵥷�װ  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/18  12:06:57
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _WINDOWS_UI_BASE_H_
#define     _WINDOWS_UI_BASE_H_

#include <windows.h>
#include <windowsx.h>
#include <string>

//  ���������
//  ����Ҫʵ�ֳ��໯������Ҫ�̳и��࣬��ʵ��GetSuperClassName�ӿ�
//  ������Ҫʵ�ָ���Ĳ���������Ϣ�����ַ������Ƶȣ�Ҳ�ɼ̳е�ǰ��
//  ��ʱ�ɸ�����Ҫ��д��ǰ�౻�����ĳ�Ա��������
class CWindowWndBase
{
public:

    CWindowWndBase();

    ~CWindowWndBase();

public:

    /**
    *  @brief  ��������
    *  @param  [in] 	dwExStyle               ������չ���
    *  @param  [in] 	pstrWindowName          ������
    *  @param  [in] 	dwStyle                 ���ڷ��
    *  @param  [in] 	rc                      ����λ������
    *  @param  [in] 	x                       �������Ͻ�X����
    *  @param  [in] 	y                       �������Ͻ�y����
    *  @param  [in] 	cx                      ���ڿ��
    *  @param  [in] 	cy                      ���ڸ߶�
    *  @param  [in] 	hwndParent              �����ھ��
    *  @param  [in] 	hMenu                   �˵����
    *  @param  [in] 	hInstance               ʵ�����
    *  @return:         ���ش����Ĵ��ھ��
    *  @detail:         �������ڵ�ʱ�򣬸�����ΪNULL��Ϊ��ģ̬������Ϊģ̬����
    */
    HWND CreateWnd( DWORD dwExStyle,
        LPCTSTR pstrWindowName,
        DWORD dwStyle,
        const RECT rc,
        HWND hwndParent = NULL,
        HMENU hMenu = NULL,
        HINSTANCE hInstance = NULL,
        LPVOID lpParam = NULL );

    HWND CreateWnd( DWORD dwExStyle,
        LPCTSTR pstrWindowName,
        DWORD dwStyle,
        int x = CW_USEDEFAULT,
        int y = CW_USEDEFAULT,
        int cx = CW_USEDEFAULT,
        int cy = CW_USEDEFAULT,
        HWND hwndParent = NULL,
        HMENU hMenu = NULL,
        HINSTANCE hInstance = NULL,
        LPVOID lpParam = NULL );

    HWND CreateWnd(LPCTSTR pstrWindowName, HWND hwndParent, DWORD dwStyle =0, DWORD dwExStyle =0);

    //  ��ȡ�����Ĵ��ھ��
    HWND GetHWND() const;
    operator HWND() const;

    //  ���ô���״̬
    void ShowWindow( int nShow );

    //  ��Ϣѭ��
    UINT MessageLoop();

    //  �رմ���(nRetΪ�˳���)
    void CloseWindow( UINT nRet = IDOK );

    //  ���ھ���
    void CenterWindow();

    //  ���ô��ڴ�Сͼ��(nResIDΪ��ԴID)
    BOOL SetIcon( UINT nResID );

    //  ����/������Ϣ
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

    //  ���ÿͻ��������С
    //  cx:�ͻ�������, cy:�ͻ�����߶�
    void ResizeClient(int cx = -1, int cy = -1);

    //  ���໯����
    BOOL SubClassWnd( HWND hWnd );

    //  ȡ�����໯����
    BOOL UnSubClassWnd();

    //  �����Ƿ���Ч
    BOOL IsWindowValid();

protected:

    //  ��ȡ��������
    virtual LPCTSTR GetWindowClassName() const;

    //  ��ȡ������
    virtual LPCTSTR GetSuperClassName() const;

    //  ��ȡ��������
    virtual UINT GetWndClassStyle() const;

    //  ��Ϣ������
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    //  Ԥ������Ϣ(������ԣ�����������ķ���TRUE�����ټ�������)
    virtual BOOL PreHandleMessage( LPMSG pMsg );

    //  ���������(������ԣ�����������ķ���TRUE�����ټ�������)
    virtual BOOL TranslateAccelerator( LPMSG pMsg );

    //  �������ٺ�����һ����Ϣ������(WM_NCDESTROY)
    virtual void OnFinalMessage();

    //  ��������Ϣ���п���ʱ��������(idleCountΪÿ�ο��м���)
    //  ע�⣺һ��ֻ������С�����򵥵�����(��ѡ)
    //  res:����ΪTRUE�����ʾ��������񣬴�����ΪFALSE�����ʾ�Լ���ִ��
    //  ����Ϊ�˰�������ִ����ִ��ĳЩ�����û����Լ�ȷ���Ƿ����ִ��
    //  �����OnIdleʵ��ʾ������
    virtual void OnIdle( long long int idleCount, BOOL &res );

private:

    //  ������ԣ� �ȴ�����ټ����ٴ���Ԥ������Ϣ,Ȼ������ַ���Ϣ
    BOOL TranslateMessage( LPMSG pMsg );

private:

    //  ע�ᴰ����
    BOOL RegisterWindowClass();

    //  ע�ᴰ����(���ڳ���)
    BOOL RegisterSuperClass();

    //  ���ڹ��̴�����
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    //  ���ര�ڹ��̴�����
    static LRESULT CALLBACK SuperWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:

    HWND            m_hWnd;             //  ���ھ��
    WNDPROC         m_OldWndProc;       //  ���ڴ��ڹ��̴�����
    BOOL            m_bSubClassed;      //  �Ƿ����໯
    HINSTANCE       m_hInstance;        //  ʵ�����

};


#endif  //  _WINDOWS_UI_BASE_H_
