
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   22:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IPhysicObjectAffector.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IPhysicObjectAffector
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IPhysicObjectAffector___
#define ___IPhysicObjectAffector___

#include "compileConfig.h"

namespace mray{
namespace physics{

class IPhysicObject;

class MRAY_DLL IPhysicObjectAffector
{
public:

	IPhysicObjectAffector(){}
	virtual~IPhysicObjectAffector(){}

	virtual void affect(IPhysicObject*o,float dt)=0;

};

}
}

#endif //___IPhysicObjectAffector___

