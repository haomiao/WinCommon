///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_event.h  
/// 	@brief	�¼���. 
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
 * \brief �¼���
 */
class CEventLock : public ILockBase
{
public:

    /**
      *  @brief  ���캯��  
      *  @param  [in] 	bCreateFlag             ������ʶ 
      *  @param  [in] 	lpEventAttributes       �¼���ȫ����
      *  @param  [in] 	bManualReset            �ֶ����ñ�ʶ
      *  @param  [in] 	bInitialState           ��ʼ���¼�״̬
      *  @param  [in] 	lpName                  �¼�����
      */
    CEventLock( BOOL bCreateFlag = TRUE, LPSECURITY_ATTRIBUTES lpEventAttributes = NULL, BOOL bManualReset = TRUE, BOOL bInitialState = TRUE, LPCTSTR lpName = NULL);

    virtual ~CEventLock();

    //  ���Ѵ������¼�
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

    HANDLE      m_eventHandle;      //  �¼����
};

#endif  //  _LOCK_EVENT_H_