///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   windows_helper.h
/// 	@brief	windows���ṩ��һЩ��������. 
/// 
///		��Ҫ�ṩwindows�³��õĸ������ߡ����������Դ�����  
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
*  @brief   �ж�ĳ����ID����(���̶��㴰��)���������ڱ����Ӧ�����Ƿ����
*  @param  [in] 	processID             ����ID
*  @param  [in] 	lpWindowName          ���ڱ�����
*  @param  [in] 	lpClassName           ��������
*  @param  [in]     processName           ������
*  @detail:�����õڶ��֣���һ�����ͨ���������Ǵ��ڱ�����֮һ����,��������Чʱ�����߾���Ӧ�����
*          �����ҵ��ĵ�һ�����̼���
*/
BOOL IsProcessExist( DWORD processID ) ;
BOOL IsProcessExist( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL);
BOOL IsProcessExist2( LPCTSTR processName );


/**
*  @brief   ��ȡ���̶��㴰�ھ��
*  @param  [in] 	processID             ����ID
*  @param  [in] 	lpWindowName          ���ڱ�����
*  @param  [in] 	lpClassName           ��������
*  @param  [in]     processName           ������
*  @detail:�����ҵ���һ�����̼���
*/
HWND GetProcessTopWindowWnd( DWORD processID );
HWND GetProcessTopWindowWnd( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL );
HWND GetProcessTopWindowWnd2( LPCTSTR processName );


/**
*  @brief   ��ȡ���̾��(��ʹ��ʱ����ʹ��CloseHandle�ͷ�)
*  @param  [in] 	processID             ����ID
*  @param  [in] 	lpWindowName          ���ڱ�����
*  @param  [in] 	lpClassName           ��������
*  @param  [in]     processName           ������
*  @detail:�����ҵ���һ�����̼���
*/
HANDLE GetProcessHandle( DWORD processID );
HANDLE GetProcessHandle( LPCTSTR lpWindowName, LPTSTR lpClassName = NULL );
HANDLE GetProcessHandle2( LPCTSTR processName );


/**
*  @brief   ��ȡ��������������Ϣ
*  @param  [in] 	dwCode           ��GetLastError��ȡ�Ĵ�����
*  @return 				             ���ش���������Ϣ
*/
TString GetSystemErrorDesc( DWORD dwErrorCode );


/**
*  @brief   ��ȡ��ǰϵͳ��Ϣ
*  @param  [out] 	lpSystemInfo     ϵͳ��Ϣ
*/
void GetCurrentSystemInfo( LPSYSTEM_INFO lpSystemInfo);


/**
*  @brief   ��ȡ��ǰϵͳ�������ܹ�
*  detail:  PROCESSOR_ARCHITECTURE_AMD64��PROCESSOR_ARCHITECTURE_IA64��PROCESSOR_ARCHITECTURE_INTEL
            PROCESSOR_ARCHITECTURE_UNKNOWN
*/
DWORD GetSystemProcessorArchitecture();


/**
*  @brief   ��ȡ��ǰϵͳ��ҳ��С(һ�㳣��VirtualAlloc��ʹ��)
*/
DWORD GetSystemPageSize();


/**
*  @brief   ��ȡ��ǰϵͳ����������
*/
DWORD GetSystemProcessorNumber();


/**
*  @brief   ��ȡ����ϵͳ��Ϣ
*  @param  [out] 	lpOSInfo        ����ϵͳ��Ϣ
*  @param  [out] 	versionMS       ����ϵͳ���汾����Ϣ
*  @param  [out] 	versionLS       ����ϵͳ�ΰ汾����Ϣ
*  @param  [out] 	nVer            ����ϵͳ�汾����Ϣ
*  @detail��Ϊ�����Ƿ�Ϊ��ȡInfoEx������֮ǰӦ����lpOSInfo��dwOSVersionInfoSize����Ϊ��Ӧ�ṹ��С
            �磺 OSVERSIONINFOEX osInfoEx;
                 osInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
                 GetOSInfo( (LPOSVERSIONINFO)&osInfoEx );

                 OSVERSIONINFO osInfo;
                 osInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
                 GetOSInfo( &osInfo );
            ����΢���Ƽ�ʹ�øýӿ�(�������⣬�������ϵͳ�޷���������)���ɻ���������ʵ������磺
            ͨ��Kernel32.dll��ȡ��Ʒ���汾��(������ϵͳ�汾һ��)
*/
BOOL GetOSInfo( LPOSVERSIONINFO  lpOSInfo);

//  ��ȡ����ϵͳ��Ϣ, �������汾�Ŵΰ汾�š��汾��ʽ����Ϣ��:6.1.13.1345
BOOL GetOSInfo( DWORD &versionMS, DWORD &versionLS );
BOOL GetOSInfo( DWORD nVer[4] );

//  ����Ƚϳ��õĻ���GetSystemMetrics��SystemParametersInfo������System Information Functions


//////////////////////////////////////////////////////////////////////////
//  ������Ҫ��װ���õ�windows�µľ������Դ��װ

//  ģ����ʽ��Handle�Զ���װ
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

    //  �������
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

    //  ������(�ⲿӦ����ʵ������ͷŷ��صľ����Դ)
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

    HANDLE	    m_handle;   //  ���
};


typedef AutoHandle<INVALID_HANDLE_VALUE> AutoFileHandle;     // �ļ��Զ����
typedef AutoHandle<NULL>                 AutoResourceHandle; // ��Դ�Զ����(���ڡ����̡��̵߳���Դ)


//  HDC�Զ���װ
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
    //  ��ȡ��ǰ��ѡ��HDC�е���Ӧ���͵Ķ�����

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
    //  ���ý�ѡ��HDC�е���Ӧ���͵Ķ���������������ǰ���õĶ�����

    inline int SelectClipRgn( HRGN hrgn )
    {
        return ::SelectClipRgn( m_hDC, hrgn );
    }

    //  mode:ִ�м�������Ĳ���ģʽ
    inline int ExtSelectClipRgn( HRGN hrgn, int mode )
    {
        return ::ExtSelectClipRgn( m_hDC, hrgn, mode );
    }

    //  bForceBackground:����ǰ��ɫ���Ǳ���ɫ��ʶ
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

    HWND        m_hWnd;             //  ���ھ��
    HDC         m_hDC;              //  ���ھ����Ӧ�豸�����ľ��
    BOOL        m_isNeedRelease;    //  �Ƿ���Ҫ�ͷŸ��豸�����ľ��
};


//  �Զ�����DC��װ
//  һ������WIN32�������ӦWM_PAINT��Ϣ�������п�ʼ���ƺͽ������Ƶķ�װ
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

    PAINTSTRUCT	    m_ps;       //  ���ƽ�ṹ
    HWND		    m_hwnd;     //  ���ھ��
    HDC			    m_hdc;      //  �����豸�豸������
};


//  ģ����࣬�Զ�GDI�����װ
//  һ�㲻����ʹ��
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

    T   m_gdiObj;      //  GDI������
};


typedef AutoGDIBase<HBITMAP>    AutoBitmapBase;
typedef AutoGDIBase<HBRUSH>     AutoBrushBase;
typedef AutoGDIBase<HPALETTE>   AutoPaletteBase;
typedef AutoGDIBase<HPEN>       AutoPenBase;
typedef AutoGDIBase<HRGN>       AutoRgnBase;
typedef AutoGDIBase<HFONT>      AutoFontBase;


//////////////////////////////////////////////////////////////////////////
//  ע�⣺ ���Ӷ����з��صľ��һ�㲻��Ҫ�ֶ�ȥ�ͷ�,�ڲ����Զ��ͷŵ�

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