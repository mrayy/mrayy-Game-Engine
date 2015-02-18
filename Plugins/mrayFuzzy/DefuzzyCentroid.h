

/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   19:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\DefuzzyCentroid.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	DefuzzyCentroid
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DefuzzyCentroid___
#define ___DefuzzyCentroid___

#include "IDefuzzifyOperation.h"


namespace mray{
namespace AI{

class DefuzzyCentroid:public IDefuzzifyOperation
{
private:
protected:
	int m_numSamples;
public:
	DefuzzyCentroid(int numSamples);
	virtual~DefuzzyCentroid();

	virtual float Defuzzify(FuzzyVariable*var);

};

}
}

#endif //___DefuzzyCentroid___
