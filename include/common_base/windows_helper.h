///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   windows_helper.h
/// 	@brief	windows下提供的一些帮助工具. 
/// 
///		主要提供windows下常用的辅助工具、句柄管理、资源管理等  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/03  13:45:58
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _WINDOWS_HELPER_H_
#define     _WINDOWS_HELPER_H_

#include <Windows.h>
#include <tchar.h>
#include "string_helper.h"


/**
*  @brief   判断某进程ID、或(进程顶层窗口)类名、窗口标题对应进程是否存在
*  @param  [in] 	processID             进程ID
*  @param  [in] 	lpWindowName          窗口标题名
*  @param  [in] 	lpClassName           窗口类名
*  @param  [in]     processName           进程名
*  @detail:若采用第二种，则一般情况通过类名或是窗口标题名之一即可,均参数有效时将两者均对应则存在
*          返回找到的第一个进程即可
*/
BOOL IsProcessExist( DWORD processID ) ;
BOOL IsProcessExist( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL);
BOOL IsProcessExist2( LPCTSTR processName );


/**
*  @brief   获取进程顶层窗口句柄
*  @param  [in] 	processID             进程ID
*  @param  [in] 	lpWindowName          窗口标题名
*  @param  [in] 	lpClassName           窗口类名
*  @param  [in]     processName           进程名
*  @detail:返回找到第一个进程即可
*/
HWND GetProcessTopWindowWnd( DWORD processID );
HWND GetProcessTopWindowWnd( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL );
HWND GetProcessTopWindowWnd2( LPCTSTR processName );


/**
*  @brief   获取进程句柄(不使用时，请使用CloseHandle释放)
*  @param  [in] 	processID             进程ID
*  @param  [in] 	lpWindowName          窗口标题名
*  @param  [in] 	lpClassName           窗口类名
*  @param  [in]     processName           进程名
*  @detail:返回找到第一个进程即可
*/
HANDLE GetProcessHandle( DWORD processID );
HANDLE GetProcessHandle( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL );
HANDLE GetProcessHandle2( LPCTSTR processName );


/**
*  @brief   获取错误代码的描述信息
*  @param  [in] 	dwCode           由GetLastError获取的错误码
*  @return 				             返回错误描述信息
*/
TString GetSystemErrorDesc( DWORD dwErrorCode );


/**
*  @brief   获取当前系统信息
*  @param  [out] 	lpSystemInfo     系统信息
*/
void GetCurrentSystemInfo( LPSYSTEM_INFO lpSystemInfo);


/**
*  @brief   获取当前系统处理器架构
*  detail:  PROCESSOR_ARCHITECTURE_AMD64、PROCESSOR_ARCHITECTURE_IA64、PROCESSOR_ARCHITECTURE_INTEL
            PROCESSOR_ARCHITECTURE_UNKNOWN
*/
DWORD GetSystemProcessorArchitecture();


/**
*  @brief   获取当前系统分页大小(一般常在VirtualAlloc中使用)
*/
DWORD GetSystemPageSize();


/**
*  @brief   获取当前系统处理器个数
*/
DWORD GetSystemProcessorNumber();


/**
*  @brief   获取操作系统信息
*  @param  [out] 	lpOSInfo        操作系统信息
*  @param  [out] 	versionMS       操作系统主版本号信息
*  @param  [out] 	versionLS       操作系统次版本号信息
*  @param  [out] 	nVer            操作系统版本号信息
*  @detail：为区分是否为获取InfoEx，传参之前应设置lpOSInfo的dwOSVersionInfoSize参数为相应结构大小
            如： OSVERSIONINFOEX osInfoEx;
                 osInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
                 GetOSInfo( (LPOSVERSIONINFO)&osInfoEx );

                 OSVERSIONINFO osInfo;
                 osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                 GetOSInfo( &osInfo );
            此外微软不推荐使用该接口(兼容问题，后面操作系统无法具体区分)，可换用其他的实现替代如：
            通过Kernel32.dll获取产品主版本号(因其与系统版本一致)
*/
BOOL GetOSInfo( LPOSVERSIONINFO  lpOSInfo);

