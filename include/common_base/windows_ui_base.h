///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   windows_ui_base.h
/// 	@brief	窗口UI类 
/// 
///		主要实现基于基于Win32窗口创建、消息机制的简单封装  
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

//  窗口类基类
//  若需要实现超类化，则需要继承该类，并实现GetSuperClassName接口
//  此外若要实现更多的操作，如消息处理、分发，绘制等，也可继承当前类
//  此时可根据需要重写当前类被保护的成员函数即可
class CWindowWndBase
{
public:

    CWindowWndBase();

    ~CWindowWndBase();

public:

    /**
    *  @brief  构建窗口
    *  @param  [in] 	dwExStyle               窗口扩展风格
    *  @param  [in] 	pstrWindowName          窗口名
    *  @param  [in] 	dwStyle                 窗口风格
    *  @param  [in] 	rc                      窗口位置区域
    *  @param  [in] 	x                       窗口左上角X坐标
    *  @param  [in] 	y                       窗口左上角y坐标
    *  @param  [in] 	cx                      窗口宽度
    *  @param  [in] 	cy                      窗口高度
    *  @param  [in] 	hwndParent              父窗口句柄
    *  @param  [in] 	hMenu                   菜单句柄
    *  @param  [in] 	hInstance               实例句柄
    *  @return:         返回创建的窗口句柄
    *  @detail:         创建窗口的时候，父窗口为NULL则为非模态，否则为模态窗口
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

    //  获取创建的窗口句柄
    HWND GetHWND() const;
    operator HWND() const;

    //  设置窗口状态
    void ShowWindow( int nShow );

    //  消息循环
    UINT MessageLoop();

    //  关闭窗口(nRet为退出码)
    void CloseWindow( UINT nRet = IDOK );

    //  窗口居中
    void CenterWindow();

    //  设置窗口大小图标(nResID为资源ID)
    BOOL SetIcon( UINT nResID );

    //  发送/推送消息
    LRESULT SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);
    LRESULT PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0L);

    //  重置客户端区域大小
    //  cx:客户区域宽度, cy:客户区域高度
    void ResizeClient(int cx = -1, int cy = -1);

    //  子类化窗口
    BOOL SubClassWnd( HWND hWnd );

    //  取消子类化窗口
    BOOL UnSubClassWnd();

    //  窗口是否有效
    BOOL IsWindowValid();

protected:

    //  获取窗口类名
    virtual LPCTSTR GetWindowClassName() const;

    //  获取超类名
    virtual LPCTSTR GetSuperClassName() const;

    //  获取窗口类风格
    virtual UINT GetWndClassStyle() const;

    //  消息处理函数
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    //  预处理消息(处理策略：本函数处理的返回TRUE，则不再继续传递)
    virtual BOOL PreHandleMessage( LPMSG pMsg );

    //  翻译加速器(处理策略：本函数处理的返回TRUE，则不再继续传递)
    virtual BOOL TranslateAccelerator( LPMSG pMsg );

    //  窗口销毁后的最后一个消息处理函数(WM_NCDESTROY)
    virtual void OnFinalMessage();

    //  处理窗口消息队列空闲时，处理函数(idleCount为每次空闲计数)
    //  注意：一般只用做短小，极简单的任务(可选)
    //  res:设置为TRUE，则表示已完成任务，此外若为FALSE，则表示仍继续执行
    //  此外为了帮助可以执行性执行某些任务，用户需自己确定是否继续执行
    //  详情见OnIdle实现示例代码
    virtual void OnIdle( long long int idleCount, BOOL &res );

private:

    //  处理策略： 先处理加速键，再处理预处理消息,然后继续分发消息
    BOOL TranslateMessage( LPMSG pMsg );

private:

    //  注册窗口类
    BOOL RegisterWindowClass();

    //  注册窗口类(基于超类)
    BOOL RegisterSuperClass();

    //  窗口过程处理函数
    static LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

    //  超类窗口过程处理函数
    static LRESULT CALLBACK SuperWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

protected:

    HWND            m_hWnd;             //  窗口句柄
    WNDPROC         m_OldWndProc;       //  早期窗口过程处理函数
    BOOL            m_bSubClassed;      //  是否子类化
    HINSTANCE       m_hInstance;        //  实例句柄

};


#endif  //  _WINDOWS_UI_BASE_H_
