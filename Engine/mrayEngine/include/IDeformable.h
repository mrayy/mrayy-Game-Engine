
/********************************************************************
	created:	2008/10/25
	created:	25:10:2008   19:33
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IDeformable.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IDeformable
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IDeformable___
#define ___IDeformable___

#include "IPhysicObjectAffector.h"
#include "IPhysicObject.h"

namespace mray{
namespace physics{

class MRAY_DLL IDeformable:public IPhysicObjectAffector
{
protected:
	IPhysicObject*m_mesh;
public:
	virtual void setPhysicObject(IPhysicObject*mesh){
		m_mesh=mesh;
	}
	IPhysicObject*getPhysicObject(){
		return m_mesh;
	}

};

}
}

#endif //___IDeformable___

