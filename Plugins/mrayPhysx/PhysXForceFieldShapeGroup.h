

/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   13:46
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXForceFieldShapeGroup.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXForceFieldShapeGroup
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXForceFieldShapeGroup__
#define __PhysXForceFieldShapeGroup__

#include "IPhysicalForceFieldShapeGroup.h"

class NxForceFieldShapeGroup;
class NxForceFieldShape;

namespace mray
{
namespace physics
{

class PhysXForceFieldShapeGroup:public IPhysicalForceFieldShapeGroup
{
private:
protected:
	NxForceFieldShapeGroup* m_group;
	std::list<IPhysicalForceFieldShape*> m_shapes;
	core::string m_name;
	IObject* m_userData;

	IPhysicalForceFieldShape* _CreateShape(NxForceFieldShape* s);
public:
	PhysXForceFieldShapeGroup(NxForceFieldShapeGroup* g);
	virtual~PhysXForceFieldShapeGroup();

	NxForceFieldShapeGroup* getNxShapeGroup();

	void setName(const core::string& name);
	const core::string& getName()const;

	void setUserData(IObject* d);
	IObject* getUserData()const;

	IPhysicalForceFieldShape* createShape(IPhysicalForceFieldShapeDesc* desc);
	void releaseShape(IPhysicalForceFieldShape*shape);

	IPhysicalForceField* getForceField()const;

	const std::list<IPhysicalForceFieldShape*>& getShapes()const;
};


}
}

#endif