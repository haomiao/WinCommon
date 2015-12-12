///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   function_scope_guard.h   
/// 	@brief	函数范围守护. 
/// 
///		主要实现简单的函数范围守护工具  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/04  10:23:24
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _FUNCTION_SCOPEGUARD_H_
#define     _FUNCTION_SCOPEGUARD_H_

#include <functional>

template<typename _TRet>
class CFuncScopeGuard
{
public:

    typedef std::function<_TRet(void) > scope_func;

public:

    CFuncScopeGuard(const scope_func &enter, const scope_func &leave)
        :m_leaveFunc(leave)
    {
        if (enter)
        {
            enter();
        }
    }

    CFuncScopeGuard(const scope_func &leave) :m_leaveFunc(leave){}

    ~CFuncScopeGuard()
    {
        if (m_leaveFunc)
        {
            m_leaveFunc();
        }
    }


private:

    CFuncScopeGuard(const CFuncScopeGuard &){}
    CFuncScopeGuard &operator=(const CFuncScopeGuard &){}

protected:

    scope_func m_leaveFunc;
};


typedef CFuncScopeGuard<void> CFuncVoidScopeGuard;


#endif  //  _FUNCTION_SCOPEGUARD_H_
