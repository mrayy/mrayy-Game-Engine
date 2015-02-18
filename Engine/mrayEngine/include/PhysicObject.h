
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   17:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\PhysicObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	PhysicObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___PhysicObject___
#define ___PhysicObject___

#include "IPhysicObject.h"

namespace mray{
namespace physics{

class MRAY_DLL PhysicObject:public IPhysicObject
{
protected:
	std::vector<math::vector3d> *m_points;
public:
	PhysicObject(std::vector<math::vector3d> *points);
	virtual~PhysicObject();

	void setPoints(std::vector<math::vector3d> *points);
	math::vector3d*getPoints();

	virtual void updateVertices(float dt);
};
	
}
}

#endif //___PhysicObject___

