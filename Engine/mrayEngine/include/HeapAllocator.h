
#ifndef __HeapAllocator__
#define __HeapAllocator__
 
#include "mTypes.h"
#include "CompileConfig.h"
#include <new>
//#include "CMemory.h"


namespace mray{


template <typename T>
class HeapAllocator
{
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	template <class U> 
	struct rebind { typedef HeapAllocator<U> other; };
	HeapAllocator()
	{}
	pointer address(reference x) const {retuen &x;}
	const_pointer address(const_reference x) const {return &x;}
	pointer allocate(size_type size, const void* hint = 0)
	{
		return static_cast<pointer>(malloc(size*sizeof(T)));
	}
	//for Dinkumware:
	char *_Charalloc(size_type n){return static_cast<char*>(malloc(n));}
	// end Dinkumware

	template <class U> HeapAllocator(const HeapAllocator<U>&){}
	void deallocate(pointer p, size_type n= 0)
	{
		free(p);
	}
	void deallocate(void *p, size_type n= 0)
	{
		free(p);
	}
	size_type max_size() const throw() {return size_t(-1) / sizeof(value_type);}
	void construct(pointer p, const T& val)
	{
		new(static_cast<void*>(p)) T(val);
	}
	void construct(pointer p)
	{
		new(static_cast<void*>(p)) T();
	}
	void destroy(pointer p){free(p);}
private:
};


};//mray

#endif

