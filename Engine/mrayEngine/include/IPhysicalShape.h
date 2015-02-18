
/********************************************************************
	created:	2009/11/27
	created:	27:11:2009   16:11
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalShape___
#define ___IPhysicalShape___




#include "mTypes.h"
#include "IPhysical3DDef.h"

namespace mray{
namespace physics{
	
	class IPhysicalNode;
	class IPhysicMaterial;

class IPhysicalShape
{
private:
protected:
	void* m_userData;
public:
	IPhysicalShape():m_userData(0){}
	virtual~IPhysicalShape(){}

	virtual IPhysicalNode* getPhysicalNode()=0;

	virtual const core::string& getName()=0;

	virtual void setCollisionGroup(GroupID collisionGroup)=0;
	virtual GroupID getCollisionGroup()const=0;
	virtual void getWorldBounds(math::box3d&bounds)const=0;

	virtual void setLocalPos(const math::matrix4x4&m)=0;
	virtual void setLocalTranslation(const math::vector3d&v)=0;
	virtual void setLocalOrintation(const math::quaternion&v)=0;

	virtual math::matrix4x4 getLocalPos()const=0;
	virtual math::vector3d getLocalTranslation()const=0;
	virtual math::quaternion getLocalOrintation()const=0;

	virtual void setGlobalPos(const math::matrix4x4&m)=0;
	virtual void setGlobalTranslation(const math::vector3d&v)=0;
	virtual void setGlobalOrintation(const math::quaternion&v)=0;

	virtual math::matrix4x4 getGlobalPos()const=0;
	virtual math::vector3d getGlobalTranslation()const=0;
	virtual math::quaternion getGlobalOrintation()const=0;

	virtual void setMaterial(IPhysicMaterial* mat)=0;
	virtual IPhysicMaterial* getMaterial()const=0;

	virtual void setSkinWidth(float w)=0;
	virtual float getSkinWidth()=0;

	void* GetUserData()const{return m_userData;}
	void SetUserData(void*d){m_userData=d;}

};

}
}


#endif //___IPhysicalShape___

