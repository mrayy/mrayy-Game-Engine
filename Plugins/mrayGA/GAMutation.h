

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   19:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAMutation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAMutation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAMutation_h__
#define GAMutation_h__

#include "CompileConfig.h"
#include "GAChromOperation.h"

namespace mray
{
namespace AI
{

class MRAYGA_API GAFlipMutation:public GAMutationOperation
{
private:
protected:
	float m_chance;
public:
	GAFlipMutation(float chance);
	virtual ~GAFlipMutation();

	virtual void Apply(GAChromosome* c);
	
};

}
}

#endif // GAMutation_h__
