

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   16:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAChromosome.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAChromosome
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAChromosome_h__
#define GAChromosome_h__

#include <mtypes.h>

namespace mray
{
namespace AI
{
	class GAAlgorithmParameters;

class GACodeBuffer
{
	char* m_buffer;
	uint m_size;
public:
	GACodeBuffer():m_buffer(0),m_size(0)
	{
	}
	GACodeBuffer(uint sz):m_buffer(new char[sz]),m_size(sz)
	{
	}
	virtual~GACodeBuffer()
	{
		delete [] m_buffer;
	}

	void* GetBuffer()
	{
		return m_buffer;
	}
	uint GetSize(){
		return m_size;
	}

	void Resize(uint sz)
	{
		if(m_size==sz)
			return;
		delete [] m_buffer;
		m_size=sz;
		m_buffer=new char[m_size];
	}
};

class GAChromosome
{
private:
protected:
	uint m_ID;
public:
	GAChromosome():m_ID(0)
	{}
	virtual ~GAChromosome()
	{}


	void SetID(uint id){m_ID=id;}
	uint GetID(){return m_ID;}

	virtual uint GetCodeLength()=0;

	virtual void CreateBuffer(GACodeBuffer*buff)=0;
	virtual void FillBuffer(GACodeBuffer* buff)=0;
	virtual void FillBuffer(GACodeBuffer* buff,int start,int len)=0;
	virtual void FromBuffer(GACodeBuffer* buff)=0;

	virtual void Flip(int start,int len)=0;

	virtual GAChromosome* CreateCopy()=0;

	
};

}
}

#endif // GAChromosome_h__
