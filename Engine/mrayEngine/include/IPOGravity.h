
/********************************************************************
	created:	2008/10/26
	created:	26:10:2008   20:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IPOGravity.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IPOGravity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IPOGravity___
#define ___IPOGravity___


#include "IPhysicObjectAffector.h"
#include "IPhysicObject.h"
#include <point3d.h>

namespace mray{
namespace physics{

class MRAY_DLL IPOGravity:public IPhysicObjectAffector
{
	math::vector3d m_force;
public:

	IPOGravity(math::vector3d force);
	virtual~IPOGravity();

	virtual void affect(IPhysicObject*o,float dt);
};

}
}

#endif //___IPOGravity___

