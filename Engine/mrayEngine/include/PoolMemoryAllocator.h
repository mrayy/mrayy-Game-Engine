


/********************************************************************
	created:	2010/12/17
	created:	17:12:2010   16:52
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PoolMemoryAllocator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PoolMemoryAllocator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef PoolMemoryAllocator_h__
#define PoolMemoryAllocator_h__

#include "mTypes.h"
#include "CompileConfig.h"


namespace mray
{

class MRAY_DLL PoolMemory
{
private:
protected:
	typedef uchar Byte;
	struct Block;
	struct CashNode
	{
		CashNode():prev(0),next(0),block(0)
		{
		}
		CashNode*prev;
		CashNode*next;
		Block* block;
	};
	struct Block
	{
		Block* _prev;
		Block* _next;
	//	Byte _free;
		uint _size;
		ushort _PoolID;
		//ushort _freeCashID;
		//CashNode* cash;
		CashNode* cashIt;

		Block(Block*prev,Block*next,uint sz,ushort id):_prev(prev),_next(next),_size(sz),/*_free(1),*/_PoolID(id),cashIt(0)
		{
		}
		~Block(){}
	};

	struct Pool
	{
		Pool(Byte*d,ushort id):data(d),ID(id)
		{
		}
		Byte*data;
		ushort ID;
	};
	std::list<Pool> m_memoryPool;
	Block* m_head;
	uint m_poolSize;
	ushort m_poolID;
	//std::vector<Block*> m_freeCash;
	
	class CashList
	{
		CashNode*m_head;
		CashNode*m_graveYard;
		int m_cnt;
		int m_newCalls;
	public:
		CashList();
		virtual~CashList();

		CashNode* Insert(Block*b);
		void Remove(CashNode*node);
		CashNode* begin();
		CashNode* end();
	};
	CashList m_freeCash;

	PoolMemory(const PoolMemory&o);
	PoolMemory&operator=(PoolMemory&o);


	Block* _Grow(Block*prev);

public:
	PoolMemory(uint poolSize=0xFFFF);
	virtual ~PoolMemory();

	void* Allocate(uint sz);
	
	void Deallocate(void*data,uint=0);

	uint GetPoolSize()const{return m_poolSize;}

};




//////////////////////////////////////////////////////////////////////////


template <typename T> class PoolMemoryAllocator;

template <> class PoolMemoryAllocator<void>
{
public:
	typedef void* pointer;
	typedef const void* const_pointer;
	// reference to void members are impossible.
	typedef void value_type;
	template <class U> 
	struct rebind { typedef PoolMemoryAllocator<U> other; };
};    

namespace pool_alloc{
	inline void destruct(char *){}
	inline void destruct(wchar_t*){}
	template <typename T> 
	inline void destruct(T *t){t->~T();}
} // namespace

template <typename T>
class PoolMemoryAllocator
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
	struct rebind { typedef PoolMemoryAllocator<U> other; };
	PoolMemoryAllocator(uint poolSize=0xFFFFF):mem_(poolSize)
	{}
	PoolMemoryAllocator(const PoolMemoryAllocator&o):mem_(o.mem_.GetPoolSize())
	{}
	pointer address(reference x) const {retuen &x;}
	const_pointer address(const_reference x) const {return &x;}
	pointer allocate(size_type size, PoolMemoryAllocator<void>::const_pointer hint = 0)
	{
		return static_cast<pointer>(mem_.Allocate(size*sizeof(T)));
	}
	//for Dinkumware:
	char *_Charalloc(size_type n){return static_cast<char*>(mem_.Allocate(n));}
	// end Dinkumware

	template <class U> PoolMemoryAllocator(const PoolMemoryAllocator<U>&){}
	void deallocate(pointer p, size_type n= 0)
	{
		mem_.Deallocate(p, n);
	}
	void deallocate(void *p, size_type n= 0)
	{
		mem_.Deallocate(p, n);
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
	void destroy(pointer p){pool_alloc::destruct(p);}
private:

	PoolMemory mem_;
};

template <typename T, typename U>
inline bool operator==(const PoolMemoryAllocator<T>&, const PoolMemoryAllocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const PoolMemoryAllocator<T>&, const PoolMemoryAllocator<U>){return false;}


// For VC6/STLPort 4-5-3 see /stl/_alloc.h, line 464
// "If custom allocators are being used without member template classes support :
// user (on purpose) is forced to define rebind/get operations !!!"
#ifdef _WIN32
#define POOL_ALLOC_CDECL __cdecl
#else
#define POOL_ALLOC_CDECL
#endif


}
namespace std{
	template <class _Tp1, class _Tp2>
	inline mray::PoolMemoryAllocator<_Tp2>& POOL_ALLOC_CDECL
		__stl_alloc_rebind(mray::PoolMemoryAllocator<_Tp1>& __a, const _Tp2*) 
	{  
		return (mray::PoolMemoryAllocator<_Tp2>&)(__a); 
	}


	template <class _Tp1, class _Tp2>
	inline mray::PoolMemoryAllocator<_Tp2> POOL_ALLOC_CDECL
		__stl_alloc_create(const mray::PoolMemoryAllocator<_Tp1>&, const _Tp2*) 
	{ 
		return mray::PoolMemoryAllocator<_Tp2>(); 
	}

}

#endif // PoolMemoryAllocator_h__