//  获取操作系统信息, 返回主版本号次版本号、版本格式化信息如:6.1.13.1345
BOOL GetOSInfo( DWORD &versionMS, DWORD &versionLS );
BOOL GetOSInfo( DWORD nVer[4] );

//  此外比较常用的还有GetSystemMetrics、SystemParametersInfo等其他System Information Functions


//////////////////////////////////////////////////////////////////////////
//  以下主要封装常用的windows下的句柄、资源包装

//  模板形式的Handle自动封装
template<HANDLE HANDLE_VALUE>
class AutoHandle
{
public:

    AutoHandle( HANDLE handle = HANDLE_VALUE )
    { 
        Attach( handle );
    }

    AutoHandle( AutoHandle<HANDLE_VALUE> & other )
    { 
        Attach( other.Detach() );
    }

    ~AutoHandle()
    { 
        if ( IsValid() == TRUE)
        {
            ::CloseHandle( m_handle );
        }
    }

    AutoHandle<HANDLE_VALUE>& ReSet( HANDLE handle = HANDLE_VALUE )
    {
        if (handle == m_handle)
        {
            return *this;
        }
        
        Attach( handle );
        return *this;
    }

    AutoHandle<HANDLE_VALUE>& ReSet( AutoHandle<HANDLE_VALUE>& other )
    {
        if ( *this != other )
        {
            ReSet( other.Detach() );
        }

        return *this;
    }

    //  依附句柄
    void Attach( HANDLE handle )
    {
        if (m_handle == handle)
        {
            return;
        }

        if ( m_handle != HANDLE_VALUE )
        {
            ::CloseHandle( Detach() );
        }
        m_handle = handle;
    }

    //  分离句柄(外部应根据实际情况释放返回的句柄资源)
    HANDLE Detach()
    {
        HANDLE handle = m_handle;
        m_handle = HANDLE_VALUE;
        return handle;
    }

    operator HANDLE	() const	
    { 
        return m_handle;
    }

    HANDLE GetHandle() const	
    { 
        return m_handle;
    }

    BOOL IsValid() const
    { 
        return m_handle != HANDLE_VALUE;
    }

    AutoHandle<HANDLE_VALUE>& operator = ( HANDLE handle )
    { 
        return ReSet( handle );
    }

    AutoHandle<HANDLE_VALUE>& operator = ( AutoHandle<HANDLE_VALUE>& other )
    {
        return ReSet( other );
    }

    bool operator == ( HANDLE handle ) const
    { 
        return m_handle == handle;
    }

    bool operator != ( HANDLE handle ) const
    { 
        return !( operator == ( handle ) );
    }

    bool operator == ( const AutoHandle<HANDLE_VALUE>& other ) const
    {
        return m_handle == other.m_handle;
    }

    bool operator != ( const AutoHandle<HANDLE_VALUE>& other ) const
    { 
        return !( operator == ( other ) ); 
    }

private:

    HANDLE	    m_handle;   //  句柄
};


typedef AutoHandle<INVALID_HANDLE_VALUE> AutoFileHandle;     // 文件自动句柄
typedef AutoHandle<NULL>                 AutoResourceHandle; // 资源自动句柄(窗口、进程、线程等资源)


//  HDC自动封装
class AutoDC
{
public:
    AutoDC(HDC hDC = NULL) :m_hDC(hDC), m_isNeedRelease(FALSE), m_hWnd(NULL)
    {

    }

    AutoDC( HWND hWnd )
    {
        m_hDC = ::GetDC( hWnd );
        if (IsValid() == TRUE)
        {
            m_hWnd          = hWnd;
            m_isNeedRelease = TRUE;
        }
    }

    AutoDC( AutoDC& other )
    {
        ReSet( other );
    }

    ~AutoDC()	
    { 
        Release();
    }

    inline BOOL IsNeedRelease()
    {
        return m_isNeedRelease;
    }

    inline void Attach( HDC hDC )
    {
        Release();
        m_hDC = hDC;
        m_hWnd = NULL;
        m_isNeedRelease = FALSE;
    }

