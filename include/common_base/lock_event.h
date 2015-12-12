///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_event.h  
/// 	@brief	事件锁. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/03  17:43:52
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _LOCK_EVENT_H_
#define     _LOCK_EVENT_H_

#include "lock_base.h"

/**
 * \brief 事件锁
 */
class CEventLock : public ILockBase
{
public:

    /**
      *  @brief  构造函数  
      *  @param  [in] 	bCreateFlag             创建标识 
      *  @param  [in] 	lpEventAttributes       事件安全属性
      *  @param  [in] 	bManualReset            手动重置标识
      *  @param  [in] 	bInitialState           初始化事件状态
      *  @param  [in] 	lpName                  事件命名
      */
    CEventLock( BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpEventAttributes = NULL, BOOL bManualReset = TRUE, BOOL bInitialState = TRUE, LPCTSTR lpName = NULL);

    virtual ~CEventLock();

    //  打开已创建的事件
    BOOL OpenEvent( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName );

    virtual void Lock();

    virtual void UnLock();

    BOOL IsLockValid();

    inline HANDLE GetHandle()
    {
        return m_eventHandle;
    }

    inline operator HANDLE()
    {
        return m_eventHandle;
    }

private:

    CEventLock( const CEventLock& );
    CEventLock & operator = ( const CEventLock & );

private:

    HANDLE      m_eventHandle;      //  事件句柄
};

#endif  //  _LOCK_EVENT_H_