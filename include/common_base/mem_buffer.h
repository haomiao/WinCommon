///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   mem_buffer.h
/// 	@brief	内存缓冲区资源管理. 
/// 
///		主要提供一个简单的内存缓冲区资源管理的工具类，实现自动资源释放的功能  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/07  14:53:58
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _MEM_BUFFER_H_
#define     _MEM_BUFFER_H_

#include "ref_counter.h"
#include "smart_ptr.h"

class CMemBuffer : public CRefCounterShared
{
public:

    CMemBuffer();

    virtual ~CMemBuffer();

    /**
		\brief 获取管理的内存块
		\return 返回NULL 表示无效
	*/
	void * GetBuffer();

	/**
		\brief 返回内存块的大小
		\return 0表示内存块无效
	*/
	size_t GetSize() const;

	/**
		\brief 分配内存空间
	*/
	bool Alloc(size_t size);

	/**
		\brief 重新分配内存空间
        若copy_data为ture且size>m_bufferSize,则只拷贝两者最小的长度
	*/
	bool ReAlloc(size_t size, bool copy_data = true);

    /**
    *   \brief 拷贝srcMem中的数据至m_pBuffer
    */
    bool CopyData(const void* srcMem, size_t size);

protected:

    void            *m_pBuffer;
    size_t          m_bufferSize;
    size_t          m_reserveBufferSize;
};


//  使用时示例：  MemBufferPtr pBuffer = new CMemBuffer();
//               pBuffer->Alloc( 24 );
//               pBuffer->ReAlloc( 36 );
typedef CSmartPtr<CMemBuffer> MemBufferPtr;


#endif  //  _MEM_BUFFER_H_