    inline void Attach( HWND hWnd )
    {
        HDC hDC = ::GetDC( hWnd );
        Attach( hDC );
        m_hWnd = hWnd;
        m_isNeedRelease = TRUE;
    }

    inline HDC Detach()
    {
        HDC tempHDC = m_hDC;
        m_hDC = NULL;
        m_isNeedRelease = FALSE;
        m_hWnd = NULL;
        return tempHDC;
    }

    operator HDC () const
    {
        return m_hDC;
    }

    inline HDC GetDC() const
    {
        return m_hDC;
    }

    operator HWND() const
    {
        return m_hWnd;
    }

    inline HWND GetHWnd() const
    {
        return m_hWnd;
    }

    inline AutoDC& ReSet( HDC hDC = NULL )
    {
        Attach( hDC );
        return *this;
    }

    inline AutoDC& ReSet( AutoDC& other )
    {
        if ( *this != other )
        {
            HWND hWnd = other.GetHWnd();
            BOOL isNeedRelease = other.IsNeedRelease();
            Attach( other.Detach() );
            m_isNeedRelease = isNeedRelease;
            m_hWnd = hWnd;
        }

        return *this;
    }


    AutoDC& operator = ( HDC hDC )
    {
        return ReSet( hDC );
    }


    AutoDC& operator = ( AutoDC& other )
    {
        return ReSet( other );
    }

    bool operator == ( HDC hDC ) const
    {
        return m_hDC == hDC;
    }

    bool operator != ( HDC hDC ) const
    {
        return !( operator == ( hDC ) );
    }

    bool operator == ( const AutoDC& other ) const
    {
        return operator == (other.GetDC() );
    }

    bool operator != ( const AutoDC& other ) const
    {
        return !( operator == ( other ) );
    }

    inline BOOL IsValid() const
    {
        return m_hDC != NULL;
    }
    
    //////////////////////////////////////////////////////////////////////////
    //  获取当前已选择到HDC中的相应类型的对象句柄

    inline HBITMAP GetHBitmap()
    {
        return (HBITMAP)GetCurrentObject( OBJ_BITMAP );
    }

    inline HBRUSH GetHBrush()
    {
        return (HBRUSH)GetCurrentObject( OBJ_BRUSH );
    }

    inline HCOLORSPACE GetHColorSpace()
    {
        return (HCOLORSPACE)GetCurrentObject( OBJ_COLORSPACE );
    }

    inline HFONT GetHFont()
    {
        return (HFONT)GetCurrentObject( OBJ_FONT );
    }

    inline HPALETTE GetHPalette()
    {
        return (HPALETTE)GetCurrentObject( OBJ_PAL );
    }

    inline HPEN GetHPen()
    {
        return (HPEN)GetCurrentObject( OBJ_PEN );
    }
   
    //////////////////////////////////////////////////////////////////////////
    //  设置将选择到HDC中的相应类型的对象句柄，并返回先前设置的对象句柄

    inline int SelectClipRgn( HRGN hrgn )
    {
        return ::SelectClipRgn( m_hDC, hrgn );
    }

    //  mode:执行剪切区域的操作模式
    inline int ExtSelectClipRgn( HRGN hrgn, int mode )
    {
        return ::ExtSelectClipRgn( m_hDC, hrgn, mode );
    }

    //  bForceBackground:设置前景色或是背景色标识
    inline HPALETTE SelectPalette( HPALETTE hpal, BOOL bForceBackground )
    {
        return ::SelectPalette( m_hDC, hpal, bForceBackground );
    }

    inline HBITMAP SelectHBitmap( HBITMAP hbm )
    {
        return (HBITMAP)SelectObject( hbm );
    }

    inline HBRUSH SelectHBrush( HBRUSH hbr )
    {
        return (HBRUSH)SelectObject( hbr );
    }

    inline HPEN SelectHPen( HPEN hpen )
    {
        return (HPEN)SelectObject( hpen );
    }

    inline HRGN SelectHRgn( HRGN hrgn )
    {
        return (HRGN)SelectObject( hrgn );
    }

    inline HFONT SelectHFont( HFONT hf )
    {
        return (HFONT)SelectObject( hf );
    }

