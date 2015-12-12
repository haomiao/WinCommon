///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014
///     All rights reserved.  
///  
/// 	@file   delegate.h
/// 	@brief	��Ҫʵ��ί�нӿ�. 
/// 
///		�ṩ��Ҫ��ί�л��ࡢģ���ࡢ����  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/25  9:00:00
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _DELEGATE_H_
#define     _DELEGATE_H_

#include <list>
#include "lock_critical_section.h"


//  ί�л���,�ṩ��Ҫ��ί�нӿ�
class CDelegateBase
{
public:

    CDelegateBase(void *pObject = NULL, void *pFunCallback = NULL);

    CDelegateBase(const CDelegateBase & other);

    virtual ~CDelegateBase();

    //  ���û��ȡί���߶��󡢻ص�����
    inline void SetObject(void *pObject){m_pObject = pObject;}

    inline void SetFunCallback(void *pFunCallback){m_pFunCallback = pFunCallback;}

    inline void * GetObject() const {return m_pObject;}

    virtual void * GetFunCallback() const {return m_pFunCallback;}

    //  ������ǰί�ж���
    virtual CDelegateBase * Copy() const
    { 
        return NULL; 
    }

    virtual BOOL operator == (const CDelegateBase &other);
    virtual BOOL operator != (const CDelegateBase &other);
    CDelegateBase & operator = (const CDelegateBase & other);

    //  ʵ�ֺ�������ִ�лص�
    BOOL operator()(void * pParamter);

protected:

    //  ִ��
    virtual BOOL Invoke( void * pParamter )
    { 
        return FALSE;
    }

private:

    void * m_pObject;       //  ί���߶���
    void * m_pFunCallback;  //  ί�лص�
};


//  �޶���ί��
//  �ص�����Ϊ����ֵΪBOOL,����Ϊvoid *����
//  �������������ͻص����ɼ̳��Ը�����дʵ��
class CDelegateNoneObject: public CDelegateBase
{
public:
    
    //  �����ص���������
    typedef BOOL (*PFunCallback)(void * pParamter);

public:
    
    CDelegateNoneObject(PFunCallback pFunCallback);

    virtual ~CDelegateNoneObject();

    virtual CDelegateBase * Copy() const;

protected:

    virtual BOOL Invoke(void * pParamter);

};


//  ģ��ί������
//  �����ַ�ʽʵ��,1.�̳��ౣ�����ص�������ַ
//                 2.ʹ�û���ı���Ļص�������ַ
//  ����ע�ͺ��Դ��Ϊ����һ��ʵ�ַ�ʽ
//  ���������ַ�ʽ����ʹ�õ���UFunCallBack
template<class O>
class CDelegateT: public CDelegateBase
{
public:

    //  �����ص���������
    typedef BOOL (O::*PFunCallback)(void *pParater);

    union UFunCallBack
    {
        PFunCallback pFunc;
        void * pp;
    };

public:

    CDelegateT(void *pObject = NULL, UFunCallBack pFunCallback = NULL):
      CDelegateBase(pObject, pFunCallback.pp)//,m_pNewFunCallback(pFunCallback.pFunc)
    {

    }

    virtual ~CDelegateT()
    {

    }

    virtual CDelegateBase * Copy() const
    {
        return new CDelegateT(*this);
    }

protected:

    virtual BOOL Invoke(void * pParamter)
    {
        O *pObject = (O *)GetObject();

        UFunCallBack tempCallback;
        tempCallback.pp = GetFunCallback();
        if (pObject != NULL && tempCallback.pp != NULL)
        {
            return (pObject->*tempCallback.pFunc)(pParamter);
        }
//         if (pObject != NULL && m_pNewFunCallback != NULL)
//         {
//             return (pObject->*m_pNewFunCallback)(pParamter);
//         }
        return FALSE;
    }

private:

    //PFunCallback    m_pNewFunCallback;  //  �ص�����
};


//  ί�й�����֮ģ��ί��(���ذ汾1)
template<class O>
CDelegateT<O> * MakeDelegateT(O *pObject = NULL, BOOL (O::*pFunCallback)(void *pParamter) = NULL)
{
    CDelegateT<O>::UFunCallBack func;
    func.pFunc = pFunCallback;
    return new CDelegateT<O>(pObject, func);
}


//  ί�й�����֮ģ��ί��(���ذ汾2)
template<class O>
CDelegateT<O> MakeDelegateT2(O *pObject = NULL, BOOL (O::*pFunCallback)(void *pParamter) = NULL)
{
    CDelegateT<O>::UFunCallBack func;
    func.pFunc = pFunCallback;
    return CDelegateT<O>(pObject, func);
}


//  ί�й�����֮�޶���ί��(���ذ汾1)
inline CDelegateNoneObject * MakeDelegateNoneObject(BOOL (*pFunCallback)(void *pParamter) = NULL)
{
    return new CDelegateNoneObject(pFunCallback);
}


//  ί�й�����֮�޶���ί��(���ذ汾2)
inline CDelegateNoneObject  MakeDelegateNoneObject2(BOOL (*pFunCallback)(void *pParamter) = NULL)
{
    return CDelegateNoneObject(pFunCallback);
}


//  ί���¼�Դ,��ģ�ⶩ�ġ�������
//  �κμ̳���CDelegateBaseί�о���
//  ����ί���¼����޳��ظ���ί��
class CDelegateEventSource
{
public:

    //  �����ص���������
    typedef BOOL (*PFunCallback)(void * pParamter);

public:

    CDelegateEventSource();

    ~CDelegateEventSource();

public:

    //  ���/ɾ��ί��
    void AppendDelegate(const CDelegateBase & delegateObject);
    void DeleteDelegate(const CDelegateBase & delegateObject);

    void operator += (const CDelegateBase &other);
    void operator -= (const CDelegateBase &other);
    
    //  �ɿ���/ɾ�����NoneObject���͵�ί��
    void operator += (PFunCallback pFunCallback);
    void operator -= (PFunCallback pFunCallback);

    //  ���ί�ж���
    void ClearAll();

    //  �Ƿ�ί�ж���Ϊ��
    BOOL IsEmpty();
    operator bool();

    //  ����ִ��ί������
    BOOL operator()(void *pParamter);

private:

    std::list<CDelegateBase *>  m_delegateList; //  ί���б�

    CCriticalSectionLock        m_delegateLock; //  ί���б���Դ������
};


#endif  //  _DELEGATE_H_