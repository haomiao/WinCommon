///////////////////////////////////////////////////////////////////// 
///     COPYRIGHT NOTICE  
///     Copyright (c) 2015   
///     All rights reserved.  
///  
/// 	@file   handle_helper.h
/// 	@brief	句柄封装. 
/// 
///		主要实现基本句柄封装、帮助工具  
/// 
/// 	@version 		  0.0.1   
/// 	@author           zhouhao
/// 	@date             2015/12/05  14:36:02
///  
///  
/////////////////////////////////////////////////////////////////////

#ifndef     _HANDLE_HELPER_H_
#define     _HANDLE_HELPER_H_

#include <windows.h>
#include "config.h"
#include "type_helper.h"


// Used so we always remember to close the handle. Example:
//   ScopedHandle hfile(CreateFile(...));
//   if (!hfile.Get())
//     ...process error
//   ReadFile(hfile.Get(), ...);
//
// To sqirrel the handle away somewhere else:
//   secret_handle_ = hfile.Take();
//
// To explicitly close the handle:
//   CloseHandle(hfile.Take());
class ScopedHandle
{
public:

	ScopedHandle() : handle_(NULL)
    {

	}

	explicit ScopedHandle(HANDLE h) : handle_(NULL) 
    {
		Set(h);
	}

	~ScopedHandle() 
    {
		Close();
	}

	// Use this instead of comparing to INVALID_HANDLE_VALUE to pick up our NULL
	// usage for errors.
	bool IsValid() const
    {
		return handle_ != NULL;
	}

	void Set(HANDLE new_handle) 
    {
		Close();

		// Windows is inconsistent about invalid handles, so we always use NULL
        if (new_handle != INVALID_HANDLE_VALUE)
        {
            handle_ = new_handle;
        }
			
	}

	HANDLE Get() 
    {
		return handle_;
	}

	operator HANDLE() 
    { 
        return handle_; 
    }

	HANDLE Take() 
    {
		// transfers ownership away from this object
		HANDLE h = handle_;
		handle_ = NULL;
		return h;
	}

private:

	void Close() 
    {
		if (handle_) 
        {
			if (!::CloseHandle(handle_))
            { 
				ASSERT(FALSE);
			}
			handle_ = NULL;
		}
	}

private:

	HANDLE          handle_;
    DISALLOW_COPY_AND_ASSIGN(ScopedHandle);
};



// Like ScopedHandle, but for HANDLEs returned from FindFile().
class ScopedFindFileHandle 
{
public:

	explicit ScopedFindFileHandle(HANDLE handle) : handle_(handle)
    {
		// Windows is inconsistent about invalid handles, so we always use NULL
        if (handle_ == INVALID_HANDLE_VALUE)
        {
            handle_ = NULL;
        }
    }

    ~ScopedFindFileHandle()
    {
        if (handle_)
        {
            FindClose(handle_);
        }
    }

	// Use this instead of comparing to INVALID_HANDLE_VALUE to pick up our NULL
	// usage for errors.
	bool IsValid() const 
    { 
        return handle_ != NULL; 
    }

	operator HANDLE() 
    { 
        return handle_;
    }

private:

	HANDLE          handle_;
    DISALLOW_COPY_AND_ASSIGN(ScopedFindFileHandle);
};



// Like ScopedHandle but for HDC.  Only use this on HDCs returned from
// CreateCompatibleDC.  For an HDC returned by GetDC, use ReleaseDC instead.
class ScopedHDC 
{
public:

	explicit ScopedHDC(HDC h) : hdc_(h)
    { 

    }

    ~ScopedHDC()
    {
        if (hdc_)
        {
            DeleteDC(hdc_);
        }
    }

	operator HDC()
    { 
        return hdc_;
    }

private:

	HDC         hdc_;
    DISALLOW_COPY_AND_ASSIGN(ScopedHDC);
};



// Like ScopedHandle but for HBITMAP.
class ScopedBitmap
{
public:

	explicit ScopedBitmap(HBITMAP h) : hbitmap_(h)
    { 

    }

    ~ScopedBitmap()
    {
        if (hbitmap_)
        {
            DeleteObject(hbitmap_);
        }
    }

	operator HBITMAP()
    { 
        return hbitmap_; 
    }

private:

	HBITMAP         hbitmap_;
    DISALLOW_COPY_AND_ASSIGN(ScopedBitmap);
};



// Like ScopedHandle but for HRGN.
class ScopedHRGN 
{
public:

	explicit ScopedHRGN(HRGN h) : hrgn_(h)
    {

    }

    ~ScopedHRGN()
    {
        if (hrgn_)
        {
            DeleteObject(hrgn_);
        }
    }

	operator HRGN() 
    { 
        return hrgn_;
    }

	ScopedHRGN& operator=(HRGN hrgn) 
    {
        if (hrgn_ && hrgn != hrgn_)
        {
            DeleteObject(hrgn_);
        }
        hrgn_ = hrgn;
		return *this;
	}

private:

	HRGN            hrgn_;
    DISALLOW_COPY_AND_ASSIGN(ScopedHRGN);
};



// Like ScopedHandle except for HGLOBAL.
template<class T>
class ScopedHGlobal 
{
public:

	explicit ScopedHGlobal(HGLOBAL glob) : glob_(glob)
    {
		data_ = static_cast<T*>(GlobalLock(glob_));
	}

	~ScopedHGlobal() 
    {
		GlobalUnlock(glob_);
	}

	T* get() 
    { 
        return data_;
    }

	size_t Size() const 
    { 
        return GlobalSize(glob_); 
    }

	T* operator->() const
    {
		assert(data_ != 0);
		return data_;
	}

private:

	HGLOBAL         glob_;
	T               *data_;
    DISALLOW_COPY_AND_ASSIGN(ScopedHGlobal);
};


#endif  //   _HANDLE_HELPER_H_