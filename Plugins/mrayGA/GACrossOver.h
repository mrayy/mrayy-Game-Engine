

/********************************************************************
	created:	2010/05/09
	created:	9:5:2010   17:21
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GACrossOver.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GACrossOver
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GACrossOver_h__
#define GACrossOver_h__

#include "CompileConfig.h"
#include "GAChromOperation.h"

namespace mray
{
namespace AI
{
	class GACodeBuffer;

class MRAYGA_API GAMultiPointCrossover:public GACrossoverOperation
{
private:
protected:
	GACodeBuffer *m_buffer;
	int m_numberOfPoints;
	int* m_points;
public:
	GAMultiPointCrossover(int numberOfPoints);
	virtual ~GAMultiPointCrossover();

	void SetNumberOfPoints(int c);

	virtual void Apply(GAChromosome* c1,GAChromosome* c2,GAChromosome*&result);
};

}
}
#endif // GACrossOver_h__
