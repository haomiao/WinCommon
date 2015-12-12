///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   auto_lock.h 
/// 	@brief	�Զ��������װ. 
/// 
///		��Ҫʵ�ֶ��������Զ���������������  
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
 * \brief �Զ���
 */
//  ʹ��ʾ��:CCriticalSectionLock crisecLoc;
//          AutoLock(&crisecLoc);���Զ��ӽ���
class AutoLock
{
public:

    AutoLock(ILockBase *lock);

    ~AutoLock();

private:

    ILockBase *m_lock;
};


//    �ṩģ���Զ���
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