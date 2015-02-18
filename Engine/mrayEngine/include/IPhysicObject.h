
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   17:41
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IPhysicObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IPhysicObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	interface for a physical object
*********************************************************************/


#ifndef ___IPhysicObject___
#define ___IPhysicObject___

#include <point3d.h>
#include <mArray.h>
#include <GCPtr.h>
#include "IPhysicObjectAffector.h"

namespace mray{
namespace physics{

struct MRAY_DLL PhysicalVertex{
private:
	float weight;
	float iweight;

public:
	math::vector3d oldPos;
	math::vector3d pos;
	math::vector3d force;
	math::vector3d vel;

	PhysicalVertex();

	void setWeight(float w);
	float getWeight();

	math::vector3d getNextPos(float dt);
	void update(float dt);
};

class MRAY_DLL IPhysicObject
{
public:

protected:
	std::vector<PhysicalVertex> m_vertices;


	typedef std::list<GCPtr<IPhysicObjectAffector>> AffectorList;
	AffectorList m_affectors;
	void update(float dt);
public:


	IPhysicObject();
	virtual~IPhysicObject();

	PhysicalVertex*getVertex(int i);
	PhysicalVertex*getVerticies();
	int getVertexCount();

	void translate(math::vector3d p);

	void addAffector(GCPtr<IPhysicObjectAffector>a);
	void clearAffectors();

	virtual void updateVertices(float dt)=0;
};

}
}

#endif //___IPhysicObject___

