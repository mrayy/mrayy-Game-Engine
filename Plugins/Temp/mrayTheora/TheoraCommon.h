
/********************************************************************
	created:	2009/05/17
	created:	17:5:2009   22:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayTheora\TheoraCommon.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayTheora
	file base:	TheoraCommon
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___TheoraCommon___
#define ___TheoraCommon___

#include <mTypes.h>

namespace mray{
namespace video{

class TheoraCommon
{
private:
protected:
	void initalizeTables();
public:
	TheoraCommon();
	virtual~TheoraCommon();

	static TheoraCommon instance;

public:
	uint YTable [256];
	uint BUTable[256];
	uint GUTable[256];
	uint GVTable[256];
	uint RVTable[256];

};

}
}

#endif //___TheoraCommon___
