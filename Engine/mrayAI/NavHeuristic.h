
/********************************************************************
	created:	2010/03/10
	created:	10:3:2010   15:28
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\NavHeuristic.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	NavHeuristic
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavHeuristic___
#define ___NavHeuristic___

#include "ICostFunction.h"

namespace mray{
namespace AI{

class MRAY_AI_DLL NavHeuristic:public ICostFunction
{
private:
protected:
public:
	NavHeuristic();
	virtual~NavHeuristic();

};

}
}


#endif //___NavHeuristic___
