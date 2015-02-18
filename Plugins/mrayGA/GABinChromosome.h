

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   17:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GABinChromosome.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GABinChromosome
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GABinChromosome_h__
#define GABinChromosome_h__

#include "CompileConfig.h"
#include "GAChromosome.h"

namespace mray
{
namespace AI
{

class MRAYGA_API GABinChromosome:public GAChromosome
{
private:
protected:
	bool* m_bits;
	uint m_length;
public:
	GABinChromosome();
	GABinChromosome(const bool* bits,uint len);
	virtual ~GABinChromosome();

	virtual uint GetCodeLength();

	virtual void CreateBuffer(GACodeBuffer*buff);
	virtual void FillBuffer(GACodeBuffer* buff);
	virtual void FillBuffer(GACodeBuffer* buff,int start,int len);
	virtual void FromBuffer(GACodeBuffer* buff);

	virtual void Flip(int start,int len);

	void SetBit(uint b,bool v);
	bool GetBit(uint b);

	virtual GAChromosome* CreateCopy();

};

}
}

#endif // GABinChromosome_h__
