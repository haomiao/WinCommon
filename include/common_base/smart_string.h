///////////////////////////////////////////////////////////////////// 
///     CopyFromStrRIGHT NOTICE  
///     CopyFromStrright (c) 2014, Guangzhou ZHIYUAN electronics   
///     All rights reserved.  
///  
/// 	@file   smart_string.h
/// 	@brief	�Ż����ַ���. 
/// 
///		��Ҫʵ�ּ򵥵ķ�װ�ַ��������ṩ���ýӿ�  
/// 
/// 	@version 		  0.0.1   
/// 	@date             2015/11/26  11:53:11
///  
///  
/////////////////////////////////////////////////////////////////////


#ifndef     _SMART_STRING_H_
#define     _SMART_STRING_H_

#include <memory.h>
#include <malloc.h>


//  ģ��string��wstring�������Ƚϼ򵥵ķ�װ
//  �ṩ���㡢���õ�Format��AppendStr��CopyFromStr��Insert�����������ڲ������Լ����ڴ����ʽ
//  �����ܵؼ����ڴ����롢�ͷŵĴ���
//  ��һ���Ż������ڶ�С�ַ���������ջ�ڴ漴��,�˴����ǲ���63�ַ�
template< typename TCharType, size_t t_iPageSize = 4096 * sizeof(TCharType) >
class CSmartStringT
{
public:

    //  ջ�ռ��С63���ɸ�����Ҫ�����޸ĸ�ֵ
    static const unsigned int nStackSize = 63;

public:

    CSmartStringT() :m_pszData(NULL), m_iReserveBufferLen(nStackSize)
    {
        Clear();
    }

    CSmartStringT( const TCharType* pszSrc ) :m_pszData( NULL ), m_iReserveBufferLen( nStackSize )
    {
        Clear();
        CopyFromStr( pszSrc );
    }

    CSmartStringT(const CSmartStringT& strSrc ) :m_pszData( NULL ), m_iReserveBufferLen( nStackSize )
    {
        Clear();
        CopyFromStr( strSrc );
    }

    ~CSmartStringT()
    {
        if ( m_pszData != NULL )
        {
            free( m_pszData );
            m_pszData = NULL;
        }
    }

    //  ��ֵ��������
    CSmartStringT& operator = ( const TCharType* pszSrc )
    {
        if ( m_pszData != pszSrc )
        {
            CopyFromStr( pszSrc );
        }
        return *this;
    }
    CSmartStringT& operator = (const CSmartStringT& strSrc )
    {
        if ( this != &strSrc )
        {
            CopyFromStr( strSrc.GetStr() );
        }
        return *this;
    }


    //  �����ַ���
    //  ������ԣ�����С�ڶ�ջ�ռ䣬��ֻ�����ڶ�ջ������Ϊ�ѿռ�
    size_t CopyFromStr( const TCharType* pszSrc )
    {
        return _CopyFromStr( pszSrc );
    }

    //  ׷���ַ���
    CSmartStringT& operator += ( const TCharType* pszSrc )
    {
        if ( m_pszData != pszSrc )
        {
            AppendStr( pszSrc );
        }
        return *this;
    }
    CSmartStringT& operator += ( const CSmartStringT& strSrc )
    {
        if ( m_pszData != strSrc.GetStr() )
        {
            AppendStr( strSrc );
        }
        return *this;
    }

    //  �����ַ���
    size_t AppendStr( const TCharType* pszSrc )
    {
        return Insert( (int)m_iDataLen, pszSrc );
    }
    size_t AppendStr( const CSmartStringT& strSrc )
    {
        return Insert( (int)m_iDataLen, strSrc.GetStr() );
    }

    //  �ж��ַ��������Ƿ���ͬ
    BOOL operator == ( const TCharType* pszSrc )
    {
        if ( isEmpty() && pszSrc == NULL )
        {
            return true;
        }
        return ( _StrCmp( m_pszData, pszSrc ) == 0 );
    }
    BOOL operator == ( const CSmartStringT& strSrc )
    {
        if ( isEmpty() && strSrc.GetStr() == NULL )
        {
            return TRUE;
        }

        if ( IsUseStack() ^ strSrc.IsUseStack() )   //  ʹ�ö�ջ��ͬ
        {
            return FALSE;
        }
        
        
        return ( _StrCmp( GetStr(), strSrc.GetStr() ) == 0 );
    }

