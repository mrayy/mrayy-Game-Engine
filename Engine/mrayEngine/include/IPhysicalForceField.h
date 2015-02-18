


/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   10:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicalForceField.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicalForceField
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicalForceField__
#define __IPhysicalForceField__

#include "IObject.h"
#include "PhysicalForceFieldDef.h"

namespace mray
{
namespace physics
{
	class IPhysicManager;

class IPhysicalForceField:public IObject
{
private:
protected:
	IObject* m_userData;
	
public:
	IPhysicalForceField():m_userData(0){}
	virtual~IPhysicalForceField(){}

	virtual IPhysicManager* getCreator()const=0;

	virtual void setPose(const math::matrix4x4 &m)=0;
	virtual math::matrix4x4 getPose()const=0;

	virtual void setNode(IPhysicalNode* node)=0;
	virtual IPhysicalNode* getNode()const=0;

	virtual void setKernal(IPhysicalForceFieldLinearKernal* k)=0;
	virtual IPhysicalForceFieldLinearKernal* getKernal()const=0;

	virtual IPhysicalForceFieldShapeGroup* getIncludeShapeGroup()=0;

	virtual void addShapeGroup(IPhysicalForceFieldShapeGroup* g)=0;
	virtual void removeShapeGroup(IPhysicalForceFieldShapeGroup* g)=0;
	virtual const std::list<IPhysicalForceFieldShapeGroup*> getShapeGroups()const=0;

	virtual void setGroupID(GroupID g)=0;
	virtual GroupID getGroupID()const=0;
	
	virtual void setGroupMask(uint *bits)=0;
	virtual void getGroupMask(uint *bits)const=0;

	virtual void setCoordinates(EForceFieldCoordinates c)=0;
	virtual EForceFieldCoordinates getCoordinates()const=0;

	virtual void setName(const core::string& name)=0;
	virtual const core::string& getName()=0;

	virtual void setFluidType(EForceFieldType t)=0;
	virtual EForceFieldType getFluidType()const=0;

	virtual void setClothType(EForceFieldType t)=0;
	virtual EForceFieldType getClothType()const=0;

	virtual void setSoftBodyType(EForceFieldType t)=0;
	virtual EForceFieldType getSoftBodyType()const=0;

	virtual void setRigidBodyType(EForceFieldType t)=0;
	virtual EForceFieldType getRigidBodyType()const=0;

	virtual void setFlag(EForceFieldFlags f,bool s)=0;
	virtual bool getFlag(EForceFieldFlags f)=0;

	virtual ForceFieldVariety getVariety()const=0;
	virtual void setVariety(ForceFieldVariety v)=0;

	IObject* getUserData()const{return m_userData;}
	void setUserData(IObject* o){m_userData=o;}
};


}
}

#endif
