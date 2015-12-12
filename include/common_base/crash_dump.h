///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   crash_dump.h
/// 	@brief	�쳣������. 
/// 
///		��Ҫʵ�ֻ����ġ��������쳣������ļ򵥷�װ  
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


//  CCrashDump�࣬��Ҫ��װ:SetUnhandledExceptionFilter
//                        _set_invalid_parameter_handler��
//                        _set_purecall_handler
// ��δ����·������Ĭ��Ϊ��ǰ��ִ���ļ�·���£��ļ���Ϊ: Dump.dmp
class CCrashDump
{

public:

    CCrashDump();

    ~CCrashDump();

public:

    /**
    *  @brief  ���ô��ڽ��̲���(��������Ч����Ϊ���̼䲶�񣬷���Ϊ�����ڲ���)
    *  @param  [in] 	lpClassName        ��������
    *  @param  [in] 	lpWindowName       ������(����)
    *  detail�����̼䣬Ҳ��ͨ���ܵ��������ڴ桢����IPC��ͨ��ʵ�֣���ǰ�汾�ݲ�֧�ֽ��̼䲶��
    */
    static BOOL SetCatchProcessWindow( LPCTSTR lpClassName, LPCTSTR lpWindowName);

    /**
    *  @brief  ����dump�����ļ��洢·��(��ѡ)
    *  @param  [in] 	filePathName        �ļ�·����(ȫ·��)
    */
    static BOOL SetDumpFilePath(LPCTSTR filePathName);

    /**
    *  @brief  �����Զ���minidumpCallbackѡ��(��ѡ)
    *  @param  [in] 	miniDumpCallbackFun         minidump��չ�ص�����
    */
    static void SetMiniDumpCallback( MINIDUMP_CALLBACK_ROUTINE miniDumpCallbackFun, PVOID callbackFunParam);

    /**
    *  @brief  ����PureCall�ص�ѡ��(��ѡ)
    *  @param  [in] 	pureCallbackFun             ���麯�������쳣�ص�����
    */
    static void SetPurecallHandlerCallback( _purecall_handler pureCallbackFun);

    /**
    *  @brief  ����PureCall�ص�ѡ��(��ѡ)
    *  @param  [in] 	invalidParamCallbackFun     ��Ч���������쳣�ص�����
    */
    static BOOL SetInvalidParameterHandlerCallback( _invalid_parameter_handler invalidParamCallbackFun );

    /**
    *  @brief   ����Ĭ��crash dump�洢·��
    */
    static void SetDefaultDumpPath();

private:

    /**
    *  @brief  �Զ����δ�����쳣�������쳣
    *  @param  [in] 	pExceptionInfo      �쳣��Ϣ�������������Ļ�����Ϣ
    *  @return 				            ���ش�����״̬
    */
    static LONG WINAPI CustomUnhandledExceptionFilter( PEXCEPTION_POINTERS pExceptionInfo );

private:

    /**
    *  @brief  �������洢�����쳣��Ϣ�ļ�
    *  @param  [in] 	filePathName        dump�ļ��洢λ��
    *  @param  [in] 	pExceptionInfo      �쳣��Ϣ�������������Ļ�����Ϣ
    *  @return 				                ���سɹ����״̬
    */
    static BOOL CreateMiniDump( LPCTSTR filePathName, PEXCEPTION_POINTERS pExceptionInfo );

private:

    static CString                      m_processWindowName;                //  ������(����)
    static CString                      m_processWindowClassName;           //  ��������
    static CString                      m_filePathName;                     //  dump�洢·��
    static MINIDUMP_CALLBACK_ROUTINE    m_miniDumpCallbackFun;              //  miniDump��չ�ص�
    static PVOID                        m_callbackFunParam;                 //  ��չ�ص�����
    
    static _purecall_handler            m_oldPureCallbackFun;               //  ���ڴ��麯�������쳣�ص�
    static _invalid_parameter_handler   m_oldInvalidParamCallbackFun;       //  ���ڴ�����������쳣�ص�

    static PTOP_LEVEL_EXCEPTION_FILTER  m_oldUnHandledExceptionFilter;      //  ����miniDump��չ�ص�

    static HWND                         m_processWnd;                       //  ���̴��ھ��

};  //  CCrashDump


//  Windows�ṹ���쳣ת��ΪC++�쳣
//  �ɱ���C++���쳣ͳһ����
//  ʹ��ʱVC�¿�����Ҫ����\EHa�쳣����ģʽ

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

    //  ����SE�쳣ת����(ÿ���߳���Ҫ����������)
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