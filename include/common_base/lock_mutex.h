///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_mutex.h   
/// 	@brief	互斥锁. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/02  10:23:12
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _LOCK_MUTEX_H_
#define     _LOCK_MUTEX_H_

#include "lock_base.h"

/**
 * \brief 互斥锁
 */
class CMutexLock : public ILockBase
{
public:

    /**
      *  @brief  构造函数  
      *  @param  [in] 	bCreateFlag             创建标识 
      *  @param  [in] 	lpMutexAttributes       互斥锁安全属性
      *  @param  [in] 	bInitialOwner           初始化锁拥有者
      *  @param  [in] 	lpName                  锁命名
      */
    CMutexLock(BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL, BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL);

    virtual ~CMutexLock();

    //  打开已创建的互斥锁
    BOOL OpenMutex( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);

    virtual void Lock();

    virtual void UnLock();

    //  设置锁等待时间
    void SetLockLimited( DWORD dwMilliseconds = INFINITE );

    BOOL IsLockValid();

    inline HANDLE GetHandle()
    {
        return m_mutexHandle;
    }

    inline operator HANDLE()
    {
        return m_mutexHandle;
    }

private:

    CMutexLock( const CMutexLock& );
    CMutexLock & operator = ( const CMutexLock & );

private:

    HANDLE      m_mutexHandle;      //  互斥句柄
    DWORD       m_dwMilliseconds;   //  等待时间
};

#endif  //  _LOCK_MUTEX_H_