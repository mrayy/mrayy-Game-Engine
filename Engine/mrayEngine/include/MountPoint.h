
/********************************************************************
	created:	2009/03/27
	created:	27:3:2009   17:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MountPoint.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MountPoint
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MountPoint___
#define ___MountPoint___

#include "IMovable.h"

namespace mray{
namespace scene{

class MountPoint:public IMovable
{
private:
protected:
	core::string m_name;

	MovableNodeList m_children;

	IMovable* m_parent;

	uint m_id;

	math::vector3d m_position;
	math::quaternion m_orintation;
	math::quaternion m_absOrintaion;
	math::vector3d m_scale;

	math::matrix4x4 m_absTransformation;
	math::matrix4x4 m_relTransformation;
public:
	MountPoint(const core::string&name);
	virtual~MountPoint();

	virtual const core::string& getNodeName()const;
	virtual bool setNodeName(const core::string&name);

	virtual const uint& getID(){return m_id;}
	virtual bool setID(const uint& id){m_id=id;return true;}

	virtual void removeChild(IMovable*elem);
	virtual void addChild(IMovableCRef elem);
	virtual void removeFromParent();
	virtual const MovableNodeList& getChildren();

	virtual IMovable* getParent();
	virtual void setParent(IMovable*p);
	virtual bool NeedChildUpdate();


	virtual math::vector3d getAbsolutePosition()const;
	virtual math::quaternion getAbsoluteOrintation()const;
	virtual math::vector3d getAbsoluteScale()const;
	virtual const math::vector3d& getPosition()const;
	virtual const math::quaternion& getOrintation()const;
	virtual const math::vector3d& getScale()const;

	virtual void updateRelativeTransformation();
	virtual void updateAbsoluteTransformation();
	virtual math::matrix4x4&getAbsoluteTransformation();
	virtual const math::matrix4x4&getAbsoluteTransformation()const;
	virtual const math::matrix4x4&getRelativeTransformation()const;

	virtual bool setPosition(const math::vector3d&v);
	virtual bool setOrintation(const math::quaternion& v);
	virtual bool setScale(const math::vector3d& v);

	virtual const math::box3d& getBoundingBox();
	virtual math::box3d getTransformedBoundingBox();
};


}
}


#endif //___MountPoint___
