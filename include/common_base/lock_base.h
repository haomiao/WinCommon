///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014 
///     All rights reserved.  
///  
/// 	@file   lock_base.h
/// 	@brief	�����࣬�ṩ�������ӿ�. 
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
 * \brief ���ӿ�
 */
class ILockBase
{
public:

    virtual ~ILockBase() {}

    /**
     * \brief ����
     */
    virtual void Lock() = 0;

    /**
     * \brief ����
     */
    virtual void UnLock() = 0;
};

#endif  //  _LOCK_BASE_H_