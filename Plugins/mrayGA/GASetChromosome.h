

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   21:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GASetChromosome.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GASetChromosome
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	chromosome vector holds indicies of an actual data set
*********************************************************************/

#ifndef GASetChromosome_h__
#define GASetChromosome_h__

#include "CompileConfig.h"
#include "GAValuedChrom.h"

namespace mray
{
namespace AI
{

class MRAYGA_API GASetChromosome:public GAValuedChrom<uint>
{
private:
protected:
	uint *m_indicies;
	uint m_length;
	uint m_maxIndex;
public:
	GASetChromosome(uint *indicies,int length,int maxIndex,GAAlgorithmParameters* params);
	GASetChromosome(int length,int maxIndex,GAAlgorithmParameters* params);
	virtual ~GASetChromosome();

	virtual uint GetCodeLength();

	virtual void CreateBuffer(GACodeBuffer*buff);
	virtual void FillBuffer(GACodeBuffer* buff);
	virtual void FillBuffer(GACodeBuffer* buff,int start,int len);
	virtual void FromBuffer(GACodeBuffer* buff);

	virtual void Flip(int start,int len);

	void SetAt(uint b,uint v);
	uint GetAt(uint b);

	virtual GAChromosome* CreateCopy();
};

}
}


#endif // GASetChromosome_h__

