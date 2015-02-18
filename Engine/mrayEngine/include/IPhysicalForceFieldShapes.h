
/********************************************************************
	created:	2012/02/15
	created:	15:2:2012   11:47
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicalForceFieldShapes.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicalForceFieldShapes
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicalForceFieldShapes__
#define __IPhysicalForceFieldShapes__

#include "IObject.h"
#include "PhysicalForceFieldDef.h"

namespace mray
{
namespace physics
{

class IPhysicalForceField;

class MRAY_DLL IPhysicalForceFieldShape:public IObject
{
	DECLARE_RTTI;
private:
protected:
public:
	IPhysicalForceFieldShape(){}
	virtual~IPhysicalForceFieldShape(){}

	virtual math::matrix4x4 getTransformation()const=0;
	virtual void setTransformation(const math::matrix4x4& t)=0;

	virtual IPhysicalForceField* getOwnerForceField()const=0;

	virtual void setName(const core::string &name)=0;
	virtual const core::string& getName()const=0;

	virtual EForceFieldShapeType getType()const=0;
};


class IPhysicalForceFieldSphereShape:public IPhysicalForceFieldShape
{
private:
protected:
public:
	IPhysicalForceFieldSphereShape(){}
	virtual~IPhysicalForceFieldSphereShape(){}

	virtual void setRadius(float r)=0;
	virtual float getRadius()const=0;


	EForceFieldShapeType getType()const
	{
		return EFFShape_Sphere;
	}
};

class IPhysicalForceFieldCapsuleShape:public IPhysicalForceFieldShape
{
private:
protected:
public:
	IPhysicalForceFieldCapsuleShape(){}
	virtual~IPhysicalForceFieldCapsuleShape(){}

	virtual void setRadius(float r)=0;
	virtual float getRadius()const=0;

	virtual void setHeight(float r)=0;
	virtual float getHeight()const=0;

	EForceFieldShapeType getType()const
	{
		return EFFShape_Capsule;
	}
};

class IPhysicalForceFieldBoxShape:public IPhysicalForceFieldShape
{
private:
protected:
public:
	IPhysicalForceFieldBoxShape(){}
	virtual~IPhysicalForceFieldBoxShape(){}

	virtual void setDimensions(const math::vector3d& r)=0;
	virtual math::vector3d getDimensions()const=0;
	EForceFieldShapeType getType()const
	{
		return EFFShape_Box;
	}
};
class IPhysicalForceFieldConvexShape:public IPhysicalForceFieldShape
{
private:
protected:
public:
	IPhysicalForceFieldConvexShape(){}
	virtual~IPhysicalForceFieldConvexShape(){}

	EForceFieldShapeType getType()const
	{
		return EFFShape_Convex;
	}
};

}
}

#endif


