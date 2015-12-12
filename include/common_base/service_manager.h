///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     All rights reserved.  
///  
/// 	@file   service_manager.h    
/// 	@brief	启动/停止 Windows服务的管理类. 
///  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/12/04  11:13:14
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _SERVICE_MANAGER_H_
#define     _SERVICE_MANAGER_H_


#include <Windows.h>
#include <winsvc.h>
#include <tchar.h>

class CServiceManager
{
public:

    CServiceManager( LPCTSTR lpServiceName ) :
				m_schSCManager(NULL),
                m_schService(NULL)
    {
		memset(m_chServiceName, 0, sizeof(m_chServiceName));
		_tcscmp(m_chServiceName, lpServiceName);
    }

    bool Open()
    {
        Close();

        m_schSCManager = OpenSCManager( 
            NULL,                    // local computer
            NULL,                    // servicesActive database 
            SC_MANAGER_ALL_ACCESS);  // full access rights 

        if (NULL == m_schSCManager) 
        {
            return false;
        }
        
        m_schService = OpenService( 
            m_schSCManager,         // SCM database 
			m_chServiceName,            // name of service 
            SERVICE_ALL_ACCESS);  // full access 

        if (m_schService == NULL)
        {  
            CloseServiceHandle(m_schSCManager);
            return false;
        } 

        return true;
    }


    bool Close()
    {
        if ( m_schSCManager != NULL)
        {
            CloseServiceHandle( m_schSCManager );
        }

        if (m_schService != NULL)
        {
            CloseServiceHandle( m_schService );
        }
        
        return true;
    }


    bool Start()
    {
        if (m_schSCManager == NULL || 
            m_schService == NULL)
        {
            return false;
        }

        DWORD dwBytesNeeded;
        SERVICE_STATUS_PROCESS ssStatus;
        // Check the status in case the service is not stopped. 
        if (!QueryServiceStatusEx( 
            m_schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE) &ssStatus,             // address of structure
            sizeof(SERVICE_STATUS_PROCESS), // size of structure
            &dwBytesNeeded ) )              // size needed if buffer is too small
        {
            return false; 
        }

        // Check if the service is already running. It would be possible
        if(ssStatus.dwCurrentState != SERVICE_STOPPED && ssStatus.dwCurrentState != SERVICE_STOP_PENDING)
        {
            return true; 
        }
        else
        {
            return StartService(
                m_schService,  // handle to service 
                0,           // number of arguments 
                NULL) ? true : false;      // no arguments 

        }
    }


    bool Stop()
    {
        if ( m_schSCManager == NULL ||
            m_schService == NULL )
        {
            return false;
        }

        DWORD dwBytesNeeded;
        SERVICE_STATUS_PROCESS ssStatus;
        // Check the status in case the service is not stopped. 
        if ( !QueryServiceStatusEx(
            m_schService,                     // handle to service 
            SC_STATUS_PROCESS_INFO,         // information level
            (LPBYTE)&ssStatus,             // address of structure
            sizeof( SERVICE_STATUS_PROCESS ), // size of structure
            &dwBytesNeeded ) )              // size needed if buffer is too small
        {
            return false;
        }

        // Check if the service is already running. It would be possible
        // to stop the service here, but for simplicity this example just returns. 
        if ( ssStatus.dwCurrentState == SERVICE_STOPPED || ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
        {
            return true;
        }
        else
        {
            SERVICE_STATUS ss;
            return ControlService( m_schService, SERVICE_CONTROL_STOP, &ss ) ? true : false;
        }
    }


    BOOL GetServiceStatus(SERVICE_STATUS& pStatus )
    {
       if (IsServiceExist() == TRUE)
       {
            return ::QueryServiceStatus( m_schService, &pStatus );
       }

       return FALSE;
    }


    BOOL IsServiceExist()
    {
        if ( m_schSCManager == NULL || m_schService == NULL )
        {
            BOOL bExist = FALSE;
            SC_HANDLE scm = ::OpenSCManager( NULL, NULL, GENERIC_READ );
            if ( scm )
            {
                SC_HANDLE sv = ::OpenService( scm, m_chServiceName, GENERIC_READ );
                if ( sv )
                {
                    bExist = TRUE;
                    ::CloseServiceHandle( sv );
                }
                ::CloseServiceHandle( scm );
            }
            return bExist;
        }
        return TRUE;
    }

private:

    SC_HANDLE   m_schSCManager;

    SC_HANDLE   m_schService;

	TCHAR       m_chServiceName[MAX_PATH];
};

#endif  //  _SERVICE_MANAGER_H_