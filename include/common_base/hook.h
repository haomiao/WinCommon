///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   hook.h
/// 	@brief	HOOK的简单封装. 
/// 
///		主要实现对HOOK(钩子)的接口简单封装，便于维护  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/18  9:03:59
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _HOOK_H_
#define     _HOOK_H_

#include <windows.h>
#include <map>

#include "singleton.h"

//  HOOK钩子，单例实现，目前仅支持为局部钩子
//  该钩子可在需要的时候任意添加、安装钩子
//  暂时实现为局部钩子，当dwThreadId参数为0时，则为系统钩子;否则为线程钩子
//  此外若要实现远程钩子，则需要将钩子封装为DLL
class CHook
{

public:

    /**
    *  @brief  添加安装某类型的HOOK
    *  @param  [in] 	idHook          HOOK类型
    *  @param  [in] 	lpfn            HOOK处理函数
    *  @param  [in] 	hMod            HOOK应用进程实例
    *  @param  [in] 	dwThreadId      HOOK应用进程某线程
    */
    HHOOK AddHook( int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId );

    /**
    *  @brief  卸载某类型的HOOK
    *  @param  [in] 	idHook          HOOK类型
    */
    BOOL ReleaseHook( int idHook );

    /**
    *  @brief  卸载所有的HOOK
    */
    void ClearAllHook();

protected:

    CHook();

    virtual ~CHook();

private:

    typedef std::map<int, HHOOK> HOOKMAP;
    HOOKMAP         m_hookMap;              //  HOOK类型映射
};

typedef CSingletonAutoT2<CHook> CSingletonHook;

#endif  //  _HOOK_H_
