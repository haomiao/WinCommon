///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   smart_ptr.h
/// 	@brief	智能指针相关操作的封装. 
/// 
///		主要实现多种智能指针并实现必要的便利接口的封装  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/07  11:44:25
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _SMART_PTR_H_
#define     _SMART_PTR_H_

#include "ref_counter.h"
#include "string_helper.h"

/**
*	\brief 任何继承至CRefCounterShared的类，可以使用该类达到智能指针的效果
*   模板参数T：即为继承至CRefCounterShared类的任何类
*/
template<typename T = CRefCounterShared>
class CSmartPtr
{
public:

    CSmartPtr( T* pRef = NULL )
    {
        m_pRef = pRef;
        if ( m_pRef != NULL )
        {
            m_pRef->AddRef();
        }
    }

    CSmartPtr( const CSmartPtr<T>& other )
    {
        m_pRef = other.m_pRef;
        if ( m_pRef != NULL )
        {
            m_pRef->AddRef();
        }
    }

    ~CSmartPtr()
    {
        if ( m_pRef != NULL )
        {
            m_pRef->DecRef();
        }
    }

    CSmartPtr& operator = ( const CSmartPtr<T> &other )
    {
        if ( other.m_pRef != m_pRef )
        {
            if ( m_pRef != NULL )
            {
                m_pRef->DecRef();
            }

            m_pRef = other.m_pRef;
            if ( m_pRef != NULL )
            {
                m_pRef->AddRef();
            }

        }
        return *this;
    }

    CSmartPtr& operator = ( T* pRef )
    {
        if ( m_pRef != pRef )
        {
            if ( m_pRef != NULL )
            {
                m_pRef->DecRef();
            }

            m_pRef = pRef;
            if ( m_pRef != NULL )
            {
                m_pRef->AddRef();
            }
            
        }
        return *this;
    }

    T* get() const
    {
        return m_pRef;
    }

    /**
    *	\brief 注意：该函数可能抛异常。当指针为空时抛异常
    *			暂时找不到在此处不抛异常的方案。
    */
    T* operator ->( ) const
    {
        assert( m_pRef != NULL );
        if ( !m_pRef )
        {
            ThrowNullException();
        }
        return m_pRef;
    }

    /**
    *	\brief 注意：该函数可能抛异常。当指针为空时抛异常
    *			暂时找不到在此处不抛异常的方案。
    */
    T& operator *( ) const
    {
        assert( m_pRef != NULL );
        if ( !m_pRef )
        {
            ThrowNullException();
        }
        return *( m_pRef );
    }

    /**
    *	\brief 达到使用if(x)判断指针是否为空的效果（x为CSmarPtr对象）
    */
    operator bool() const
    {
        return m_pRef ? true : false;
    }

    /**
    *	\brief 实现动态类型转换
    */
    template<typename Y>
    static CSmartPtr dynamicCast( const CSmartPtr<Y> &other )
    {
        return CSmartPtr<T>( dynamic_cast<T*>( other.m_pRef ) );
    }

    /**
    *	\brief 实现将同一继承体系的指针类型转换成该智能指针对象
    */
    template<typename Y>
    static CSmartPtr dynamicCast( Y* p )
    {
        return CSmartPtr<T>( dynamic_cast<T*>( p ) );
    }

protected:

    void ThrowNullException() const
    {
        std::string strErrMsg;
        strErrMsg = StringFormatA("SmartPtr using an null pointer at file:%s, line:%d\n" ,__FILE__, __LINE__);
        throw  strErrMsg;
    }

public:

    T   *m_pRef;
};

#endif  //  _SMART_PTR_H_