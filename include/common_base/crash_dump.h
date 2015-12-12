///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   crash_dump.h
/// 	@brief	异常捕获类. 
/// 
///		主要实现基本的、常见的异常捕获类的简单封装  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/20  11:36:50
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _CRASH_DUMP_H_
#define     _CRASH_DUMP_H_

#include <stdlib.h>
#include <DbgHelp.h>
#include <eh.h>


//  CCrashDump类，主要封装:SetUnhandledExceptionFilter
//                        _set_invalid_parameter_handler、
//                        _set_purecall_handler
// 若未设置路径，则默认为当前可执行文件路径下，文件名为: Dump.dmp
class CCrashDump
{

public:

    CCrashDump();

    ~CCrashDump();

public:

    /**
    *  @brief  设置窗口进程捕获(若设置有效，则为进程间捕获，否则为进程内捕获)
    *  @param  [in] 	lpClassName        窗口类名
    *  @param  [in] 	lpWindowName       窗口名(标题)
    *  detail：进程间，也可通过管道、共享内存、其他IPC等通信实现，当前版本暂不支持进程间捕获
    */
    static BOOL SetCatchProcessWindow( LPCTSTR lpClassName, LPCTSTR lpWindowName);

    /**
    *  @brief  设置dump捕获文件存储路径(可选)
    *  @param  [in] 	filePathName        文件路径名(全路径)
    */
    static BOOL SetDumpFilePath(LPCTSTR filePathName);

    /**
    *  @brief  设置自定义minidumpCallback选项(可选)
    *  @param  [in] 	miniDumpCallbackFun         minidump扩展回调函数
    */
    static void SetMiniDumpCallback( MINIDUMP_CALLBACK_ROUTINE miniDumpCallbackFun, PVOID callbackFunParam);

    /**
    *  @brief  设置PureCall回调选项(可选)
    *  @param  [in] 	pureCallbackFun             纯虚函数调用异常回调函数
    */
    static void SetPurecallHandlerCallback( _purecall_handler pureCallbackFun);

    /**
    *  @brief  设置PureCall回调选项(可选)
    *  @param  [in] 	invalidParamCallbackFun     无效参数调用异常回调函数
    */
    static BOOL SetInvalidParameterHandlerCallback( _invalid_parameter_handler invalidParamCallbackFun );

    /**
    *  @brief   设置默认crash dump存储路径
    */
    static void SetDefaultDumpPath();

private:

    /**
    *  @brief  自定义的未处理异常、捕获异常
    *  @param  [in] 	pExceptionInfo      异常信息、处理器上下文环境信息
    *  @return 				            返回处理结果状态
    */
    static LONG WINAPI CustomUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo );

private:

    /**
    *  @brief  创建并存储捕获异常信息文件
    *  @param  [in] 	filePathName        dump文件存储位置
    *  @param  [in] 	pExceptionInfo      异常信息、处理器上下文环境信息
    *  @return 				                返回成功与否状态
    */
    static BOOL CreateMiniDump( LPCTSTR filePathName, PEXCEPTION_POINTERS pExceptionInfo );

private:

    static CString                      m_processWindowName;                //  窗口名(标题)
    static CString                      m_processWindowClassName;           //  窗口类名
    static CString                      m_filePathName;                     //  dump存储路径
    static MINIDUMP_CALLBACK_ROUTINE    m_miniDumpCallbackFun;              //  miniDump扩展回调
    static PVOID                        m_callbackFunParam;                 //  扩展回调参数
    
    static _purecall_handler            m_oldPureCallbackFun;               //  早期纯虚函数调用异常回调
    static _invalid_parameter_handler   m_oldInvalidParamCallbackFun;       //  早期错误参数调用异常回调

    static PTOP_LEVEL_EXCEPTION_FILTER  m_oldUnHandledExceptionFilter;      //  早期miniDump扩展回调

    static HWND                         m_processWnd;                       //  进程窗口句柄

};  //  CCrashDump


//  Windows结构化异常转化为C++异常
//  可便于C++中异常统一处理
//  使用时VC下可能需要开启\EHa异常捕获模式

#define SET_SE_TRANSLATOR SetSETranslator();

class CStructExecpt2CplusplusExcept
{
public:

    CStructExecpt2CplusplusExcept( UINT exceptionCode,
        PEXCEPTION_POINTERS pExp ) :m_exceptionCode( exceptionCode ), m_pExectionPtr( pExp ),
        m_exceptionRecord( *m_pExectionPtr->ExceptionRecord ),
        m_context( *m_pExectionPtr->ContextRecord )
    {

    };

    ~CStructExecpt2CplusplusExcept()
    {

    };

public:

    //  设置SE异常转化器(每个线程需要开启、设置)
    static void SetSETranslator()
    {
        _set_se_translator( TransFunc );
    }

    operator DWORD() 
    {
        //return m_pExectionPtr->ExceptionRecord->ExceptionCode;
        return m_exceptionCode;
    }

    PEXCEPTION_POINTERS GetExceptionPointers()
    {
        return m_pExectionPtr;
    }

    PEXCEPTION_RECORD GetExceptionRecord()
    {
        return &m_exceptionRecord;
    }

    PCONTEXT GetExceptionContext()
    {
        return &m_context;
    }

private:

    static void TransFunc( unsigned int exceptionCode, PEXCEPTION_POINTERS pExp )
    {
        throw CStructExecpt2CplusplusExcept( exceptionCode, pExp );
    }

private:

    PEXCEPTION_POINTERS     m_pExectionPtr;
    EXCEPTION_RECORD        m_exceptionRecord;
    CONTEXT                 m_context;
    UINT                    m_exceptionCode;

};  //  CStructExecpt2CplusplusExcept


#endif  //  _CRASH_DUMP_H_