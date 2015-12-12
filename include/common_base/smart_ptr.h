///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   smart_ptr.h
/// 	@brief	����ָ����ز����ķ�װ. 
/// 
///		��Ҫʵ�ֶ�������ָ�벢ʵ�ֱ�Ҫ�ı����ӿڵķ�װ  
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
*	\brief �κμ̳���CRefCounterShared���࣬����ʹ�ø���ﵽ����ָ���Ч��
*   ģ�����T����Ϊ�̳���CRefCounterShared����κ���
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
    *	\brief ע�⣺�ú����������쳣����ָ��Ϊ��ʱ���쳣
    *			��ʱ�Ҳ����ڴ˴������쳣�ķ�����
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
    *	\brief ע�⣺�ú����������쳣����ָ��Ϊ��ʱ���쳣
    *			��ʱ�Ҳ����ڴ˴������쳣�ķ�����
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
    *	\brief �ﵽʹ��if(x)�ж�ָ���Ƿ�Ϊ�յ�Ч����xΪCSmarPtr����
    */
    operator bool() const
    {
        return m_pRef ? true : false;
    }

    /**
    *	\brief ʵ�ֶ�̬����ת��
    */
    template<typename Y>
    static CSmartPtr dynamicCast( const CSmartPtr<Y> &other )
    {
        return CSmartPtr<T>( dynamic_cast<T*>( other.m_pRef ) );
    }

    /**
    *	\brief ʵ�ֽ�ͬһ�̳���ϵ��ָ������ת���ɸ�����ָ�����
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