    //  ��ȡԭʼ�ַ���
    const TCharType* GetStr()
    {
        if ( IsUseStack() == TRUE )
        {
            return &m_stackBuffer[0];
        }
        else
        {
            return m_pszData;
        }
    }

    const TCharType* GetStr() const
    {
        if ( IsUseStack() == TRUE )
        {
            return &m_stackBuffer[0];
        }
        else
        {
            return m_pszData;
        }
    }

    //  ָ��λ�ô������ַ���
	size_t Insert(int index, const TCharType* pszSrc)
	{
        return _Insert( index, pszSrc );
	}

    //  �ͷŶ�����Դ�ռ�
	size_t ReleaseExtraMem()
	{
        return _ReleaseExtraMem( TCharType( 0 ) );
	}

    //  ��ʽ���ַ���
	void Format(const TCharType* pszFormat, ...)
	{
		va_list argList;
		va_start( argList, pszFormat );
		FormatV( pszFormat, argList );
		va_end( argList );
	}

    //  ��ȡ�ַ�������
    size_t GetStrLength()
    {
        return m_iDataLen;
    }

    //  �Ƿ�Ϊ��
    BOOL isEmpty()
    {
        return m_iDataLen == 0;
    }

    //  ��������(�������ͷŶ���ջ�ռ�,���ͷ������ReleaseExtraMem)
    void Clear()
    {
        Clear( TCharType( 0 ) );
    }

private:

    void Clear( char )
    {
        m_iDataLen = 0;
        m_stackBuffer[0] = '\0';
    }

    void Clear( wchar_t )
    {
        m_iDataLen = 0;
        m_stackBuffer[0] = L'\0';
    }

    //  �Ƿ�ʹ�ö�ջ�ռ�
    //  ��ֵĲ���Ϊ��ֻҪС��nStackSize����ʹ�ö�ջ�ռ䣬����Ϊ��
    BOOL IsUseStack()
    {
        return m_iDataLen < nStackSize;
    }
    BOOL IsUseStack() const
    {
        return m_iDataLen < nStackSize;
    }

    //  Ԥ�ÿռ�
    void _ReverseMem( size_t wantlen, char )
    {
        m_iReserveBufferLen = max( wantlen, min( wantlen * 2, m_iReserveBufferLen + t_iPageSize ) );
        m_pszData = (TCharType*)realloc( m_pszData, m_iReserveBufferLen );
    }

    void _ReverseMem( size_t wantlen, wchar_t )
    {
        m_iReserveBufferLen = max( wantlen, min( wantlen * 2, m_iReserveBufferLen + t_iPageSize ) );
        m_pszData = (TCharType*)realloc( m_pszData, m_iReserveBufferLen * 2);
    }

    size_t _ReleaseExtraMem( char )
    {
        if ( m_pszData == NULL )
        {
            return 0;
        }

        size_t iBufferLen = 0;
        if ( m_iDataLen < nStackSize )
        {
            iBufferLen = nStackSize * sizeof( TCharType );
        }
        else
        {
            iBufferLen = m_iDataLen * sizeof( TCharType );
        }

        size_t iReleaseLen = m_iReserveBufferLen - iBufferLen;
        if ( iReleaseLen != 0 )
        {
            m_iReserveBufferLen = iBufferLen;
            m_pszData = (TCharType*)realloc( m_pszData, m_iReserveBufferLen );

        }
        return iReleaseLen;
    }

    size_t _ReleaseExtraMem( wchar_t )
    {
        if ( m_pszData == NULL )
        {
            return 0;
        }

        size_t iBufferLen = 0;
        if (m_iDataLen < nStackSize)
        {
            iBufferLen = nStackSize * sizeof( TCharType );
        }
        else
        {
            iBufferLen = m_iDataLen * sizeof( TCharType );
        }
        
        size_t iReleaseLen = m_iReserveBufferLen - iBufferLen;
        if ( iReleaseLen != 0 )
        {
            m_iReserveBufferLen = iBufferLen;
            m_pszData = (TCharType*)realloc( m_pszData, 2 * m_iReserveBufferLen );

        }
        return iReleaseLen;
    }


