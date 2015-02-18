
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   16:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\PhysicMesh.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	PhysicMesh
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this implementation contains a definition to a physical mesh
				which contains forces for each vertex
*********************************************************************/


#ifndef ___PhysicMesh___
#define ___PhysicMesh___

#include <IMeshBuffer.h>
#include "IPhysicObject.h"

namespace mray{
namespace physics{

class MRAY_DLL PhysicMesh:public IPhysicObject
{
protected:
	scene::IMeshBuffer*m_mesh;
public:
	PhysicMesh(scene::IMeshBuffer*mesh);
	virtual~PhysicMesh();

	void setMesh(scene::IMeshBuffer*mesh);
	scene::IMeshBuffer*getMesh();

	virtual void updateVertices(float dt);
};

}
}

#endif //___PhysicMesh___

