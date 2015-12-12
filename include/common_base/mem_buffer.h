///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   mem_buffer.h
/// 	@brief	�ڴ滺������Դ����. 
/// 
///		��Ҫ�ṩһ���򵥵��ڴ滺������Դ����Ĺ����࣬ʵ���Զ���Դ�ͷŵĹ���  
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
		\brief ��ȡ������ڴ��
		\return ����NULL ��ʾ��Ч
	*/
	void * GetBuffer();

	/**
		\brief �����ڴ��Ĵ�С
		\return 0��ʾ�ڴ����Ч
	*/
	size_t GetSize() const;

	/**
		\brief �����ڴ�ռ�
	*/
	bool Alloc(size_t size);

	/**
		\brief ���·����ڴ�ռ�
        ��copy_dataΪture��size>m_bufferSize,��ֻ����������С�ĳ���
	*/
	bool ReAlloc(size_t size, bool copy_data = true);

    /**
    *   \brief ����srcMem�е�������m_pBuffer
    */
    bool CopyData(const void* srcMem, size_t size);

protected:

    void            *m_pBuffer;
    size_t          m_bufferSize;
    size_t          m_reserveBufferSize;
};


//  ʹ��ʱʾ����  MemBufferPtr pBuffer = new CMemBuffer();
//               pBuffer->Alloc( 24 );
//               pBuffer->ReAlloc( 36 );
typedef CSmartPtr<CMemBuffer> MemBufferPtr;


#endif  //  _MEM_BUFFER_H_