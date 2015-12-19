IsWindowValid
IsWindowValid
IsWindowValid
IsWindowValid
#include "stdafx.h"
#include "windows_ui_base.h"

#ifdef _DEBUG
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#endif


CWindowWndBase::CWindowWndBase() : 
m_hWnd(NULL), 
m_OldWndProc(::DefWindowProc), 
m_bSubClassed(FALSE),
m_hInstance(NULL)
{

}


CWindowWndBase::~CWindowWndBase()
{

}


HWND CWindowWndBase::GetHWND() const 
{ 
    return m_hWnd; 
}


UINT CWindowWndBase::GetWndClassStyle() const
{
    return 0;
}


LPCTSTR CWindowWndBase::GetWindowClassName() const
{
    return L"CWindowWndBase";
}


LPCTSTR CWindowWndBase::GetSuperClassName() const
{
    return NULL;
}


CWindowWndBase::operator HWND() const
{
    return m_hWnd;
}


HWND CWindowWndBase::CreateWnd( LPCTSTR pstrWindowName, HWND hwndParent, DWORD dwStyle, DWORD dwExStyle )
{
    return CreateWnd( dwExStyle, pstrWindowName, dwStyle, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwndParent, NULL, NULL, ( LPVOID )this );
}


HWND CWindowWndBase::CreateWnd( DWORD dwExStyle,
    LPCTSTR pstrWindowName,
    DWORD dwStyle,
    const RECT rc,
    HWND hwndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam )
{
    return CreateWnd( dwExStyle, pstrWindowName, dwStyle, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, hwndParent, hMenu, hInstance,
        lpParam);
}


HWND CWindowWndBase::CreateWnd(
    DWORD dwExStyle,
    LPCTSTR pstrWindowName,
    DWORD dwStyle,
    int x,
    int y,
    int cx,
    int cy,
    HWND hwndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
    )
{
    UNREFERENCED_PARAMETER( lpParam );

    m_hInstance = hInstance;
    if ( GetSuperClassName() != NULL && !RegisterSuperClass() )
    {
        return NULL;
    }

    if ( GetSuperClassName() == NULL && !RegisterWindowClass() )
    {
        return NULL;
    }

    m_hWnd = ::CreateWindowEx( dwExStyle, GetWindowClassName(), pstrWindowName,
        dwStyle, x, y, cx, cy, hwndParent, hMenu, m_hInstance, ( LPVOID )this );
    return m_hWnd;
}


BOOL CWindowWndBase::SubClassWnd( HWND hWnd )
{
    if ( hWnd == NULL || ::IsWindow( hWnd ) == FALSE || m_hWnd != NULL)
    {
        return FALSE;
    }

    m_OldWndProc = SubclassWindow(hWnd, WndProc);
    if ( m_OldWndProc == NULL )
    {
        return FALSE;
    }

    m_bSubClassed = TRUE;
    m_hWnd = hWnd;
    return ::SetWindowLongPtr( m_hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>( this ) ) != 0;
}


BOOL CWindowWndBase::UnSubClassWnd()
{
    if ( IsWindowValid() != TRUE)
    {
        return FALSE;
    }

    if ( m_bSubClassed == FALSE )
    {
        return FALSE;
    }

    SubclassWindow( m_hWnd, m_OldWndProc );
    m_OldWndProc = ::DefWindowProc;
    m_bSubClassed = FALSE;
    m_hWnd = NULL;
    return TRUE;
}


void CWindowWndBase::ShowWindow( int nShow )
{
    if ( IsWindowValid() != TRUE )
    {
        return;
    }

    ::ShowWindow( m_hWnd, nShow );
}


UINT CWindowWndBase::MessageLoop()
{
    if ( IsWindowValid() != TRUE )
    {
        return -1;
    }

    UINT nRet = 0;
    HWND hWndParent = GetWindowOwner(m_hWnd);
    ::ShowWindow(m_hWnd, SW_SHOWNORMAL);
    if ( hWndParent != NULL)
    {
        ::EnableWindow(hWndParent, FALSE);
    }
    

    MSG msg = { 0 };
    long long int idleCount = 0;
    BOOL resIdle = FALSE;
    for ( ; ; )
    {
        while ( !::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) &&
            IsWindowValid() == TRUE)
        {
            OnIdle( ++idleCount, resIdle );
            ::Sleep( 0 );
            if ( resIdle == TRUE )
            {
                break;
            }
        }

        do 
        {
            if ( ::GetMessage( &msg, NULL, 0, 0 ) &&
                IsWindowValid() == TRUE )
            {
                if ( msg.message == WM_CLOSE && msg.hwnd == m_hWnd )
                {
                    nRet = msg.wParam;
                    if ( hWndParent != NULL )
                    {
                        ::EnableWindow( hWndParent, TRUE );
                    }
                    ::SetFocus( hWndParent );
                }

                if ( FALSE == TranslateMessage( &msg ) )
                {
                    ::TranslateMessage( &msg );
                    ::DispatchMessage( &msg );
                }

                idleCount = 0;
                resIdle = FALSE;
            }
            else
            {
                goto MsgLabel;
            }
        } while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) &&
            IsWindowValid() == TRUE );

        if ( IsWindowValid() == FALSE )
        {
            goto MsgLabel;
        }
    }