    inline HCOLORSPACE SelectHColorSpace( HCOLORSPACE hColorSpace )
    {
        return (HCOLORSPACE)SelectObject( hColorSpace );
    }

private:

    inline void Release()
    {
        if ( IsValid() == TRUE )
        {
            if ( IsNeedRelease() == TRUE && m_hWnd != NULL )
            {
                ::ReleaseDC( m_hWnd, m_hDC );
            }
        }
    }

    inline HGDIOBJ GetCurrentObject( UINT uObjectType )
    {
        return ::GetCurrentObject( m_hDC, uObjectType );
    }

    inline HGDIOBJ SelectObject( HGDIOBJ hgdiobj )
    {
        return ::SelectObject( m_hDC, hgdiobj );
    }

private:

    HWND        m_hWnd;             //  窗口句柄
    HDC         m_hDC;              //  窗口句柄对应设备上下文句柄
    BOOL        m_isNeedRelease;    //  是否需要释放该设备上下文句柄
};


//  自动绘制DC封装
//  一般用在WIN32编程中相应WM_PAINT消息处理函数中开始绘制和结束绘制的封装
class AutoPaintDC
{
public:
    AutoPaintDC( HWND hwnd ) : m_hwnd( hwnd )
    {
        m_hdc = ::BeginPaint( m_hwnd, &m_ps );
    }

    ~AutoPaintDC()
    {
        ::EndPaint( m_hwnd, &m_ps );
    }

    operator HDC () const
    {
        return m_hdc; 
    }

    inline HDC GetDC() const
    {
        return m_hdc;
    }

    inline BOOL IsValid()	const
    { 
        return m_hdc != NULL;
    }

public:

    PAINTSTRUCT	    m_ps;       //  绘制结结构
    HWND		    m_hwnd;     //  窗口句柄
    HDC			    m_hdc;      //  窗口设备设备上下文
};


//  模板基类，自动GDI对象封装
//  一般不单独使用
template<class T>
class AutoGDIBase
{
public:

    AutoGDIBase( T gdiObj = NULL )
    { 
        Attach( gdiObj );
    }

    AutoGDIBase( AutoGDIBase<T>& other )
    { 
        Attach( other.Deatch() );
    }

    ~AutoGDIBase()
    { 
        if ( m_gdiObj != NULL )
        {
            ::DeleteObject( m_gdiObj );
        }
    }

    AutoGDIBase<T>& ReSet( T gdiObj = NULL )
    {
        if ( gdiObj != m_gdiObj )
        {
            Attach( gdiObj );
        }

        return *this;
    }

    AutoGDIBase<T>& ReSet( AutoGDIBase<T>& other )
    {
        if ( this != &other )
        {
            Attach( other.Deatch() );
        }

        return *this;
    }

    inline void Attach( T gdiObj = NULL )
    {
        if ( IsValid() == TRUE )
        {
            ::DeleteObject( m_gdiObj );
        }

        m_gdiObj = gdiObj;
    }

    inline T Deatch()
    {
        T tempGdiObj = m_gdiObj;
        m_gdiObj = NULL;
        return tempGdiObj;
    }

    AutoGDIBase<T>& operator = ( T gdiObj )
    { 
        return ReSet( gdiObj );
    }

    AutoGDIBase<T>& operator = ( AutoGDIBase<T>& other )
    {
        return ReSet( other );
    }

    bool operator == ( T gdiObj ) const
    {
        return m_gdiObj == gdiObj;
    }

    bool operator != ( T gdiObj ) const
    {
        return !( operator == ( gdiObj ) );
    }

    bool operator == ( const AutoGDIBase<T>& other ) const
    {
        return operator == ( other.GetGDIObj() );
    }

    bool operator != ( const AutoGDIBase<T>& other ) const
    {
        return !( operator == ( other ) );
    }

    operator T() const
    { 
        return m_gdiObj;
    }

    T GetGDIObj() const
    { 
        return m_gdiObj;
    }

    BOOL IsValid() const
    { 
        return m_gdiObj != NULL;
    }

protected:

    T   m_gdiObj;      //  GDI对象句柄
};


