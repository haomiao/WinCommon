/////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   identity_master.h
/// 	@brief	身份认证管理. 
/// 
///		主要实现简单的身份认证管理器  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/10/15  10:14:12
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _IDENTITY_MASTER_H_
#define     _IDENTITY_MASTER_H_

#include <string>
#include <map>
#include "lock_critical_section.h"


class IdentityMaster 
{

public:

	/**
	*	\brief  如果key为NULL则默认前缀为identity
	*	\param[in] key 统一名称的前缀,如果key为：identity,则从此处得到名字形式如下：identity1,identity2....
	*/
	IdentityMaster(const char *key = NULL);

	/**
	*	\brief 有名称需要向管理器汇报，才不会产生重复内容。
	*/
	bool AddIdentity(const char *identity);

	/**
	*	\brief 产生一个名称。
	*/
	bool GetIdentity(std::string &identity);

	/**
	*	\brief 释放一个名称。
	*/
	bool ReleaseIdentity(const std::string &identity);

protected:

	typedef std::map<unsigned long, char> MapInt2Char;

	unsigned long _index;

	std::string _key;

	MapInt2Char _map_idle_identity; 

	MapInt2Char _map_using_identity;

	CCriticalSectionLock _lock;
};


#endif /* _IDENTITY_MASTER_H_ */
