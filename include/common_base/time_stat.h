///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   time_stat.h
/// 	@brief	��ʱ������. 
/// 
///		��Ҫʵ�ָ��ֳ��õļ�ʱ�����ࡢ������  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/04  13:56:19
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _TIME_STAT_H_
#define     _TIME_STAT_H_


#include <Windows.h>

//  �߾��ȼ�ʱ�����ڲ����ø߾������ܼ�ʱ�������ֵ
//  ����֧��ʱ�������ö�ý��߾��ȼ�ʱ��
class CTimeStat  
{
public:

	CTimeStat();

	virtual ~CTimeStat();
 
    //  ��ʼ��ʱ
	void StartStat();

    //  ������ʱ
	void EndStat();

    //  ��ȡ����ʱ��
	double GetRunTime();

protected:

    //  ��ȡ��ǰʱ��
	LONGLONG _CurTime();

private:

	LONGLONG        m_llTimeStart;
	LONGLONG        m_llTimeEnd;
	double          m_fTimeEscape;
	LONGLONG        m_llCountPreSecond;
};


//  Ŀǰ��֧��CPU Pentium���ϵ�
//  ����RDTSC����ָ���ȡʱ��������ȼ���
//  �ڶ���п��ܻ��г�������������¾��Ȳ��߻��ȶ������Ŀ���
//  TODO:�ɸ�����Ҫѡ��ʹ��,Ŀǰδ����
class CTimeStamp
{
public:

    CTimeStamp();

    virtual ~CTimeStamp();

    //  ��ʼ��ʱ
    void StartStat();

    //  ������ʱ
    void EndStat();

    //  ��ȡ����ʱ��(��ת��ΪmsΪ��λ��ʱ��)
    double GetRunTime();

protected:

    //  ��ȡ��ǰʱ��
    LONGLONG GetCycleTime();

    //  ��ȡ��ǰCPU��Ƶ
    double GetCPUFreq();

private:

    LONGLONG        m_llTimeStart;
    LONGLONG        m_llTimeEnd;
    double          m_fTimeEscape;
    double          m_llCpuFreq;
};

//  �����ʱ������������ʽ����ʵ����time��GetTickCount��clock��
//  ��һ�㾫�ȶ��Ƚϵ�
#endif  //  _TIME_STAT_H_