MsgLabel:

    if ( hWndParent != NULL )
    {
        ::EnableWindow(hWndParent, TRUE);
        ::SetFocus( hWndParent );
    }

    if ( msg.message == WM_QUIT )
    {
        ::PostQuitMessage( msg.wParam );
    }
    
    return nRet;
}


void CWindowWndBase::CloseWindow( UINT nRet )
{
    if ( IsWindowValid() != TRUE )
    {
        return;
    }

    SendMessage(WM_CLOSE, (WPARAM)nRet, 0L);
}


void CWindowWndBase::CenterWindow()
{
    if ( IsWindowValid() != TRUE && (GetWindowStyle(m_hWnd) & WS_CHILD) )
    {
        return;
    }

    RECT rcDlg = { 0 };
    ::GetWindowRect(m_hWnd, &rcDlg);
    RECT rcArea = { 0 };
    RECT rcCenter = { 0 };
	HWND hWnd = m_hWnd;
    HWND hWndParent = ::GetParent(m_hWnd);
    HWND hWndCenter = ::GetWindowOwner(m_hWnd);
    if ( hWndCenter != NULL )
    {
        hWnd = hWndCenter;
    }

	MONITORINFO oMonitor;
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST), &oMonitor);
	rcArea = oMonitor.rcWork;

    if ( hWndCenter == NULL )
    {
        rcCenter = rcArea;
    }
    else
    {
        ::GetWindowRect( hWndCenter, &rcCenter );
    }

    int DlgWidth = rcDlg.right - rcDlg.left;
    int DlgHeight = rcDlg.bottom - rcDlg.top;

    // Find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - DlgWidth / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - DlgHeight / 2;

    // The dialog is outside the screen, move it inside
    if ( xLeft < rcArea.left )
    {
        xLeft = rcArea.left;
    }
    else if ( xLeft + DlgWidth > rcArea.right )
    {
        xLeft = rcArea.right - DlgWidth;
    }

    if ( yTop < rcArea.top )
    {
        yTop = rcArea.top;
    }
    else if ( yTop + DlgHeight > rcArea.bottom )
    {
        yTop = rcArea.bottom - DlgHeight;
    }

    ::SetWindowPos(m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}


BOOL CWindowWndBase::SetIcon(UINT nResID)
{
    if ( IsWindowValid() != TRUE )
    {
        return FALSE;
    }

    HICON hIcon = ( HICON )::LoadImage( m_hInstance, MAKEINTRESOURCE( nResID ), IMAGE_ICON, ::GetSystemMetrics( SM_CXICON ), ::GetSystemMetrics( SM_CYICON ), LR_DEFAULTCOLOR );

    if (hIcon == NULL)
    {
        return FALSE;
    }

    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);
    hIcon = ( HICON )::LoadImage( m_hInstance, MAKEINTRESOURCE( nResID ), IMAGE_ICON, ::GetSystemMetrics( SM_CXSMICON ), ::GetSystemMetrics( SM_CYSMICON ), LR_DEFAULTCOLOR );

    ::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);

    return TRUE;
}


BOOL CWindowWndBase::RegisterWindowClass()
{
    WNDCLASS wc = { 0 };
    wc.style = GetWndClassStyle();
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hIcon = NULL;
    wc.lpfnWndProc = CWindowWndBase::WndProc;
    wc.hInstance = m_hInstance;
    wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = GetWindowClassName();
    ATOM ret = ::RegisterClass(&wc);
    return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}


BOOL CWindowWndBase::RegisterSuperClass()
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEX);
    if( !::GetClassInfoEx(NULL, GetSuperClassName(), &wc) ) 
    {
        if ( !::GetClassInfoEx( m_hInstance, GetSuperClassName(), &wc ) ) 
        {
            return FALSE;
        }
    }

    m_OldWndProc = wc.lpfnWndProc;
    wc.lpfnWndProc = CWindowWndBase::SuperWndProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = GetWindowClassName();
    ATOM ret = ::RegisterClassEx(&wc);
    return ret != NULL || ::GetLastError() == ERROR_CLASS_ALREADY_EXISTS;
}


