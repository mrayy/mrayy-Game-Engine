

/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   22:02
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhysicsSweepQueryHit.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhysicsSweepQueryHit
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysicsSweepQueryHit__
#define __PhysicsSweepQueryHit__



namespace mray
{
namespace physics
{

	class IPhysicalShape;

class PhysicsSweepQueryHit
{
private:
protected:
public:
	PhysicsSweepQueryHit():t(0),hitShape(0),sweepShape(0),userData(0)
	{}

	float t;//[0->1]
	IPhysicalShape* hitShape;
	IPhysicalShape* sweepShape;
	void*userData;
	math::vector3d point;
	math::vector3d normal;
};


}
}

#endif
