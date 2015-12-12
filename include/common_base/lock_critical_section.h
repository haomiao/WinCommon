///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_critical_section.h 
/// 	@brief	临界区锁. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/11  11:13:52
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _LOCK_CRITICAL_SECTION_H_
#define     _LOCK_CRITICAL_SECTION_H_

#include "lock_base.h"

/**
 * \brief 临界区锁
 */
class CCriticalSectionLock : public ILockBase
{
public:

    CCriticalSectionLock();

    CCriticalSectionLock( DWORD spinCount );

    virtual ~CCriticalSectionLock();

    virtual void Lock();

    virtual void UnLock();

    BOOL TryLock();
   
    DWORD SetSpinCount( DWORD dwSpinCount );

private:

    CCriticalSectionLock( const CCriticalSectionLock& );
    CCriticalSectionLock & operator = ( const CCriticalSectionLock & );

private:

    CRITICAL_SECTION m_criticalSection;
};

#endif  //  _LOCK_CRITICAL_SECTION_H_