LRESULT CALLBACK CWindowWndBase::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CWindowWndBase* pThis = NULL;
    if( uMsg == WM_NCCREATE ) 
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<CWindowWndBase*>(lpcs->lpCreateParams);
        pThis->m_hWnd = hWnd;
        ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LPARAM>(pThis));
    } 
    else 
    {
        pThis = reinterpret_cast<CWindowWndBase*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA) );
        if( uMsg == WM_NCDESTROY && pThis != NULL )
        {
            LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            ::SetWindowLongPtr(pThis->m_hWnd, GWLP_USERDATA, 0L);
            if ( pThis->m_bSubClassed )
            {
                pThis->UnSubClassWnd();
            }

            pThis->m_hWnd = NULL;
            pThis->OnFinalMessage();
            return lRes;
        }
    }

    if( pThis != NULL )
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    } 
    else 
    {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


LRESULT CALLBACK CWindowWndBase::SuperWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    CWindowWndBase* pThis = NULL;
    if( uMsg == WM_NCCREATE )
    {
        LPCREATESTRUCT lpcs = reinterpret_cast<LPCREATESTRUCT>(lParam);
        pThis = static_cast<CWindowWndBase*>(lpcs->lpCreateParams);
        ::SetProp(hWnd, _T("SuperWnd"), (HANDLE) pThis);
        pThis->m_hWnd = hWnd;
    } 
    else
    {
        pThis = reinterpret_cast<CWindowWndBase*>(::GetProp(hWnd, _T("SuperWnd") ) );

        if( uMsg == WM_NCDESTROY && pThis != NULL ) 
        {
            LRESULT lRes = ::CallWindowProc(pThis->m_OldWndProc, hWnd, uMsg, wParam, lParam);
            if ( pThis->m_bSubClassed )
            {
                pThis->UnSubClassWnd();
            }

            ::SetProp(hWnd, _T("SuperWnd"), NULL);
            pThis->m_hWnd = NULL;
            pThis->OnFinalMessage();
            return lRes;
        }
    }

    if( pThis != NULL )
    {
        return pThis->HandleMessage(uMsg, wParam, lParam);
    } 
    else 
    {
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}


LRESULT CWindowWndBase::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( IsWindowValid() != TRUE )
    {
        return -1;
    }

    return ::SendMessage(m_hWnd, uMsg, wParam, lParam);
} 


LRESULT CWindowWndBase::PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    if ( IsWindowValid() != TRUE )
    {
        return -1;
    }

    return ::PostMessage(m_hWnd, uMsg, wParam, lParam);
}


void CWindowWndBase::ResizeClient( int cx , int cy )
{
    if ( IsWindowValid() != TRUE )
    {
        return;
    }

    RECT rc = { 0 };
    if ( !::GetClientRect( m_hWnd, &rc ) )
    {
        return;
    }

    if ( cx != -1 )
    {
        rc.right = cx;
    }
    
    if ( cy != -1 )
    {
        rc.bottom = cy;
    }
    
    if ( !::AdjustWindowRectEx( &rc, GetWindowStyle( m_hWnd ), ( !( GetWindowStyle( m_hWnd ) & WS_CHILD ) && ( ::GetMenu( m_hWnd ) != NULL ) ), GetWindowExStyle( m_hWnd ) ) )
    {
        return;
    }
    
    ::SetWindowPos( m_hWnd, NULL, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE );
}


LRESULT CWindowWndBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return ::CallWindowProc( m_OldWndProc, m_hWnd, uMsg, wParam, lParam );
}


void CWindowWndBase::OnFinalMessage()
{

}


BOOL CWindowWndBase::PreHandleMessage( LPMSG pMsg )
{
    UNREFERENCED_PARAMETER( pMsg );
    return FALSE;
}


BOOL CWindowWndBase::TranslateAccelerator( LPMSG pMsg )
{
    UNREFERENCED_PARAMETER( pMsg );
    return FALSE;
}


BOOL CWindowWndBase::TranslateMessage( LPMSG pMsg )
{
    //  先处理加速键，再处理预处理消息,然后继续分发消息
    BOOL res = TranslateAccelerator(pMsg);
    if ( res == TRUE )
    {
        return res;
    }

    res = PreHandleMessage( pMsg );
    return res;
}


void CWindowWndBase::OnIdle( long long int idleCount, BOOL &res )
{
    UNREFERENCED_PARAMETER( idleCount );

    CString tempStr;
    static long long int tempTotalCount = 0;
    if ( tempTotalCount > 100000 )
    {
        //  若不处理空闲任务,则设置后直接返回
        res = TRUE;
        return;
    }
    else if ( tempTotalCount == 100000 )
    {
        res = TRUE;
        tempStr.Format( _T( "任务已经完成,不再执行任务!" ), idleCount );
        HDC hDc = ::GetDC( m_hWnd );
        ::TextOut( hDc, 10, 10, tempStr, tempStr.GetLength() );
        ::ReleaseDC( m_hWnd, hDc );
        return;
    }

    tempTotalCount += 1;
    
    tempStr.Format( _T( "空闲计数: %10lld" ), idleCount );
    HDC hDc = ::GetDC( m_hWnd );
    ::TextOut( hDc, 10, 10, tempStr, tempStr.GetLength() );
    ::ReleaseDC( m_hWnd, hDc );
}


BOOL CWindowWndBase::IsWindowValid()
{
    if ( m_hWnd == NULL || ::IsWindow( m_hWnd ) == FALSE )
    {
        return FALSE;
    }
    return TRUE;
}