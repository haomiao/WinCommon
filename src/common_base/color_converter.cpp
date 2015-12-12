#include "stdafx.h"
#include "color_converter.h"

namespace
{
    static const float OneThird = 1.0f / 3;
}

void CCorlorConverter::RGBtoHSL( DWORD ARGB, float &H, float &S, float &L )
{
    const float R = (float)GetRValue( ARGB );
    const float G = (float)GetGValue( ARGB );
    const float B = (float)GetBValue( ARGB );
    const float nR = ( R < 0 ? 0 : ( R>255 ? 255 : R ) ) / 255;
    const float nG = ( G < 0 ? 0 : ( G>255 ? 255 : G ) ) / 255;
    const float nB = ( B < 0 ? 0 : ( B>255 ? 255 : B ) ) / 255;
    const float m = min( min( nR, nG ), nB );
    const float M = max( max( nR, nG ), nB );

    L = ( m + M ) / 2;
    if ( M == m )
    {
        H = S = 0;
    }
    else
    {
        const float f = ( nR == m ) ? ( nG - nB ) : ( ( nG == m ) ? ( nB - nR ) : ( nR - nG ) );
        const float i = ( nR == m ) ? 3.0f : ( ( nG == m ) ? 5.0f : 1.0f );

        H = ( i - f / ( M - m ) );
        if ( H >= 6 )
        {
            H -= 6;
        }

        H *= 60;
        S = ( 2 * ( L ) <= 1 ) ? ( ( M - m ) / ( M + m ) ) : ( ( M - m ) / ( 2 - M - m ) );
    }
}


DWORD CCorlorConverter::HSLtoRGB( DWORD &ARGB, float H, float S, float L )
{
    const float q = 2 * L < 1 ? L*( 1 + S ) : ( L + S - L*S );
    const float p = 2 * L - q;
    const float h = H / 360;
    const float tr = h + OneThird;
    const float tg = h;
    const float tb = h - OneThird;
    const float ntr = tr < 0 ? tr + 1 : ( tr>1 ? tr - 1 : tr );
    const float ntg = tg < 0 ? tg + 1 : ( tg>1 ? tg - 1 : tg );
    const float ntb = tb < 0 ? tb + 1 : ( tb>1 ? tb - 1 : tb );
    const float R = 255 * ( 6 * ntr < 1 ? p + ( q - p ) * 6 * ntr : ( 2 * ntr < 1 ? q : ( 3 * ntr < 2 ? p + ( q - p ) * 6 * ( 2.0f*OneThird - ntr ) : p ) ) );
    const float G = 255 * ( 6 * ntg < 1 ? p + ( q - p ) * 6 * ntg : ( 2 * ntg < 1 ? q : ( 3 * ntg < 2 ? p + ( q - p ) * 6 * ( 2.0f*OneThird - ntg ) : p ) ) );
    const float B = 255 * ( 6 * ntb < 1 ? p + ( q - p ) * 6 * ntb : ( 2 * ntb < 1 ? q : ( 3 * ntb < 2 ? p + ( q - p ) * 6 * ( 2.0f*OneThird - ntb ) : p ) ) );

    ARGB &= 0xFF000000;
    ARGB |= RGB( (BYTE)( R < 0 ? 0 : ( R>255 ? 255 : R ) ), (BYTE)( G < 0 ? 0 : ( G>255 ? 255 : G ) ), (BYTE)( B < 0 ? 0 : ( B>255 ? 255 : B ) ) );

    return ARGB;
}