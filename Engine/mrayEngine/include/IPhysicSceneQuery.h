

/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   21:56
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicSceneQuery.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicSceneQuery
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicSceneQuery__
#define __IPhysicSceneQuery__




#include "IPhysical3DDef.h"
#include "PhysicsRaycaseHit.h"



namespace mray
{
namespace physics
{
	class IPhysicalShape;

class IPhysicSceneQuery
{
private:
protected:
	IPhysicSceneQueryReport* m_report;
	ESceneQueryExecMode m_execMode;
public:
	IPhysicSceneQuery(IPhysicSceneQueryReport*r,ESceneQueryExecMode execMode):m_report(r),m_execMode(execMode)
	{}
	virtual~IPhysicSceneQuery(){}

	IPhysicSceneQueryReport* GetQueryReport(){return m_report;}
	ESceneQueryExecMode GetExecMode(){return m_execMode;}

	virtual void Execute()=0;
	virtual bool Finish(bool block=false)=0;

	virtual bool RaycaseAnyShape(const math::Ray3d& ray,EShapeType shapeType,float maxDistance=math::Infinity,void*userData=0)=0;
	virtual IPhysicalShape* RaycaseClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance=math::Infinity,void*userData=0)=0;
	
	virtual uint RaycaseAllShapes(const math::Ray3d& ray,EShapeType shapeType,float maxDistance=math::Infinity,void*userData=0)=0;
	virtual uint OverlapSphereShapes(const math::Sphere& sphere,EShapeType shapeType,void*userData=0)=0;
	virtual uint OverlapAABBShapes(const math::box3d& aabb,EShapeType shapeType,void*userData=0)=0;
	virtual uint OverlapOBBShapes(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType,void*userData=0)=0;
	virtual uint OverlapCapsuleShapes(const math::line3d& segment,float radius,EShapeType shapeType,void*userData=0)=0;

	virtual bool CheckOverlapSphere(const math::Sphere& sphere,EShapeType shapeType=EShape_All,void*userData=0)=0;
	virtual bool CheckOverlapAABB(const math::box3d& aabb,EShapeType shapeType=EShape_All,void*userData=0)=0;
	virtual bool CheckOverlapOBB(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType=EShape_All,void*userData=0)=0;
	virtual bool CheckOverlapCapsule(const math::line3d& segment,float radius,EShapeType shapeType=EShape_All,void*userData=0)=0;

	virtual uint CullShapes(const std::vector<math::Plane>&planes,EShapeType shapeType=EShape_All,void*userData=0)=0;

	virtual uint LinearSweepOBB(const math::box3d& aabb,const math::matrix3x3&rot,const math::vector3d&motion,void*userData=0)=0;
	virtual uint LinearSweepCapsule(const math::line3d& segment,float radius,const math::vector3d&motion,void*userData=0)=0;
};


}
}

#endif
