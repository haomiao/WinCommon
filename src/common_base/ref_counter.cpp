#include "stdafx.h"
#include "ref_counter.h"


CRefCounterBase::CRefCounterBase() :
m_ref(0)
{

}


CRefCounterBase::~CRefCounterBase()
{

}


void CRefCounterBase::AddRef( ATOMIC_T value )
{
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
    return ::InterlockedExchangeAdd( static_cast<volatile LONG *>( &m_ref ), value );
#else
    m_ref += value;
#endif
}


void CRefCounterBase::DecRef( ATOMIC_T value )
{
#if  defined(_MSC_VER) && (_MSC_VER <= 1500)
    return ::InterlockedExchangeAdd( static_cast<volatile LONG *>( &m_ref ), -value );
#else
    if (value > m_ref)
    {
        m_ref = 0;
        return;
    }
    m_ref -= value;
#endif
}


ATOMIC_T CRefCounterBase::GetRefCnt()
{
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
    return InterlockedExchangeAdd( const_cast<volatile LONG*>( &m_ref ), 0 );
#else
    return m_ref;
#endif
}


CRefCounterShared::CRefCounterShared( bool bAutoDelete ) :
m_bAutoDelete( bAutoDelete )
{

}


CRefCounterShared::~CRefCounterShared()
{

}


void CRefCounterShared::AddRef( ATOMIC_T value )
{
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
#else
    AutoLock lock( &m_lock );
#endif

    CRefCounterBase::AddRef( value );
}


void CRefCounterShared::DecRef( ATOMIC_T value )
{
    ATOMIC_T tempRef = 0;
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
    {
#else
    {
        AutoLock lock( &m_lock );
#endif

        CRefCounterBase::DecRef( value );
        tempRef = CRefCounterBase::GetRefCnt();
    }

    if ( tempRef == 0 && m_bAutoDelete == true )
    {
        delete this;
    }
}


ATOMIC_T CRefCounterShared::GetRefCnt()
{
#if defined(_MSC_VER) && (_MSC_VER <= 1500)
#else
    AutoLock lock( &m_lock );
#endif

    return CRefCounterBase::GetRefCnt();
}


bool CRefCounterShared::GetAutoDeleteFlag()
{
    return m_bAutoDelete;
}
