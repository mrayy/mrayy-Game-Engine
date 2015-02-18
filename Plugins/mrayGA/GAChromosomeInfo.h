

/********************************************************************
	created:	2010/05/10
	created:	10:5:2010   19:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGA\GAChromosomeInfo.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGA
	file base:	GAChromosomeInfo
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GAChromosomeInfo_h__
#define GAChromosomeInfo_h__


#include <vector>

namespace mray
{
namespace AI
{

	class GAChromosome;

class GAChromosomeInfo
{
public:
	GAChromosomeInfo():chrom(0),fitness(0)
	{
	}
	GAChromosomeInfo(GAChromosome*c):chrom(c),fitness(0)
	{}
	GAChromosome*chrom;
	float fitness;
};
typedef std::vector<GAChromosomeInfo> GAChromosomeInfoList;

}
}

#endif // GAChromosomeInfo_h__
