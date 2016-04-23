///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   udp_search_device.h
/// 	@brief	UDP设备搜索(广播). 
/// 
///		主要基于UDP广播搜索  
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


//  UDP搜索，实现基本的广播操作
//  支持同步和异步方式
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
    * @brief:   设置当前搜索网卡适配器IP地址
    * @param[in]:   ipAddress       适配器IP地址(当为NULL，则表示搜索当前设备所有适配器IP地址)
    * @param[in]:   port            扫描端口号(搜索设备对应类型的端口号)
    */
    bool SetAdatpterIPAddress( const char *ipAddress = NULL, unsigned short port = MAXUINT16 );

    /**
    * @brief:   网络组件是否可用
    */
    bool IsNetValid();

    /**
    * @brief:   开始广播(异步广播,通过开启线程单独广播信息)
    * @param[in]:   broadCastInfo   广播信息
    * @param[in]:   searchTimeout   搜索超时(针对每个适配器下的超时，不为总的超时；以us为单位)
    * @param[in]:   asyncSearch     异步搜索(true为异步， false为同步)
    * @param[in]:   searchCallBack  搜索回调,当搜索到设备时将被调用的函数,参数为DevMap对信息
    * @note:        注意：异步广播时，可能会收到相同的设备的多次应答
    */
    bool BroadCast( const char *broadCastInfo, unsigned int searchTimeout = 600000, bool asyncSearch = true, SearchCallBack searchCallBack = nullptr);

    /**
    * @brief:   获取搜索结果
    * @param[in]:   devMap          搜索结果信息
    * @param[in]:   waitTimeout     等待超时
    * @return:      返回搜索设备数,-1为搜索异常,一般为超时
    */
    unsigned int GetDevMapInfo( DevMap &devMap, DWORD waitTimeout = INFINITE);

private:

    /**
    * @brief:   搜索设备线程函数 
    */
    static unsigned __stdcall SearchDevThread( void *pParam );

    /**
    * @brief:   获取本机器适配器信息
    */
    void GetHostAdapterInfo();

    /**
    * @brief:   搜索设备操作
    */
    void Scanning();

private:

	PIP_ADAPTER_INFO    m_pAdatpterInfo;        //  网卡设备配置信息	
	std::set<ULONG>     m_dirAddressSet;        //  直接广播地址
    HANDLE              m_searchEventHandle;    //  搜索退出事件
	UINT                m_searchDevNum;         //  搜索设备数
    std::string         m_adatpterAddress;      //  适配器地址
    unsigned short      m_port;                 //  扫描端口号
    std::string         m_broadCastInfo;        //  广播信息
    bool                m_isNetComValid;        //  是否组件加载可用
    std::map<std::string, std::string> m_devMap;//  设备IP-应答MAP对
    SearchCallBack      m_searchCallBack;       //  搜索回调函数
    unsigned int        m_searchTimeout;        //  搜索超时
    bool                m_isAsyncSearch;        //  是否异步搜索

};
