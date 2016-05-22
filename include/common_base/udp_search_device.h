///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   udp_search_device.h
/// 	@brief	UDP�豸����(�㲥). 
/// 
///		��Ҫ����UDP�㲥����  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2016/04/05  17:55:36
///  
///  
/////////////////////////////////////////////////////////////////////


#include <iostream>
#include <map>
#include <set>
#include <afxsock.h>
#include <IPHlpApi.h>
#include <functional>


//  UDP������ʵ�ֻ����Ĺ㲥����
//  ֧��ͬ�����첽��ʽ
class CUDPBroadCast 
{
public:

    typedef std::map<std::string, std::string> DevMap;

    typedef std::tr1::function<void(const std::string&, const std::string&)> SearchCallBack;

public:

    CUDPBroadCast();

    ~CUDPBroadCast();

public:

    /**
    * @brief:   ���õ�ǰ��������������IP��ַ
    * @param[in]:   ipAddress       ������IP��ַ(��ΪNULL�����ʾ������ǰ�豸����������IP��ַ)
    * @param[in]:   port            ɨ��˿ں�(�����豸��Ӧ���͵Ķ˿ں�)
    */
    bool SetAdatpterIPAddress( const char *ipAddress = NULL, unsigned short port = MAXUINT16 );

    /**
    * @brief:   ��������Ƿ����
    */
    bool IsNetValid();

    /**
    * @brief:   ��ʼ�㲥(�첽�㲥,ͨ�������̵߳����㲥��Ϣ)
    * @param[in]:   broadCastInfo   �㲥��Ϣ
    * @param[in]:   searchTimeout   ������ʱ(���ÿ���������µĳ�ʱ����Ϊ�ܵĳ�ʱ����usΪ��λ)
    * @param[in]:   asyncSearch     �첽����(trueΪ�첽�� falseΪͬ��)
    * @param[in]:   searchCallBack  �����ص�,���������豸ʱ�������õĺ���,����ΪDevMap����Ϣ
    * @note:        ע�⣺�첽�㲥ʱ�����ܻ��յ���ͬ���豸�Ķ��Ӧ��
    */
    bool BroadCast( const char *broadCastInfo, unsigned int searchTimeout = 600000, bool asyncSearch = true, SearchCallBack searchCallBack = nullptr);

    /**
    * @brief:   ��ȡ�������
    * @param[in]:   devMap          ���������Ϣ
    * @param[in]:   waitTimeout     �ȴ���ʱ
    * @return:      ���������豸��,-1Ϊ�����쳣,һ��Ϊ��ʱ
    */
    unsigned int GetDevMapInfo( DevMap &devMap, DWORD waitTimeout = INFINITE);

private:

    /**
    * @brief:   �����豸�̺߳��� 
    */
    static unsigned __stdcall SearchDevThread( void *pParam );

    /**
    * @brief:   ��ȡ��������������Ϣ
    */
    void GetHostAdapterInfo();

    /**
    * @brief:   �����豸����
    */
    void Scanning();

private:

	PIP_ADAPTER_INFO    m_pAdatpterInfo;        //  �����豸������Ϣ	
	std::set<ULONG>     m_dirAddressSet;        //  ֱ�ӹ㲥��ַ
    HANDLE              m_searchEventHandle;    //  �����˳��¼�
	UINT                m_searchDevNum;         //  �����豸��
    std::string         m_adatpterAddress;      //  ��������ַ
    unsigned short      m_port;                 //  ɨ��˿ں�
    std::string         m_broadCastInfo;        //  �㲥��Ϣ
    bool                m_isNetComValid;        //  �Ƿ�������ؿ���
    std::map<std::string, std::string> m_devMap;//  �豸IP-Ӧ��MAP��
    SearchCallBack      m_searchCallBack;       //  �����ص�����
    unsigned int        m_searchTimeout;        //  ������ʱ
    bool                m_isAsyncSearch;        //  �Ƿ��첽����

};
