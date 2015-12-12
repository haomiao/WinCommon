///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014
///     All rights reserved.  
///  
/// 	@file   delegate.h
/// 	@brief	主要实现委托接口. 
/// 
///		提供必要的委托基类、模板类、工具  
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


//  委托基类,提供必要的委托接口
class CDelegateBase
{
public:

    CDelegateBase(void *pObject = NULL, void *pFunCallback = NULL);

    CDelegateBase(const CDelegateBase & other);

    virtual ~CDelegateBase();

    //  设置或获取委托者对象、回调函数
    inline void SetObject(void *pObject){m_pObject = pObject;}

    inline void SetFunCallback(void *pFunCallback){m_pFunCallback = pFunCallback;}

    inline void * GetObject() const {return m_pObject;}

    virtual void * GetFunCallback() const {return m_pFunCallback;}

    //  拷贝当前委托对象
    virtual CDelegateBase * Copy() const
    { 
        return NULL; 
    }

    virtual BOOL operator == (const CDelegateBase &other);
    virtual BOOL operator != (const CDelegateBase &other);
    CDelegateBase & operator = (const CDelegateBase & other);

    //  实现函数调用执行回调
    BOOL operator()(void * pParamter);

protected:

    //  执行
    virtual BOOL Invoke( void * pParamter )
    { 
        return FALSE;
    }

private:

    void * m_pObject;       //  委托者对象
    void * m_pFunCallback;  //  委托回调
};


//  无对象委托
//  回调函数为返回值为BOOL,参数为void *类型
//  若采用其他类型回调，可继承自该类重写实现
class CDelegateNoneObject: public CDelegateBase
{
public:
    
    //  声明回调函数类型
    typedef BOOL (*PFunCallback)(void * pParamter);

public:
    
    CDelegateNoneObject(PFunCallback pFunCallback);

    virtual ~CDelegateNoneObject();

    virtual CDelegateBase * Copy() const;

protected:

    virtual BOOL Invoke(void * pParamter);

};


//  模板委托类型
//  有两种方式实现,1.继承类保存对象回调函数地址
//                 2.使用基类的保存的回调函数地址
//  以下注释后的源码为另外一种实现方式
//  但不论哪种方式，均使用到了UFunCallBack
template<class O>
class CDelegateT: public CDelegateBase
{
public:

    //  声明回调函数类型
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

    //PFunCallback    m_pNewFunCallback;  //  回调函数
};


//  委托构造器之模板委托(重载版本1)
template<class O>
CDelegateT<O> * MakeDelegateT(O *pObject = NULL, BOOL (O::*pFunCallback)(void *pParamter) = NULL)
{
    CDelegateT<O>::UFunCallBack func;
    func.pFunc = pFunCallback;
    return new CDelegateT<O>(pObject, func);
}


//  委托构造器之模板委托(重载版本2)
template<class O>
CDelegateT<O> MakeDelegateT2(O *pObject = NULL, BOOL (O::*pFunCallback)(void *pParamter) = NULL)
{
    CDelegateT<O>::UFunCallBack func;
    func.pFunc = pFunCallback;
    return CDelegateT<O>(pObject, func);
}


//  委托构造器之无对象委托(重载版本1)
inline CDelegateNoneObject * MakeDelegateNoneObject(BOOL (*pFunCallback)(void *pParamter) = NULL)
{
    return new CDelegateNoneObject(pFunCallback);
}


//  委托构造器之无对象委托(重载版本2)
inline CDelegateNoneObject  MakeDelegateNoneObject2(BOOL (*pFunCallback)(void *pParamter) = NULL)
{
    return CDelegateNoneObject(pFunCallback);
}


//  委托事件源,可模拟订阅、发布者
//  任何继承于CDelegateBase委托均可
//  此外委托事件将剔除重复的委托
class CDelegateEventSource
{
public:

    //  声明回调函数类型
    typedef BOOL (*PFunCallback)(void * pParamter);

public:

    CDelegateEventSource();

    ~CDelegateEventSource();

public:

    //  添加/删除委托
    void AppendDelegate(const CDelegateBase & delegateObject);
    void DeleteDelegate(const CDelegateBase & delegateObject);

    void operator += (const CDelegateBase &other);
    void operator -= (const CDelegateBase &other);
    
    //  可快速/删除添加NoneObject类型的委托
    void operator += (PFunCallback pFunCallback);
    void operator -= (PFunCallback pFunCallback);

    //  清空委托队列
    void ClearAll();

    //  是否委托队列为空
    BOOL IsEmpty();
    operator bool();

    //  遍历执行委托内容
    BOOL operator()(void *pParamter);

private:

    std::list<CDelegateBase *>  m_delegateList; //  委托列表

    CCriticalSectionLock        m_delegateLock; //  委托列表资源保护锁
};


#endif  //  _DELEGATE_H_