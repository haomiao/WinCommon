#include "stdafx.h"
#include "auto_lock.h"

AutoLock::AutoLock(ILockBase *lock)
:m_lock(lock)
{
    m_lock?m_lock->Lock():NULL;
}


AutoLock::~AutoLock()
{
    m_lock?m_lock->UnLock():NULL;
}