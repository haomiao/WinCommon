///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_semaphore .h    
/// 	@brief	�ź�����. 
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
 * \brief �ź�����
 */
class CSemaphoreLock : public ILockBase
{
public:

    /**
      *  @brief  ���캯��  
      *  @param  [in] 	bCreateFlag             ������ʶ 
      *  @param  [in] 	lpSemaphoreAttributes   �ź�����ȫ����
      *  @param  [in] 	lInitialCount           ��ʼ�����ü���
      *  @param  [in] 	lMaximumCount           ������
      *  @param  [in] 	lpName                  �ź�������
      */
    CSemaphoreLock( BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpSemaphoreAttributes = NULL,
         LONG lInitialCount = 1, LONG lMaximumCount = 1, LPCTSTR lpName = NULL);

    virtual ~CSemaphoreLock();

    //  ���Ѵ������ź���
    BOOL OpenEvent( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName );

    //  Lock/Unlockִ�к���ͬ����
    //  Lock���ȴ����ü���,UnLock�����Ӽ�����ʹ�ÿ���
    virtual void Lock();
    virtual void UnLock();

    BOOL IsLockValid();

    //  �������Ӽ�������
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

    HANDLE      m_semaphoreHandle;      //  �ź������
    LONG        m_releaseCount;         //  �ͷż���
};

#endif  //  _LOCK_SEMAPHORE_H_