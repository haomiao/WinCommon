///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   debug_assist.h
/// 	@brief	��������. 
/// 
///		��Ҫʵ�ֻ��ڿ�Ŀ�ꡢ�ļ�Ŀ�ꡢ����̨Ŀ�ꡢ�����Ŀ�꣬ʵ�ֵ�����Ϣ��¼�������  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/06  16:13:06
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _DEBUG_ASSIST_H_
#define     _DEBUG_ASSIST_H_

#include<string>
#include "config.h"
#include "output_sink.h"

//  ���Լ���
enum DebugLevel
{
    DL_FORBID = 0,                  //  ��ֹ����
    DL_ERROR_EXCEPTION,             //  ������Ϣ���쳣
    DL_OPERATOR,                    //  �������̡�ϸ������
    DL_USERINFO,                    //  �û���Ϣ
    DL_ALL,                         //  ������Ϣ���ڲ�״̬
};


//  �������֣��ṩ�ⲿʹ�õļ�ʹ�ò���
//  һ�������Ϊ����ģʽ
//  ��������Ҫ����ʹ�ù���ģʽ��������Ŀ���������
class CDebugAssist
{
public:

    ~CDebugAssist();

public:

    static CDebugAssist * GetInstance();

    /**
    *  @brief  ����Ŀ���������
    *  @param  [in] 	pDebugSink         ������Ϣ���Ŀ�����
    */
    void SetSink( COutPutBaseSink * pDebugSink );

    /**
    *  @brief  ����/��ȡ�����������
    *  @param  [in] 	debugLevel         �����������
    */
    void SetDebugLevel( DebugLevel debugLevel );
    DebugLevel GetDebugLevel();

    /**
    *  @brief  ������Ϣ�������(ÿ����Write�򽫵�����Ϣ�������)
    *  @param  [in] 	debugInfoStr          ������Ϣ��
    *  @param  [in] 	debugInfoC            ����������Ϣ�ַ�
    *  @param  [in] 	curDebugLevel         ��ǰ���Լ���(��С��m_debugLevel�������������Ϣ)
    */
    void Write( const std::string & debugInfoStr, DebugLevel curDebugLevel =  DL_ALL);
    void Write( const char debugInfoC, DebugLevel curDebugLevel = DL_ALL );

    /**
    *  @brief  ˢ�»�������������Ϣ���
    */
    void Flush();

private:

    CDebugAssist();

    DISALLOW_COPY_AND_ASSIGN( CDebugAssist );

private:

    COutPutBaseSink *   m_pDebugSink;   //  �������Ŀ�����
    DebugLevel          m_debugLevel;   //  ���Լ���
};

#endif  //  _DEBUG_ASSIST_H_