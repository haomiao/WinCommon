#include "stdafx.h"

#include <time.h>
#include "string_helper.h"
#include "debug_assist.h"


CDebugAssist::CDebugAssist():
m_pDebugSink(nullptr),
m_debugLevel(DL_ALL)
{

}


CDebugAssist::~CDebugAssist()
{

}


CDebugAssist * CDebugAssist::GetInstance()
{
    static CDebugAssist s_debugAssist;
    return &s_debugAssist;
}


void CDebugAssist::SetSink( COutPutBaseSink * pDebugSink )
{
    Flush();
    m_pDebugSink = pDebugSink;
}


void CDebugAssist::SetDebugLevel( DebugLevel debugLevel )
{
    m_debugLevel = debugLevel;
}


DebugLevel CDebugAssist::GetDebugLevel()
{
    return m_debugLevel;
}


void CDebugAssist::Write( const std::string & debugInfoStr, DebugLevel curDebugLevel )
{
    if ( curDebugLevel < m_debugLevel)
    {
        return;
    }

    if (m_pDebugSink != nullptr)
    {
        std::string tempStr = m_pDebugSink->GetOutPutHeaderInfo();
        tempStr += debugInfoStr;
        m_pDebugSink->Write( tempStr );
    }
}


void CDebugAssist::Write( const char debugInfoC, DebugLevel curDebugLevel )
{
    if ( curDebugLevel < m_debugLevel )
    {
        return;
    }

    if ( m_pDebugSink != nullptr )
    {
        std::string tempStr = m_pDebugSink->GetOutPutHeaderInfo();
        tempStr += debugInfoC;
        m_pDebugSink->Write( tempStr );
    }
}


void CDebugAssist::Flush()
{
    if (m_pDebugSink != nullptr)
    {
        m_pDebugSink->Flush();
    }
}