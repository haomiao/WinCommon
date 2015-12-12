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
		m_llCountPreSecond = lager_int.QuadPart;    //  ��ȡʱ��Ƶ��
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
		dwSpan /= m_llCountPreSecond;   //  ִ�е�����	
		dwSpan *= 1000.0f;              //  ת��Ϊִ�еĺ�����
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
        //  ���������֧�ָ߾��ȼ�ʱ�����Ͳ��ö�ý���ʱ��
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


//  ͨ����Ƕ���αָ��֧��
//  ��ȡCPU���������к����������е�ʱ��
LONGLONG CTimeStamp::GetCycleTime()
{
    __asm __emit 0x0F
    __asm __emit 0X31
}


//  ����CPU��Ƶ
//  TODO:Ŀǰ��δ�ҵ��ȽϾ�ȷ�ķ�ʽ��ȡCPU��Ƶ
double CTimeStamp::GetCPUFreq()
{
    m_llCpuFreq = 0.0;
    return m_llCpuFreq;
}