#include "stdafx.h"
#include <algorithm>
#include "delegate.h"
#include "auto_lock.h"


CDelegateBase::CDelegateBase(void *pObject, void *pFunCallback):
m_pObject(pObject), m_pFunCallback(pFunCallback)
{

}


CDelegateBase::CDelegateBase(const CDelegateBase & other)
{
    m_pObject       = other.m_pObject;
    m_pFunCallback  = other.m_pFunCallback;
}


CDelegateBase::~CDelegateBase()
{

}


BOOL CDelegateBase::operator == (const CDelegateBase &other)
{
    return other.m_pObject == m_pObject && other.m_pFunCallback == m_pFunCallback;
}


BOOL CDelegateBase::operator != (const CDelegateBase &other)
{
    return (! (*this ==(other) ) );
}


CDelegateBase & CDelegateBase::operator = (const CDelegateBase & other)
{
    this->m_pObject         = other.m_pObject;
    this->m_pFunCallback    = other.m_pFunCallback;
    return (*this);
}


BOOL CDelegateBase::operator()(void * pParamter)
{
    return Invoke(pParamter);
}


CDelegateNoneObject::CDelegateNoneObject(PFunCallback pFunCallback):
CDelegateBase(NULL, pFunCallback)
{

}


CDelegateNoneObject::~CDelegateNoneObject()
{

}


CDelegateBase * CDelegateNoneObject::Copy() const
{
    return new CDelegateNoneObject( (PFunCallback)GetFunCallback() );
}


BOOL CDelegateNoneObject::Invoke(void * pParamter)
{
    PFunCallback pFunCallback = (PFunCallback)GetFunCallback();
    if (pFunCallback != NULL)
    {
        return (*pFunCallback)(pParamter);
    }

    return FALSE;
}


CDelegateEventSource::CDelegateEventSource():m_delegateLock(3000)
{

}


CDelegateEventSource::~CDelegateEventSource()
{
    ClearAll();
}


void CDelegateEventSource::AppendDelegate(const CDelegateBase & delegateObject)
{
    AutoLock autoLoak(&m_delegateLock);
    std::list<CDelegateBase *>::iterator it = m_delegateList.begin();
    std::list<CDelegateBase *>::iterator end = m_delegateList.end();
    for (; it != end; ++it)
    {
        if (*(*it) == delegateObject)
        {
            return;
        }
    }

    m_delegateList.push_back(delegateObject.Copy() );
}


void CDelegateEventSource::DeleteDelegate(const CDelegateBase & delegateObject)
{
    AutoLock autoLoak( &m_delegateLock );
    std::list<CDelegateBase *>::iterator it = m_delegateList.begin();
    std::list<CDelegateBase *>::iterator end = m_delegateList.end();
    for (; it != end; ++it)
    {
        if (*(*it) == delegateObject)
        {
            delete *it;
            m_delegateList.erase(it);
            break;
        }
    }
}


void CDelegateEventSource::operator += (const CDelegateBase &other)
{
    AppendDelegate(other);
}


void CDelegateEventSource::operator -= (const CDelegateBase &other)
{
    DeleteDelegate(other);
}


void CDelegateEventSource::operator += (PFunCallback pFunCallback)
{
    CDelegateBase * delegateObj = (CDelegateBase *)MakeDelegateNoneObject(pFunCallback);
    AppendDelegate(delegateObj);
}



void CDelegateEventSource::operator -= (PFunCallback pFunCallback)
{
//     有点低效,待改进
//     CDelegateBase * delegateObj = (CDelegateBase *)MakeDelegateNoneObject(pFunCallback);
//     DeleteDelegate(delegateObj);
//     delete delegateObj;

    CDelegateNoneObject  delegateObj = MakeDelegateNoneObject2(pFunCallback);
    DeleteDelegate(&delegateObj);
}


void CDelegateEventSource::ClearAll()
{
    AutoLock autoLoak( &m_delegateLock );
    std::list<CDelegateBase *>::iterator it;
    while(!m_delegateList.empty() )
    {
        it = m_delegateList.begin();
        delete *it;
        m_delegateList.erase(it);
    }

    m_delegateList.clear();
}


BOOL CDelegateEventSource::IsEmpty()
{
    AutoLock autoLoak( &m_delegateLock );
    return (m_delegateList.empty() == 0);
}


CDelegateEventSource::operator bool()
{
    return (!IsEmpty() );
}


BOOL CDelegateEventSource::operator()(void *pParamter)
{
    AutoLock autoLoak( &m_delegateLock );
    std::list<CDelegateBase *>::iterator it = m_delegateList.begin();
    std::list<CDelegateBase *>::iterator end = m_delegateList.end();
    for (; it != end; ++it)
    {
        if (*it != NULL)
        {
            (*(*it))(pParamter);
        }
    }

    return TRUE;
}
