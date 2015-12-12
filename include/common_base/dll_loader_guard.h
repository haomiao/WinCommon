///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2014
///     All rights reserved.  
///  
/// 	@file   dll_loader_guard.h
/// 	@brief	加载dll封装. 
/// 
///		提供简单的dll加载封装实现  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/10  9:00:00
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _DLL_LOADER_H_
#define     _DLL_LOADER_H_

class CDllLoaderGuard
{
public:

    CDllLoaderGuard():m_hModule(NULL),m_isOk(FALSE)
    {
		
    }

    ~CDllLoaderGuard()
    {
        if (m_hModule != NULL)
        {
            ::FreeLibrary(m_hModule);
        }
    }
	

    inline BOOL SetDllPath(LPCTSTR pFileName)
    {
        if ( pFileName  == NULL)
        {
            return FALSE;
        }

        if ( m_hModule != NULL || m_isOk  == TRUE)
        {
            ::FreeLibrary( m_hModule );
        }

        m_hModule = ::LoadLibrary(pFileName);
        m_isOk = m_hModule == NULL ? FALSE : TRUE;
		return m_isOk;
    }
	

    inline FARPROC  GetProcAddress(LPCSTR pProcName)
    {
        if (m_isOk)
        {
            return ::GetProcAddress(m_hModule, pProcName);
        }

        return NULL;
    }


    inline HRSRC FindResource(LPCTSTR pName, LPCTSTR pType)
    {
        if (m_isOk)
        {
            return ::FindResource(m_hModule, pName, pType);
        }

        return NULL;
    }


    inline BOOL isLoadOk()
    {
        return m_isOk;
    }

private:

    CDllLoaderGuard(const CDllLoaderGuard &);

    CDllLoaderGuard & operator = (const CDllLoaderGuard &);

private:
  
    HMODULE         m_hModule;

    BOOL            m_isOk;
};

#endif      //    _DLL_LOADER_H_