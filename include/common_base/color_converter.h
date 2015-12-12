///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   ColorConverter.h
/// 	@brief	ɫ��ת��. 
/// 
///		��Ҫʵ��RGB��HSL��ɫ�ʿռ���໥ת��  
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
        *  @brief  ARGB��HSLɫ�ʿռ���໥ת�� 
        *  @param  [in] 	ARGB        ARGBֵ(����RGB) 
        *  @param  [out] 	H           ɫ��  
        *  @param  [out] 	S           ���Ͷ� 
        *  @param  [out] 	L           ���� 
        *  @return 				     
        */
    static void RGBtoHSL( DWORD ARGB, float &H, float &S, float &L );
    static DWORD HSLtoRGB( DWORD& ARGB, float H, float S, float L );

};

#endif  //  _COLOR_CONVERTER_H_