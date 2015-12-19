///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   hook.h
/// 	@brief	HOOK�ļ򵥷�װ. 
/// 
///		��Ҫʵ�ֶ�HOOK(����)�Ľӿڼ򵥷�װ������ά��  
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

//  HOOK���ӣ�����ʵ�֣�Ŀǰ��֧��Ϊ�ֲ�����
//  �ù��ӿ�����Ҫ��ʱ��������ӡ���װ����
//  ��ʱʵ��Ϊ�ֲ����ӣ���dwThreadId����Ϊ0ʱ����Ϊϵͳ����;����Ϊ�̹߳���
//  ������Ҫʵ��Զ�̹��ӣ�����Ҫ�����ӷ�װΪDLL
class CHook
{

public:

    /**
    *  @brief  ��Ӱ�װĳ���͵�HOOK
    *  @param  [in] 	idHook          HOOK����
    *  @param  [in] 	lpfn            HOOK������
    *  @param  [in] 	hMod            HOOKӦ�ý���ʵ��
    *  @param  [in] 	dwThreadId      HOOKӦ�ý���ĳ�߳�
    */
    HHOOK AddHook( int idHook, HOOKPROC lpfn, HINSTANCE hMod, DWORD dwThreadId );

    /**
    *  @brief  ж��ĳ���͵�HOOK
    *  @param  [in] 	idHook          HOOK����
    */
    BOOL ReleaseHook( int idHook );

    /**
    *  @brief  ж�����е�HOOK
    */
    void ClearAllHook();

protected:

    CHook();

    virtual ~CHook();

private:

    typedef std::map<int, HHOOK> HOOKMAP;
    HOOKMAP         m_hookMap;              //  HOOK����ӳ��
};

typedef CSingletonAutoT2<CHook> CSingletonHook;

#endif  //  _HOOK_H_
