#include "stdafx.h"

#include "PoolMemoryAllocator.h"

#include <assert.h>

namespace mray
{

PoolMemory::CashList::CashList():m_cnt(0),m_newCalls(0)
{
	m_head=0;
	m_graveYard=0;
}
PoolMemory::CashList::~CashList()
{
	while(m_head)
	{
		CashNode*next=m_head->next;
		delete m_head;
		m_head=next;
	}
	while(m_graveYard)
	{
		CashNode*next=m_graveYard->next;
		delete m_graveYard;
		m_graveYard=next;
	}
}

PoolMemory::CashNode* PoolMemory::CashList::Insert(PoolMemory::Block*b)
{
	++m_cnt;
	CashNode*node;
	if(m_graveYard){
		node=m_graveYard;
		m_graveYard=m_graveYard->next;
	}else{
		++m_newCalls;
		node=new CashNode();
	}
	node->block=b;
	node->next=m_head;
	node->prev=0;
	if(m_head)
		m_head->prev=node;
	m_head=node;
	return node;
}
void PoolMemory::CashList::Remove(PoolMemory::CashNode*node)
{
	--m_cnt;
	if(node->prev)
	{
		node->prev->next=node->next;
	}
	if(node->next)
	{
		node->next->prev=node->prev;
	}
	if(node==m_head)
		m_head=node->next;
	node->next=m_graveYard;
	node->prev=0;
	m_graveYard=node;

}
PoolMemory::CashNode* PoolMemory::CashList::begin()
{
	return m_head;
}
PoolMemory::CashNode* PoolMemory::CashList::end()
{
	return 0;
}

PoolMemory::PoolMemory(const PoolMemory&o)
{
}

PoolMemory&PoolMemory::operator=(PoolMemory&o)
{
	return*this;
}


PoolMemory::Block* PoolMemory::_Grow(PoolMemory::Block*prev)
{
	Block*b;
	Byte*ptr=new Byte[m_poolSize];
	m_poolID++;
	m_memoryPool.push_back(Pool(ptr,m_poolID));
	b=reinterpret_cast<Block*>(ptr);
	b->_prev=prev;
	b->_next=0;
	//b->_free=true;
	b->_size=m_poolSize-sizeof(Block);
	b->_PoolID=m_poolID;
	if(prev)
		prev->_next=b;
	b->cashIt= m_freeCash.Insert(b);
	assert(b->cashIt!=0);
	return b;
}

PoolMemory::PoolMemory(uint poolSize):m_poolSize(poolSize),m_poolID(0)
{
	if(m_poolSize-sizeof(Block) < 0xFF) 
	{
		m_poolSize=sizeof(Block)+0xFF;
	}
	m_head=_Grow(0);
}
PoolMemory::~PoolMemory()
{
	std::list<Pool>::iterator it= m_memoryPool.begin();
	for(;it!=m_memoryPool.end();++it)
		delete it->data;
}

void* PoolMemory::Allocate(uint sz)
{
	if(sz>m_poolSize-sizeof(Block))
		throw std::exception("Can't allocate more than the pool size!");
	/*
	Block*b=m_head;
	while(true){
	while(!b->_free )
	{
	if(!b->_next)
	b=_Grow(b);
	else b=b->_next;
	}
	if(b->_size<sz)
	b=b->_next;
	else break;
	}*/
	Block*b=0;

	CashNode* it=m_freeCash.begin();
	for(;it;it=it->next)
	{
		Block*t=it->block;
		if(t){
			b=t;
			if(b->_size>=sz)
				break;
		}
	}
	if(!b || b->_size<sz)
		b=_Grow(b);
	if(b->_size-sz>2*sizeof(Block))
	{
		Block*b2=(reinterpret_cast<Block*>(reinterpret_cast<Byte*>(b)+sizeof(Block)+sz));
		//b2->_free=true;
		b2->cashIt=b->cashIt;
		b->cashIt->block=b2;
		b2->_next=b->_next;
		b2->_prev=b;
		b2->_size=b->_size-(sizeof(Block)+sz);
		b2->_PoolID=b->_PoolID;

		if(b->_next)b->_next->_prev=b2;
		b->_size=sz;
		b->_next=b2;
	}else {
		assert(b->cashIt!=0);
		m_freeCash.Remove(b->cashIt);
	}
	b->cashIt=0;
	//		b->_free=0;
	return reinterpret_cast<Byte*>(b)+sizeof(Block);
}

void PoolMemory::Deallocate(void*data,uint)
{
	if(!data)
		return;
	Block*b=reinterpret_cast<Block*>(static_cast<Byte*>(data)-sizeof(Block));
	Block*prev=b->_prev;
	Block*next=b->_next;
	if(prev && next )
	{
		if(prev->cashIt!=0 && next->cashIt!=0 
			&& b->_PoolID==prev->_PoolID && b->_PoolID==next->_PoolID )
		{
			m_freeCash.Remove(next->cashIt);
			prev->_size+=b->_size+next->_size+2*sizeof(Block);
			prev->_next=next->_next;
			if(next->_next)
				next->_next->_prev=prev;
			return;
		}
	}
	if(prev)
	{
		if(prev->cashIt!=0 && b->_PoolID==prev->_PoolID)
		{
			prev->_size+=b->_size+sizeof(Block);
			prev->_next=b->_next;
			if(next)
				next->_prev=prev;
			return;
		}
	}
	if(next)
	{
		if(next->cashIt!=0 && b->_PoolID==next->_PoolID)
		{
			b->_size+=next->_size+sizeof(Block);
			next->cashIt->block=b;
			b->cashIt=next->cashIt;
			assert(b->cashIt!=0);
			b->_next=next->_next;
			if(next->_next)
				next->_next->_prev=b;
			return;
		}
	}
	//b->_free=1;
	b->cashIt= m_freeCash.Insert(b);
	assert(b->cashIt!=0);
}


}
