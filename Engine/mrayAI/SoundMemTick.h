
/********************************************************************
	created:	2010/03/19
	created:	19:3:2010   22:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\SoundMemTick.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	SoundMemTick
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SoundMemTick___
#define ___SoundMemTick___


#include "Point3d.h"

namespace mray{
namespace AI{

class SoundMemTick
{
private:
protected:
public:
	SoundMemTick():time(0)
	{
	}
	SoundMemTick(const SoundMemTick&o):time(o.time),position(o.position)
	{
	}
	virtual~SoundMemTick(){}

	uint time;	//last time been heared

	math::vector3d position	;	//what place the sound been made
	
};

}
}


#endif //___SoundMemTick___
