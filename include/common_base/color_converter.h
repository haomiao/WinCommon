///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   ColorConverter.h
/// 	@brief	色彩转换. 
/// 
///		主要实现RGB与HSL等色彩空间的相互转换  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/09  11:03:04
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _COLOR_CONVERTER_H_
#define     _COLOR_CONVERTER_H_

class CCorlorConverter 
{
public:

    /**
        *  @brief  ARGB与HSL色彩空间的相互转换 
        *  @param  [in] 	ARGB        ARGB值(兼容RGB) 
        *  @param  [out] 	H           色调  
        *  @param  [out] 	S           饱和度 
        *  @param  [out] 	L           亮度 
        *  @return 				     
        */
    static void RGBtoHSL( DWORD ARGB, float &H, float &S, float &L );
    static DWORD HSLtoRGB( DWORD& ARGB, float H, float S, float L );

};

#endif  //  _COLOR_CONVERTER_H_