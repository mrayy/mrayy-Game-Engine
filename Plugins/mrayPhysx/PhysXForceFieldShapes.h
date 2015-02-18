

/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   14:31
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXForceFieldShapes.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXForceFieldSphereShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXForceFieldShapes__
#define __PhysXForceFieldShapes__

#include "IPhysicalForceFieldShapes.h"

class NxSphereForceFieldShape;

namespace mray
{
namespace physics
{


class PhysXForceFieldSphereShape:public IPhysicalForceFieldSphereShape
{
private:
protected:
	NxSphereForceFieldShape* m_shape;
	core::string m_name;
public:
	PhysXForceFieldSphereShape(NxSphereForceFieldShape* s):m_shape(s)
	{}
	virtual~PhysXForceFieldSphereShape(){}

	NxSphereForceFieldShape* getNxShape(){return m_shape;}

	math::matrix4x4 getTransformation()const;
	void setTransformation(const math::matrix4x4& t);

	IPhysicalForceField* getOwnerForceField()const;

	void setName(const core::string &name);
	const core::string& getName()const{return m_name;}

	void setRadius(float r);
	float getRadius()const;

};



class PhysXForceFieldCapsuleShape:public IPhysicalForceFieldCapsuleShape
{
private:
protected:
	NxCapsuleForceFieldShape* m_shape;
	core::string m_name;
public:
	PhysXForceFieldCapsuleShape(NxCapsuleForceFieldShape* s):m_shape(s)
	{}
	virtual~PhysXForceFieldCapsuleShape(){}

	NxCapsuleForceFieldShape* getNxShape(){return m_shape;}

	math::matrix4x4 getTransformation()const;
	void setTransformation(const math::matrix4x4& t);

	IPhysicalForceField* getOwnerForceField()const;

	void setName(const core::string &name);
	const core::string& getName()const{return m_name;}

	void setRadius(float r);
	float getRadius()const;

	void setHeight(float r);
	float getHeight()const;

};



class PhysXForceFieldBoxShape:public IPhysicalForceFieldBoxShape
{
private:
protected:
	NxBoxForceFieldShape* m_shape;
	core::string m_name;
public:
	PhysXForceFieldBoxShape(NxBoxForceFieldShape* s):m_shape(s)
	{}
	virtual~PhysXForceFieldBoxShape(){}

	NxBoxForceFieldShape* getNxShape(){return m_shape;}

	math::matrix4x4 getTransformation()const;
	void setTransformation(const math::matrix4x4& t);

	IPhysicalForceField* getOwnerForceField()const;

	void setName(const core::string &name);
	const core::string& getName()const{return m_name;}

	void setDimensions(const math::vector3d& r);
	math::vector3d getDimensions()const;

};

class PhysXForceFieldConvexShape:public IPhysicalForceFieldConvexShape
{
private:
protected:
	NxConvexForceFieldShape* m_shape;
	core::string m_name;
public:
	PhysXForceFieldConvexShape(NxConvexForceFieldShape* s):m_shape(s)
	{}
	virtual~PhysXForceFieldConvexShape(){}

	NxConvexForceFieldShape* getNxShape(){return m_shape;}

	math::matrix4x4 getTransformation()const;
	void setTransformation(const math::matrix4x4& t);

	IPhysicalForceField* getOwnerForceField()const;

	void setName(const core::string &name);
	const core::string& getName()const{return m_name;}

};

}
}

#endif