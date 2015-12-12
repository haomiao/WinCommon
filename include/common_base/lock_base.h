///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_base.h
/// 	@brief	锁基类，提供锁公共接口. 
/// 
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/27  9:12:52
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _LOCK_BASE_H_
#define     _LOCK_BASE_H_

/**
 * \brief 锁接口
 */
class ILockBase
{
public:

    virtual ~ILockBase() {}

    /**
     * \brief 加锁
     */
    virtual void Lock() = 0;

    /**
     * \brief 解锁
     */
    virtual void UnLock() = 0;
};

#endif  //  _LOCK_BASE_H_