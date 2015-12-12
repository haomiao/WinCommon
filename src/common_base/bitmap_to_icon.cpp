#include "stdafx.h"
#include "bitmap_to_icon.h"

HICON BitmapToIcon(HBITMAP hBitmap)
{
    CBitmap bitmap;
    bitmap.Attach(hBitmap);

    BITMAP bmp;
    bitmap.GetBitmap(&bmp);

    HBITMAP hbmMask = ::CreateCompatibleBitmap(::GetDC(NULL), bmp.bmWidth, bmp.bmHeight);

    ICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = bitmap;
    ii.hbmMask  = hbmMask;

    HICON hIcon = ::CreateIconIndirect(&ii);
    ::DeleteObject(hbmMask);

    bitmap.Detach();
    
    return hIcon;
}