typedef AutoGDIBase<HBITMAP>    AutoBitmapBase;
typedef AutoGDIBase<HBRUSH>     AutoBrushBase;
typedef AutoGDIBase<HPALETTE>   AutoPaletteBase;
typedef AutoGDIBase<HPEN>       AutoPenBase;
typedef AutoGDIBase<HRGN>       AutoRgnBase;
typedef AutoGDIBase<HFONT>      AutoFontBase;


//////////////////////////////////////////////////////////////////////////
//  注意： 各子对象中返回的句柄一般不需要手动去释放,内部会自动释放的

class AutoBitmap : public AutoBitmapBase
{
public:

    AutoBitmap( HBITMAP obj = nullptr ) : AutoBitmapBase( obj )
    {

    }

    AutoBitmap& operator = ( HBITMAP obj )
    {
        return (AutoBitmap&)ReSet( obj );
    }

    HBITMAP CreateBitmap( int nWidth, int nHeight, UINT cPlanes, UINT cBitsPerPel, CONST VOID* lpvBits )
    {
        HBITMAP obj = ::CreateBitmap( nWidth, nHeight, cPlanes, cBitsPerPel, lpvBits );
        ReSet( obj );
        return obj;
    }

    HBITMAP CreateBitmapIndirect( CONST BITMAP *lpbm )
    {
        HBITMAP obj = ::CreateBitmapIndirect( lpbm );
        ReSet( obj );
        return obj;
    }

    HBITMAP CreateCompatibleBitmap( HDC hdc, int nWidth, int nHeight )
    {
        HBITMAP obj = ::CreateCompatibleBitmap( hdc, nWidth, nHeight );
        ReSet( obj );
        return obj;
    }

    HBITMAP CreateDIBSection( HDC hdc, const BITMAPINFO* pbmi, UINT iUsage, void** ppvBits, HANDLE hSection, DWORD dwOffset )
    {
        HBITMAP obj = ::CreateDIBSection( hdc, pbmi, iUsage, ppvBits, hSection, dwOffset );
        ReSet( obj );
        return obj;
    }

    HBITMAP LoadBitmap( HINSTANCE hInstance, LPCTSTR lpBitmapName )
    {
        HBITMAP obj = ::LoadBitmap( hInstance, lpBitmapName );
        ReSet( obj );
        return obj;
    }

    int GetBitmap( BITMAP* pBitMap )
    {
        if (IsValid() == FALSE)
        {
            return 0;
        }
        return ::GetObject( m_gdiObj, sizeof( BITMAP ), pBitMap );
    }

    int GetDIBSection( DIBSECTION* pDIBSection )
    {
        if ( IsValid() == FALSE )
        {
            return 0;
        }
        return ::GetObject( m_gdiObj, sizeof( DIBSECTION ), pDIBSection );
    }
};


class AutoBrush : public AutoBrushBase
{
public:

    AutoBrush( HBRUSH obj = nullptr ) : AutoBrushBase( obj )
    {

    }

    AutoBrush& operator = ( HBRUSH obj )
    {
        return (AutoBrush&)ReSet( obj );
    }

    HBRUSH CreateDIBPatternBrushPt( const void* lpPackedDIB, UINT iUsage )
    {
        HBRUSH obj = ::CreateDIBPatternBrushPt( lpPackedDIB, iUsage );
        ReSet( obj );
        return obj;
    }

    HBRUSH CreatePatternBrush( HBITMAP hbmp )
    {
        HBRUSH obj = ::CreatePatternBrush( hbmp );
        ReSet( obj );
        return obj;
    }

    HBRUSH CreateSolidBrush( COLORREF crColor )
    {
        HBRUSH obj = ::CreateSolidBrush( crColor );
        ReSet( obj );
        return obj;
    }

    int GetLogBrush( LOGBRUSH* pLogBrush )
    {
        if ( IsValid() == FALSE )
        {
            return 0;
        }
        return ::GetObject( m_gdiObj, sizeof( LOGBRUSH ), pLogBrush );
    }
};


class AutoPalette : public AutoPaletteBase
{
public:

    AutoPalette( HPALETTE obj = nullptr ) : AutoPaletteBase( obj )
    {

    }

