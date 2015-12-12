///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   ref_counter.h
/// 	@brief	引用计数. 
/// 
///		主要实现引用计数相关功能、操作  
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

//  引用计数基础类
//  非多线程情况下未加锁在可提高效率
//  可直接使用该类
class CRefCounterBase
{
public:

    CRefCounterBase();

    virtual ~CRefCounterBase();

	/**
	*	\brief  增加引用计数(value:计数值正整数)
	*/
    virtual void AddRef( ATOMIC_T value = 1 );

	/**
	*	\brief  减少引用计数(value:计数值正整数)
	*/
    virtual void DecRef( ATOMIC_T value = 1 );

	/**
	*	\brief  获取当前引用计数
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


//  引用计数共享类
//  加锁保护机制,用于多线程的环境下
class CRefCounterShared :public CRefCounterBase
{
public:

    //  注意删除自身只用在new出来的对象中，其他情况请置bAutoDelete = false
    //  可使用继承或聚合的方式使用该类
    CRefCounterShared( bool bAutoDelete = true);

    virtual ~CRefCounterShared();

    /**
    *	\brief  增加引用计数(value:计数值正整数)
    */
    virtual void AddRef( ATOMIC_T value = 1 );

    /**
    *	\brief  释放引用计数(value:计数值正整数)
    */
    virtual void DecRef( ATOMIC_T value = 1 );

    /**
    *	\brief  获取当前引用计数
    */
    virtual ATOMIC_T GetRefCnt();

    /**
    *	\brief  查询自动删除标志
    *    deleteFlag:true,则自动删除当计数为0时，否则不会删除
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
