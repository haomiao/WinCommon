#pragma once

namespace hk
{
	template <typename T>
	void safe_delete_ptr(T * &p)
	{
		if (p)
		{
			delete p;
			p = NULL;
		}
	}

	template <typename T>
	void safe_delete_array_ptr(T * &p)
	{
		if (p)
		{
			delete  []p;
			p = NULL;
		}
	}
}			
