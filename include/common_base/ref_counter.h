///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ref_counter.h
/// 	@brief	���ü���. 
/// 
///		��Ҫʵ�����ü�����ع��ܡ�����  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/07  14:25:52
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _REFERENCE_COUNTER_H_
#define     _REFERENCE_COUNTER_H_

#include <string>
#include <Windows.h>
#include <assert.h>
#include "config.h"
#include "lock_critical_section.h"
#include "auto_lock.h"

#if defined(_MSC_VER) && (_MSC_VER <= 1500) 
typedef volatile LONG ATOMIC_T;
#else
typedef unsigned int ATOMIC_T;
#endif

//  ���ü���������
//  �Ƕ��߳������δ�����ڿ����Ч��
//  ��ֱ��ʹ�ø���
class CRefCounterBase
{
public:

    CRefCounterBase();

    virtual ~CRefCounterBase();

	/**
	*	\brief  �������ü���(value:����ֵ������)
	*/
    virtual void AddRef( ATOMIC_T value = 1 );

	/**
	*	\brief  �������ü���(value:����ֵ������)
	*/
    virtual void DecRef( ATOMIC_T value = 1 );

	/**
	*	\brief  ��ȡ��ǰ���ü���
	*/
    virtual ATOMIC_T GetRefCnt();

public:

    virtual void operator++( )
    {
        return AddRef( 1 );
    }

    virtual void operator++( int )
    {
        return AddRef( 1 );
    }

    virtual void operator--( )
    {
        return DecRef( 1 );
    }

    virtual void operator--( int )
    {
        return DecRef( 1 );
    }

    virtual void operator+=( ATOMIC_T value )
    {
        return AddRef( value );
    }

    virtual void operator-=( ATOMIC_T value )
    {
        return DecRef( value );
    }

DISALLOW_COPY_AND_ASSIGN( CRefCounterBase );

protected:

    ATOMIC_T                m_ref;
};


//  ���ü���������
//  ������������,���ڶ��̵߳Ļ�����
class CRefCounterShared :public CRefCounterBase
{
public:

    //  ע��ɾ������ֻ����new�����Ķ����У������������bAutoDelete = false
    //  ��ʹ�ü̳л�ۺϵķ�ʽʹ�ø���
    CRefCounterShared( bool bAutoDelete = true);

    virtual ~CRefCounterShared();

    /**
    *	\brief  �������ü���(value:����ֵ������)
    */
    virtual void AddRef( ATOMIC_T value = 1 );

    /**
    *	\brief  �ͷ����ü���(value:����ֵ������)
    */
    virtual void DecRef( ATOMIC_T value = 1 );

    /**
    *	\brief  ��ȡ��ǰ���ü���
    */
    virtual ATOMIC_T GetRefCnt();

    /**
    *	\brief  ��ѯ�Զ�ɾ����־
    *    deleteFlag:true,���Զ�ɾ��������Ϊ0ʱ�����򲻻�ɾ��
    */
    bool GetAutoDeleteFlag();

DISALLOW_COPY_AND_ASSIGN( CRefCounterShared );

protected:

#if defined(_MSC_VER) && (_MSC_VER <= 1500) 
#else
    CCriticalSectionLock    m_lock;
#endif

    ATOMIC_T                m_ref;
    bool                    m_bAutoDelete;
};

#endif  //      _REFERENCE_COUNTER_H_
