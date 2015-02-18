#ifndef ___BITVECTOR___
#define ___BITVECTOR___

#include "CompileConfig.h"
#include "mraySystem.h"

namespace mray{
namespace core{

class BitVector
{
protected:
	unsigned long int *m_array;
	int m_size;
	int m_bits;
public:
	BitVector():m_array(0),m_size(0),m_bits(0)
	{
	}
	BitVector(int numOfBits):m_array(0),m_size(0),m_bits(0)
	{
		resize(numOfBits);
	}
	BitVector(const BitVector&o):m_array(0),m_size(0),m_bits(0){
		*this=o;
	}
	BitVector& operator = (const BitVector&o)
	{
		resize(o.getBitsUsed());
		mraySystem::memCopy(m_array,o.m_array,m_size*sizeof(unsigned long int));
		return *this;
	}
	BitVector& operator |= (const BitVector&o)
	{
		for (int i=0;i<m_size;++i)
		{
			m_array[i]|=o.m_array[i];
		}
		return *this;
	}
	BitVector& operator &= (const BitVector&o)
	{
		for (int i=0;i<m_size;++i)
		{
			m_array[i]&=o.m_array[i];
		}
		return *this;
	}
	BitVector operator !()const
	{
		return Not();
	}
	BitVector operator | (const BitVector&o)const
	{
		return Or(o);
	}
	BitVector operator & (const BitVector&o)const
	{
		return And(o);
	}
	virtual~BitVector(){
		if(m_array){
			delete [] m_array;
			m_array=0;
		}
	}

	int getBitsUsed()const{
		return m_bits;
	}
	int getSlotsUsed()const
	{
		return m_size;
	}

	void resize(int numBits){
		unsigned long int *newVec;
		int numSlots;
		if(numBits % 32 ==0){
			numSlots=numBits/32;
		}else{
			numSlots=(numBits/32) +1;
		}
		newVec=new unsigned long int[numSlots];
		if(!newVec)return;

		int min_sz;
		if(numSlots<m_size)
			min_sz=numSlots;
		else 
			min_sz=m_size;
		mraySystem::memCopy(newVec,m_array,min_sz*sizeof(unsigned long int));

		for(int i=m_size;i<numSlots;++i)
		{
			newVec[i]=0;
		}
		m_size=numSlots;
		m_bits=numBits;
		if(m_array)
			delete [] m_array;
		m_array=newVec;
	}
	bool operator[](int index)const{

		int cell=index / 32;
		if(cell>=m_size)return false;

		int bit = index % 32;
		return (m_array[cell] & (1 << bit ))>>bit;
	}

	void set(int index,bool v){
		int cell=index / 32;
		if(cell>=m_size)return;

		int bit = index % 32;
		if(v){
			m_array[cell] = m_array[cell] | (1<<bit);
		}else
			m_array[cell] = m_array[cell] & (~(1<<bit));
	}

	void clear(bool v=false){
		unsigned long int vs;
		if(v)vs=~0;
		else vs=0;
		for(int i=0;i<m_size;++i)
			m_array[i]=vs;
	}

	//apply (and) operator
	BitVector And(const BitVector&o)const
	{
		BitVector v;
		if(getBitsUsed()!=o.getBitsUsed())
		{
			return *this;
		}
		v.resize(getBitsUsed());
		for (int i=0;i<m_size;++i)
		{
			v.m_array[i]=m_array[i]&o.m_array[i];
		}
		return v;
	}
	//apply (or) operator
	BitVector Or(const BitVector&o)const
	{
		BitVector v;
		if(getBitsUsed()!=o.getBitsUsed())
		{
			return *this;
		}
		v.resize(getBitsUsed());
		for (int i=0;i<m_size;++i)
		{
			v.m_array[i]=m_array[i]|o.m_array[i];
		}
		return v;
	}
	//apply (not) operator
	BitVector Not()const
	{
		BitVector v;
		v.resize(getBitsUsed());
		for (int i=0;i<m_size;++i)
		{
			v.m_array[i]=~m_array[i];
		}
		return v;
	}
};

}
}


#endif


