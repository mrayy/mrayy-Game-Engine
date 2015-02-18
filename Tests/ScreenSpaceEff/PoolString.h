

/********************************************************************
	created:	2011/02/23
	created:	23:2:2011   23:50
	filename: 	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff\PoolString.h
	file path:	i:\Programing\GameEngine\mrayEngine\testFrameWork\ScreenSpaceEff
	file base:	PoolString
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PoolString_h__
#define PoolString_h__

#include "PoolMemoryAllocator.h"

namespace mray
{

	template<class T >
class PoolString
{
private:
protected:

	static PoolMemoryAllocator<T> g_allocator;
	
	T* array;
	int allocated;
public:
	//! Default constructor
	PoolString()
		: array(0), allocated(1)
	{
		array = g_allocator.allocate(allocated);
		array[0] = 0x0;
	}

	PoolString(const T* c)
		: array(0), allocated(0)
	{
		*this = c;
	}


	//! Constructor
	PoolString(const PoolString<T>& other)
		: array(0), allocated(0)
	{
		*this = other;
	}
	~PoolString()
	{
		g_allocator.deallocate(array,0);
	}
	//! Assignment operator
	PoolString<T>& operator=(const PoolString<T>& other)
	{
		if (this == &other)
			return *this;

		g_allocator.deallocate(array,0);
		allocated = other.allocated;
		array = g_allocator.allocate(allocated);

		const T* p = other.array;
		for (int i=0; i<allocated; ++i, ++p)
			array[i] = *p;

		return *this;
	}
	PoolString<T>& operator=(const T* c)
	{
		if (!c)
		{
			return *this;
		}

		if (c == array)
			return *this;

		int len = 1;
		const T* p = c;
		while(*p)
		{
			++len;
			++p;
		}

		g_allocator.deallocate(array,0);
		allocated = len;
		array = g_allocator.allocate(allocated);

		for (int l = 0; l<len; ++l)
			array[l] = c[l];

		return *this;
	}

};
template<class T>
PoolMemoryAllocator<T> PoolString<T>::g_allocator;

}
#endif // PoolString_h__
