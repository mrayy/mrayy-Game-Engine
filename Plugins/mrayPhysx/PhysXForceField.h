


/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   13:24
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXForceField.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXForceField
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXForceField__
#define __PhysXForceField__

#include "IPhysicalForceField.h"

class NxForceField;

namespace mray
{
namespace physics
{

	class PhysXManager;

class PhysXForceField:public IPhysicalForceField
{
private:
protected:
	NxForceField* m_forceField;
	IPhysicalForceFieldShapeGroup* m_includeShapeGroup;

	std::list<IPhysicalForceFieldShapeGroup*> m_groups;
	core::string m_name;

	PhysXManager* m_creator;
public:
	PhysXForceField(NxForceField* f,PhysXManager* creator);
	virtual~PhysXForceField();

	NxForceField* getNxForceField(){return m_forceField;}

	IPhysicManager* getCreator()const;

	void setPose(const math::matrix4x4 &m);
	math::matrix4x4 getPose()const;

	void setNode(IPhysicalNode* node);
	IPhysicalNode* getNode()const;

	void setKernal(IPhysicalForceFieldLinearKernal* k);
	IPhysicalForceFieldLinearKernal* getKernal()const;

	IPhysicalForceFieldShapeGroup* getIncludeShapeGroup();

	void addShapeGroup(IPhysicalForceFieldShapeGroup* g);
	void removeShapeGroup(IPhysicalForceFieldShapeGroup* g);
	const std::list<IPhysicalForceFieldShapeGroup*> getShapeGroups()const;

	void setGroupID(GroupID g);
	GroupID getGroupID()const;

	void setGroupMask(uint *bits);
	void getGroupMask(uint *bits)const;

	void setCoordinates(EForceFieldCoordinates c);
	EForceFieldCoordinates getCoordinates()const;

	void setName(const core::string& name);
	const core::string& getName();

	void setFluidType(EForceFieldType t);
	EForceFieldType getFluidType()const;

	void setClothType(EForceFieldType t);
	EForceFieldType getClothType()const;

	void setSoftBodyType(EForceFieldType t);
	EForceFieldType getSoftBodyType()const;

	void setRigidBodyType(EForceFieldType t);
	EForceFieldType getRigidBodyType()const;

	void setFlag(EForceFieldFlags f,bool s);
	bool getFlag(EForceFieldFlags f);

	ForceFieldVariety getVariety()const;
	void setVariety(ForceFieldVariety v);
};

}
}

#endif