    size_t _Insert( int index, const char* pszSrc )
    {
        if ( m_iDataLen == 0 )
        {
            return CopyFromStr( pszSrc );
        }

        size_t srclen = _StrLen( pszSrc );
        if ( srclen > 0 )
        {
            index = ( index < 0 ? 0 : ( index >( int )m_iDataLen ? (int)m_iDataLen : index ) );

            size_t wantlen = ( m_iDataLen + srclen );
            if ( wantlen > m_iReserveBufferLen )
            {
                _ReverseMem( wantlen, char(0) );
            }

            if ( wantlen < nStackSize )
            {
                memmove( &m_stackBuffer[0] + index + srclen, &m_stackBuffer[0] + index, ( m_iDataLen - index + 1 ) * sizeof( TCharType ) );
                memcpy( &m_stackBuffer[0] + index, pszSrc, srclen * sizeof( TCharType ) );
            }
            else
            {
                if ( IsUseStack() == TRUE )
                {
                    if ( index == 0)    //  ׷�����ݣ���ֱ�ӽ�ջ��������ȫ�������Ѽ���
                    {
                        memcpy( m_pszData, pszSrc, srclen );
                        memcpy( m_pszData + srclen, &m_stackBuffer[0], m_iDataLen );
                    }
                    else if (index == m_iDataLen)   //  ��ջ��������ȫ�������ѣ���׷������
                    {
                        memcpy( m_pszData, &m_stackBuffer[0], m_iDataLen );
                        memcpy( m_pszData + m_iDataLen, pszSrc, srclen );
                    }
                    else
                    {
                        //  �Ƚ�ջ�ϵ�ǰ�������ƶ�������
                        memcpy( m_pszData, &m_stackBuffer[0], index );
                        //  �ٽ����������׷�ӵ�����
                        memcpy( m_pszData + index, pszSrc, srclen );
                        //  ���ջ�Ϻ�����ݼ���׷�ӵ�����
                        memcpy( m_pszData + index + srclen, &m_stackBuffer[0] + index, ( m_iDataLen - index + 1 ) );
                    }
                }
                else
                {
                    memmove( m_pszData + index + srclen, m_pszData + index, ( m_iDataLen - index + 1 ) );
                    memcpy( m_pszData + index, pszSrc, srclen );
                }
            }

            m_iDataLen += srclen;

            if (m_iDataLen >= nStackSize)
            {
                m_pszData[m_iDataLen] = '\0';
            }
            else
            {
                m_stackBuffer[m_iDataLen] = '\0';
            }
            
        }
        return m_iDataLen;
    }

    size_t _Insert( int index, const wchar_t* pszSrc )
    {
        if ( m_iDataLen == 0 )
        {
            return CopyFromStr( pszSrc );
        }

        size_t srclen = _StrLen( pszSrc );
        if ( srclen > 0 )
        {
            index = ( index < 0 ? 0 : ( index >( int )m_iDataLen ? (int)m_iDataLen : index ) );

            size_t wantlen = ( m_iDataLen + srclen);
            if ( wantlen > m_iReserveBufferLen )
            {
                _ReverseMem( wantlen, wchar_t( 0 ) );
            }

            if ( wantlen < nStackSize )
            {
                wmemmove( &m_stackBuffer[0] + index + srclen, &m_stackBuffer[0] + index, ( m_iDataLen - index + 1 ) );
                wmemcpy( &m_stackBuffer[0] + index, pszSrc, srclen );
            }
            else
            {
                if ( IsUseStack() == TRUE )
                {
                    if ( index == 0 )    //  ׷�����ݣ���ֱ�ӽ�ջ��������ȫ�������Ѽ���
                    {
                        wmemcpy( m_pszData, pszSrc, srclen );
                        wmemcpy( m_pszData + srclen, &m_stackBuffer[0], m_iDataLen );
                    }
                    else if ( index == m_iDataLen )   //  ��ջ��������ȫ�������ѣ���׷������
                    {
                        wmemcpy( m_pszData, &m_stackBuffer[0], m_iDataLen );
                        wmemcpy( m_pszData + m_iDataLen, pszSrc, srclen );
                    }
                    else
                    {
                        //  �Ƚ�ջ�ϵ�ǰ�������ƶ�������
                        wmemcpy( m_pszData, &m_stackBuffer[0], index );
                        //  �ٽ����������׷�ӵ�����
                        wmemcpy( m_pszData + index, pszSrc, srclen );
                        //  ���ջ�Ϻ�����ݼ���׷�ӵ�����
                        wmemcpy( m_pszData + index + srclen, &m_stackBuffer[0] + index, ( m_iDataLen - index + 1 ) );
                    }
                }
                else
                {
                    wmemmove( m_pszData + index + srclen, m_pszData + index, ( m_iDataLen - index + 1 ) );
                    wmemcpy( m_pszData + index, pszSrc, srclen );
                }
            }

            m_iDataLen += srclen;

            if ( m_iDataLen >= nStackSize )
            {
                m_pszData[m_iDataLen] = L'\0';
            }
            else
            {
                m_stackBuffer[m_iDataLen] = L'\0';
            }

        }
        return m_iDataLen;
    }

