
/********************************************************************
	created:	2009/11/28
	created:	28:11:2009   21:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx\PhysXShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayPhysx\mrayPhysx
	file base:	PhysXShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysXShape___
#define ___PhysXShape___


#include "IPhysicalShape.h"
#include <NxShape.h>

namespace mray{
namespace physics{

class PhysXShape:public IPhysicalShape
{
private:
protected:
	NxShape* m_nxShape;
	core::string m_name;

	IPhysicalNode* m_node;
public:
	PhysXShape(NxShape*shape,IPhysicalNode* node);
	virtual~PhysXShape();

	NxShape* getNxShape();

	IPhysicalNode* getPhysicalNode();
	void  setPhysicalNode(IPhysicalNode* node);
	
	const core::string& getName();

	void setCollisionGroup(ushort collisionGroup);
	ushort getCollisionGroup()const;
	void getWorldBounds(math::box3d&bounds)const;

	void setLocalPos(const math::matrix4x4&m);
	void setLocalTranslation(const math::vector3d&v);
	void setLocalOrintation(const math::quaternion&v);

	math::matrix4x4 getLocalPos()const;
	math::vector3d getLocalTranslation()const;
	math::quaternion getLocalOrintation()const;

	void setGlobalPos(const math::matrix4x4&m);
	void setGlobalTranslation(const math::vector3d&v);
	void setGlobalOrintation(const math::quaternion&v);

	math::matrix4x4 getGlobalPos()const;
	math::vector3d getGlobalTranslation()const;
	math::quaternion getGlobalOrintation()const;

	void setMaterial(IPhysicMaterial* mat);
	IPhysicMaterial* getMaterial()const;

	void setSkinWidth(float w);
	float getSkinWidth();

};

}
}

#endif //___PhysXShape___

