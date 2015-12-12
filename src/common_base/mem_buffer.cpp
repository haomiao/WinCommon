#include "stdafx.h"
#include "mem_buffer.h"

CMemBuffer::CMemBuffer():
m_pBuffer(NULL),
m_bufferSize(0),
m_reserveBufferSize(0)
{

}


CMemBuffer::~CMemBuffer()
{
    if ( m_pBuffer != NULL)
    {
        free(m_pBuffer);
        m_pBuffer = NULL;
    }
}


void* CMemBuffer::GetBuffer()
{
    return m_pBuffer;
}


size_t CMemBuffer::GetSize() const
{
    return m_bufferSize;
}


bool CMemBuffer::Alloc( size_t size )
{
    if ( m_pBuffer != NULL && m_reserveBufferSize >= size)
    {
        m_bufferSize = size;
        return true;
    }

    if (m_pBuffer != NULL)
    {
        free( m_pBuffer );
    }

    m_pBuffer = malloc(size);

    if ( m_pBuffer == NULL )
    {
        m_bufferSize = 0;
        return false;
    }

    m_bufferSize        = size;
    m_reserveBufferSize = size;
    memset( m_pBuffer, 0, m_bufferSize );
    return true;
}


bool CMemBuffer::ReAlloc( size_t size, bool copy_data )
{
    if ( m_pBuffer == NULL)
    {
        m_pBuffer = malloc( size );
        if ( m_pBuffer == NULL )
        {
            return false;
        }
        
        memset( m_pBuffer, 0, size );
        m_reserveBufferSize = size;
    }
    else if ( m_reserveBufferSize < size )
    {
        void * pNew = malloc( size );
        if ( pNew == NULL )
        {
            return false;
        }

        memset( pNew, 0, size );
        if ( copy_data == true)
        {
            memcpy( pNew, m_pBuffer, m_bufferSize );
        }
        free( m_pBuffer );
        m_pBuffer = pNew;
        m_reserveBufferSize = size;
    }

    m_bufferSize = size;
    return true;
}


bool CMemBuffer::CopyData( const void* srcMem, size_t size )
{
    if ( srcMem == NULL )
    {
        return false;
    }

    if ( false == Alloc( size ) )
    {
        return false;
    }

    memcpy( m_pBuffer, srcMem, size );
    return true;
}