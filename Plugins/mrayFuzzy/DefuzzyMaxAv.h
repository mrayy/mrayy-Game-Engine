
/********************************************************************
	created:	2010/03/16
	created:	16:3:2010   16:36
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayFuzzy\DefuzzyMaxAv.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayFuzzy
	file base:	DefuzzyMaxAv
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DefuzzyMaxAv___
#define ___DefuzzyMaxAv___

#include "IDefuzzifyOperation.h"

namespace mray{
namespace AI{

class DefuzzyMaxAv:public IDefuzzifyOperation
{
private:
protected:
public:
	DefuzzyMaxAv();
	virtual~DefuzzyMaxAv();

	virtual float Defuzzify(FuzzyVariable*var);
};

}
}


#endif //___DefuzzyMaxAv___
