///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   auto_lock.h 
/// 	@brief	自动锁基类封装. 
/// 
///		主要实现多种锁的自动加锁、解锁机制  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/04  12:03:04
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _AUTO_LOCK_H_
#define     _AUTO_LOCK_H_

#include "lock_base.h"

/**
 * \brief 自动锁
 */
//  使用示例:CCriticalSectionLock crisecLoc;
//          AutoLock(&crisecLoc);会自动加解锁
class AutoLock
{
public:

    AutoLock(ILockBase *lock);

    ~AutoLock();

private:

    ILockBase *m_lock;
};


//    提供模板自动锁
template<class T>
class AutoLockT
{
public:

    AutoLockT(T &obj) :m_obj(obj)
    {
        m_obj.Lock();
    }

    ~AutoLockT()
    {
        m_obj.UnLock();
    }

private:

    T   &m_obj;
};

#endif  //  _AUTO_LOCK_H_