    void FormatV( const TCharType* pszFormat, va_list args )
    {
        size_t len = _GetFormattedLength( pszFormat, args );
        if ( len > 0 )
        {
           if (len < nStackSize)
           {
               _VSprintf( &m_stackBuffer[0], pszFormat, args );
           }
           else
           {
               if (len > m_iReserveBufferLen)
               {
                   _ReverseMem( len, TCharType(0) );
               }
               _VSprintf( m_pszData, pszFormat, args );
           }
           m_iDataLen = len;
        }
        else
        {
            Clear();
        }
    }

    size_t _CopyFromStr( const char* pszSrc )
    {
        size_t srclen = _StrLen( pszSrc );
        if ( srclen > 0 )
        {
            if ( srclen < nStackSize )
            {
                memcpy( &m_stackBuffer[0], pszSrc, srclen );
                m_stackBuffer[srclen] = '\0';
            }
            else
            {
                if ( m_iReserveBufferLen < srclen )
                {
                    _ReverseMem( srclen, char(0) );
                }

                memcpy( m_pszData, pszSrc, srclen );
                m_pszData[srclen] = '\0';
            }

            m_iDataLen = srclen;
        }
        else
        {
            Clear();
        }

        return m_iDataLen;
    }


    size_t _CopyFromStr( const wchar_t* pszSrc )
    {
        size_t srclen = _StrLen( pszSrc );
        if ( srclen > 0 )
        {
            if ( srclen < nStackSize )
            {
                wmemcpy( &m_stackBuffer[0], pszSrc, srclen );
                m_stackBuffer[srclen] = L'\0';
            }
            else
            {
                if ( m_iReserveBufferLen < srclen )
                {
                    _ReverseMem( srclen, wchar_t(0) );
                }

                wmemcpy( m_pszData, pszSrc, srclen );
                m_pszData[srclen] = L'\0';
            }

            m_iDataLen = srclen;
        }
        else
        {
            Clear();
        }

        return m_iDataLen;
    }


	static size_t _StrLen(const char* psz)
	{
		return (psz != NULL ? strlen(psz) : 0);
	}

	static size_t _StrLen(const wchar_t* psz)
	{
		return (psz != NULL ? wcslen(psz) : 0);
	}

	static int _StrCmp(const char* psz1, const char* psz2)
	{
		return strcmp(psz1, psz2);
	}

	static int _StrCmp(const wchar_t* psz1, const wchar_t* psz2)
	{
		return wcscmp(psz1, psz2);
	}

	static int _GetFormattedLength(const char* pszFormat, va_list args)
	{
		return _vscprintf(pszFormat, args);
	}

	static int _GetFormattedLength(const wchar_t* pszFormat, va_list args)
	{
		return _vscwprintf(pszFormat, args);
	}

	int _VSprintf(char* pszBuffer, const char* pszFormat, va_list args)
	{
		//return vsprintf(pszBuffer, pszFormat, args);
        return vsprintf_s( pszBuffer, m_iReserveBufferLen, pszFormat, args );
	}

	int _VSprintf(wchar_t* pszBuffer, const wchar_t* pszFormat, va_list args)
	{
		//return vswprintf(pszBuffer, pszFormat, args);
        return vswprintf_s( pszBuffer, m_iReserveBufferLen, pszFormat, args );
	}

private:

	TCharType*	m_pszData;
	size_t		m_iDataLen;
    size_t		m_iReserveBufferLen;
    TCharType   m_stackBuffer[nStackSize + 1];
};

typedef CSmartStringT<char>			CSmartStringA;
typedef CSmartStringT<wchar_t>		CSmartStringW;

#ifdef _UNICODE
typedef CSmartStringW	CSmartString;
#else
typedef CSmartStringA	CSmartString;
#endif

#endif  //  _SMART_STRING_H_
