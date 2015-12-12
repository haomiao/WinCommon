///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_semaphore .h    
/// 	@brief	信号量锁. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/05  14:53:18
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _LOCK_SEMAPHORE_H_
#define     _LOCK_SEMAPHORE_H_

#include "lock_base.h"

/**
 * \brief 信号量锁
 */
class CSemaphoreLock : public ILockBase
{
public:

    /**
      *  @brief  构造函数  
      *  @param  [in] 	bCreateFlag             创建标识 
      *  @param  [in] 	lpSemaphoreAttributes   信号量安全属性
      *  @param  [in] 	lInitialCount           初始化可用计数
      *  @param  [in] 	lMaximumCount           最大计数
      *  @param  [in] 	lpName                  信号量命名
      */
    CSemaphoreLock( BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = NULL,
         LONG lInitialCount = 1, LONG lMaximumCount = 1, LPCTSTR lpName = NULL);

    virtual ~CSemaphoreLock();

    //  打开已创建的信号量
    BOOL OpenEvent( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName );

    //  Lock/Unlock执行含义同计数
    //  Lock：等待可用计数,UnLock：增加计数，使得可用
    virtual void Lock();
    virtual void UnLock();

    BOOL IsLockValid();

    //  设置增加计数个数
    void SetReleaseCount( LONG releaseCount = 1);

    inline HANDLE GetHandle()
    {
        return m_semaphoreHandle;
    }

    inline operator HANDLE()
    {
        return m_semaphoreHandle;
    }

private:

    CSemaphoreLock( const CSemaphoreLock& );
    CSemaphoreLock & operator = ( const CSemaphoreLock & );

private:

    HANDLE      m_semaphoreHandle;      //  信号量句柄
    LONG        m_releaseCount;         //  释放计数
};

#endif  //  _LOCK_SEMAPHORE_H_