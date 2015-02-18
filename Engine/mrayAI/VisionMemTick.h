
/********************************************************************
	created:	2010/03/19
	created:	19:3:2010   22:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\VisionMemTick.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	VisionMemTick
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___VisionMemTick___
#define ___VisionMemTick___

#include "Point3d.h"
#include "mtypes.h"

namespace mray{
namespace AI{

class VisionMemTick
{
private:
protected:
public:
	VisionMemTick():time(0),isVisible(0)
	{
	}
	VisionMemTick(const VisionMemTick&o):time(o.time),isVisible(o.isVisible)
		,position(o.position),direction(o.direction)
	{
	}
	VisionMemTick(double t,const math::vector3d&pos,const math::vector3d&dir,bool v):time(t),isVisible(v)
		,position(pos),direction(dir)
	{
	}
	virtual~VisionMemTick(){}

	double time;	//last time been saw

	math::vector3d position;	//last position

	math::vector3d direction;	//last direction with speed

	bool isVisible;		// is it still visible?
};

}
}


#endif //___VisionMemTick___
