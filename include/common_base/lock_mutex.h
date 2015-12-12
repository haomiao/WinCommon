///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_mutex.h   
/// 	@brief	������. 
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
 * \brief ������
 */
class CMutexLock : public ILockBase
{
public:

    /**
      *  @brief  ���캯��  
      *  @param  [in] 	bCreateFlag             ������ʶ 
      *  @param  [in] 	lpMutexAttributes       ��������ȫ����
      *  @param  [in] 	bInitialOwner           ��ʼ����ӵ����
      *  @param  [in] 	lpName                  ������
      */
    CMutexLock(BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL, BOOL bInitialOwner = FALSE, LPCTSTR lpName = NULL);

    virtual ~CMutexLock();

    //  ���Ѵ����Ļ�����
    BOOL OpenMutex( DWORD dwDesiredAccess, BOOL bInheritHandle, LPCTSTR lpName);

    virtual void Lock();

    virtual void UnLock();

    //  �������ȴ�ʱ��
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

    HANDLE      m_mutexHandle;      //  ������
    DWORD       m_dwMilliseconds;   //  �ȴ�ʱ��
};

#endif  //  _LOCK_MUTEX_H_