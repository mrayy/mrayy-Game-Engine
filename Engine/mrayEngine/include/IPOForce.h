
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   22:45
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IPOForce.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IPOForce
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IPOForce___
#define ___IPOForce___

#include "IPhysicObjectAffector.h"
#include "IPhysicObject.h"
#include <point3d.h>

namespace mray{
namespace physics{

class MRAY_DLL IPOForce:public IPhysicObjectAffector
{
	math::vector3d m_force;
public:

	IPOForce(math::vector3d force);
	virtual~IPOForce();

	virtual void affect(IPhysicObject*o,float dt);
};

}
}

#endif //___IPOForce___


