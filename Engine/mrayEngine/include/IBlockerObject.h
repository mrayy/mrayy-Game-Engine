
/********************************************************************
	created:	2008/10/24
	created:	24:10:2008   22:15
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI\IBlockerObject.h
	file BOth:	i:\Programing\GameEngine\mrayEngine\mrayCEGUItest\mrayCEGUI
	file base:	IBlockerObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef ___IBlockerObject___
#define ___IBlockerObject___

#include "IPhysicObject.h"
#include <Plane.h>
#include <box3d.h>

namespace mray{
namespace physics{

class MRAY_DLL IBlockerObject:public IPhysicObjectAffector
{
protected:
	float m_bounce;
	virtual bool test(PhysicalVertex &p,float dt)=0;
public:

	IBlockerObject();
	virtual~IBlockerObject();

	virtual void affect(IPhysicObject*o,float dt);

	void setBounce(float b);
	float getBounce();

};


////////////////////////////
class MRAY_DLL BOBoxBlocker:public IBlockerObject
{
protected:
	math::box3d m_box;
	virtual bool test(PhysicalVertex &p,float dt);
public:

	BOBoxBlocker();
	BOBoxBlocker(const math::box3d &b);


	void setBox(const math::box3d &b);
	math::box3d getBox();
};

////////////////////////////
class MRAY_DLL BOPlaneBlocker:public IBlockerObject
{
protected:
	math::Plane m_plane;
	virtual bool test(PhysicalVertex &p,float dt);
public:

	BOPlaneBlocker();
	BOPlaneBlocker(const math::Plane &p);
	virtual~BOPlaneBlocker();


	void setPlane(const math::Plane &p);
	math::Plane getPlane();
};

////////////////////////////
class MRAY_DLL BOSphereBlocker:public IBlockerObject
{
protected:

	math::vector3d m_center;
	float m_radius;
	float m_sqrRadius;
	virtual bool test(PhysicalVertex &p,float dt);
public:

	BOSphereBlocker();
	BOSphereBlocker(const math::vector3d &c,float r);
	virtual~BOSphereBlocker();


	void setSphere(const math::vector3d &c,float r);
	float getRadius();
	math::vector3d getCenter();
};


}
}

#endif //___IBlockerObject___


