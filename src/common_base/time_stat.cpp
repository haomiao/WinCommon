#include "stdafx.h"
#include "time_stat.h"
#include "mmsystem.h"

#pragma comment(lib,"winmm.lib")


CTimeStat::CTimeStat()
{
	LARGE_INTEGER lager_int;
	m_llTimeStart = m_llTimeEnd = 0;
	m_fTimeEscape = 0.0f;
	if ( QueryPerformanceFrequency(&lager_int) )
	{
		m_llCountPreSecond = lager_int.QuadPart;    //  获取时钟频率
	}
	else
	{
		m_llCountPreSecond = 0;
	}
}


CTimeStat::~CTimeStat()
{

}


void CTimeStat::StartStat()
{
	m_llTimeStart = _CurTime();
}


void CTimeStat::EndStat()
{
	m_llTimeEnd = _CurTime();
}


double CTimeStat::GetRunTime()
{
	double dwSpan;
	if( m_llCountPreSecond != 0 )
	{
		dwSpan = (double)( m_llTimeEnd - m_llTimeStart );
		dwSpan /= m_llCountPreSecond;   //  执行的秒数	
		dwSpan *= 1000.0f;              //  转化为执行的毫秒数
	}
	else
	{
		dwSpan = (double)( m_llTimeEnd - m_llTimeStart );
	}
	return dwSpan;
}


LONGLONG CTimeStat::_CurTime()
{
	LONGLONG llCount;
	LARGE_INTEGER ll_Int;
	if( (0 != m_llCountPreSecond) && QueryPerformanceCounter(&ll_Int) )
	{
		llCount = ll_Int.QuadPart;
	}
	else
	{
        //  如果机器不支持高精度计时器，就采用多媒体计时器
		llCount = (LONGLONG)timeGetTime();
	}
	return llCount;
}


CTimeStamp::CTimeStamp() :m_llTimeStart( 0 ), m_llTimeEnd( 0 ), m_fTimeEscape( 0.0 )
{
//     LARGE_INTEGER lager_int;
//     m_llTimeStart = m_llTimeEnd = 0;
//     m_fTimeEscape = 0.0f;
//     QueryPerformanceFrequency( &lager_int );
//     m_llCountPreSecond = lager_int.QuadPart;

    m_llCpuFreq = GetCPUFreq();
}


CTimeStamp::~CTimeStamp()
{

}


void CTimeStamp::StartStat()
{
    m_llTimeStart = GetCycleTime();
}


void CTimeStamp::EndStat()
{
    m_llTimeEnd = GetCycleTime();
}


double CTimeStamp::GetRunTime()
{
    return ( m_llTimeEnd - m_llTimeStart ) * 1.0 / m_llCpuFreq;
}


//  通过内嵌汇编伪指令支持
//  获取CPU自启动运行后到现在所运行的时间
LONGLONG CTimeStamp::GetCycleTime()
{
    __asm __emit 0x0F
    __asm __emit 0X31
}


//  计算CPU主频
//  TODO:目前暂未找到比较精确的方式获取CPU主频
double CTimeStamp::GetCPUFreq()
{
    m_llCpuFreq = 0.0;
    return m_llCpuFreq;
}