    AutoPalette& operator = ( HPALETTE obj )
    {
        return (AutoPalette&)ReSet( obj );
    }

    HPALETTE CreatePalette( CONST LOGPALETTE* lplgpl )
    {
        HPALETTE obj = ::CreatePalette( lplgpl );
        ReSet( obj );
        return obj;
    }

    int GetEntryCount()
    {
        if ( IsValid() == FALSE )
        {
            return 0;
        }

        WORD nEntries;
        ::GetObject( m_gdiObj, sizeof( WORD ), &nEntries );
        return (int)nEntries;
    }
};


class AutoPen : public AutoPenBase
{
public:

    AutoPen( HPEN obj = nullptr ) : AutoPenBase( obj )
    {

    }

    AutoPen& operator = ( HPEN obj )
    {
        return (AutoPen&)ReSet( obj );
    }

    HPEN CreatePen( int fnPenStyle, int nWidth, COLORREF crColor )
    {
        HPEN obj = ::CreatePen( fnPenStyle, nWidth, crColor );
        ReSet( obj );
        return obj;
    }

    HPEN CreatePenIndirect( const LOGPEN* lplgpn )
    {
        HPEN obj = ::CreatePenIndirect( lplgpn );
        ReSet( obj );
        return obj;
    }

    int GetLogPen( LOGPEN* pLogPen )
    {
        if ( IsValid() == FALSE )
        {
            return 0;
        }
        return ::GetObject( m_gdiObj, sizeof( LOGPEN ), pLogPen );
    }
};


class AutoRgn : public AutoRgnBase
{
public:

    AutoRgn( HRGN obj = nullptr ) : AutoRgnBase( obj ) 
    {

    }

    AutoRgn& operator = ( HRGN obj )
    {
        return (AutoRgn&)ReSet( obj );
    }

    HRGN CreateRectRgn( int nLeftRect, int nTopRect, int nRightRect, int nBottomRect )
    {
        HRGN obj = ::CreateRectRgn( nLeftRect, nTopRect, nRightRect, nBottomRect );
        ReSet( obj );
        return obj;
    }

    HRGN CreateRectRgnIndirect( const RECT* lprc )
    {
        HRGN obj = ::CreateRectRgnIndirect( lprc );
        ReSet( obj );
        return obj;
    }
};


class AutoFont : public AutoFontBase
{
public:

    AutoFont( HFONT obj = nullptr ) : AutoFontBase( obj ) 
    {

    }

    AutoFont& operator = ( HFONT obj )
    {
        return (AutoFont&)ReSet( obj );
    }

    HFONT CreateFont(
        int nHeight,				// height of font
        int nWidth,					// average character width
        int nEscapement,			// angle of escapement
        int nOrientation,			// base-line orientation angle
        int fnWeight,				// font weight
        DWORD bItalic,				// italic attribute option
        DWORD bUnderline,			// underline attribute option
        DWORD cStrikeOut,			// strikeout attribute option
        DWORD nCharSet,				// character set identifier
        DWORD nOutPrecision,		// output precision
        DWORD nClipPrecision,		// clipping precision
        DWORD nQuality,				// output quality
        DWORD nPitchAndFamily,		// pitch and family
        LPCTSTR lpszFace           // typeface name
        )
    {
        HFONT obj = ::CreateFont(
            nHeight,
            nWidth,
            nEscapement,
            nOrientation,
            fnWeight,
            bItalic,
            bUnderline,
            cStrikeOut,
            nCharSet,
            nOutPrecision,
            nClipPrecision,
            nQuality,
            nPitchAndFamily,
            lpszFace
            );
        ReSet( obj );

        return obj;
    }

    HFONT CreateFontIndirect( const LOGFONT* lplf )
    {
        HFONT obj = ::CreateFontIndirect( lplf );
        ReSet( obj );
        return obj;
    }

    int GetLogFont( LOGFONT* pLogFont )
    {
        if ( IsValid() == FALSE )
        {
            return 0;
        }
        return ::GetObject( m_gdiObj, sizeof( LOGFONT ), pLogFont );
    }
};


#endif  //  _WINDOWS_HELPER_H_