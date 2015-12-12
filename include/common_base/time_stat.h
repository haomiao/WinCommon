///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   time_stat.h
/// 	@brief	计时工具类. 
/// 
///		主要实现各种常用的计时工具类、函数等  
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

//  高精度计时器，内部采用高精度性能计时器计算的值
//  若不支持时，将采用多媒体高精度计时器
class CTimeStat  
{
public:

	CTimeStat();

	virtual ~CTimeStat();
 
    //  开始计时
	void StartStat();

    //  结束计时
	void EndStat();

    //  获取运行时间
	double GetRunTime();

protected:

    //  获取当前时间
	LONGLONG _CurTime();

private:

	LONGLONG        m_llTimeStart;
	LONGLONG        m_llTimeEnd;
	double          m_fTimeEscape;
	LONGLONG        m_llCountPreSecond;
};


//  目前仅支持CPU Pentium以上的
//  采用RDTSC机器指令获取时间戳，精度极高
//  在多核中可能会有出现乱序的想象导致精度不高或不稳定抖动的可能
//  TODO:可根据需要选择使用,目前未完善
class CTimeStamp
{
public:

    CTimeStamp();

    virtual ~CTimeStamp();

    //  开始计时
    void StartStat();

    //  结束计时
    void EndStat();

    //  获取运行时间(已转化为ms为单位的时间)
    double GetRunTime();

protected:

    //  获取当前时间
    LONGLONG GetCycleTime();

    //  获取当前CPU主频
    double GetCPUFreq();

private:

    LONGLONG        m_llTimeStart;
    LONGLONG        m_llTimeEnd;
    double          m_fTimeEscape;
    double          m_llCpuFreq;
};

//  此外计时器还有其他方式可以实现如time、GetTickCount、clock等
//  但一般精度都比较低
#endif  //  _TIME_STAT_H_