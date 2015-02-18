

/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   22:10
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\include\PhysXSceneQuery.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx\include
	file base:	PhysXSceneQuery
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXSceneQuery__
#define __PhysXSceneQuery__

#include "IPhysicSceneQuery.h"

class NxSceneQuery;
class NxSceneQueryReport;

namespace mray
{
namespace physics
{
	class PhysXSceneQueryReport;

class PhysXSceneQuery:public IPhysicSceneQuery
{
private:
protected:
	NxSceneQuery* m_query;
	PhysXSceneQueryReport* m_physXReport;
public:
	PhysXSceneQuery(NxSceneQuery* q,IPhysicSceneQueryReport*r,ESceneQueryExecMode execMode);
	virtual~PhysXSceneQuery();


	void Execute();
	bool Finish(bool block);

	bool RaycaseAnyShape(const math::Ray3d& ray,EShapeType shapeType,float maxDistance,void*userData);
	IPhysicalShape* RaycaseClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance,void*userData);
	uint RaycaseAllShapes(const math::Ray3d& ray,EShapeType shapeType,float maxDistance,void*userData);

	uint OverlapSphereShapes(const math::Sphere& sphere,EShapeType shapeType,void*userData);
	uint OverlapAABBShapes(const math::box3d& aabbx,EShapeType shapeType,void*userData);
	uint OverlapOBBShapes(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType,void*userData);
	uint OverlapCapsuleShapes(const math::line3d& segment,float radius,EShapeType shapeType,void*userData);

	bool CheckOverlapSphere(const math::Sphere& sphere,EShapeType shapeType,void*userData);
	bool CheckOverlapAABB(const math::box3d& aabb,EShapeType shapeType,void*userData);
	bool CheckOverlapOBB(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType,void*userData);
	bool CheckOverlapCapsule(const math::line3d& segment,float radius,EShapeType shapeType,void*userData);

	uint CullShapes(const std::vector<math::Plane>&planes,EShapeType shapeType,void*userData);

	uint LinearSweepOBB(const math::box3d& aabb,const math::matrix3x3&rot,const math::vector3d&motion,void*userData);
	uint LinearSweepCapsule(const math::line3d& segment,float radius,const math::vector3d&motion,void*userData);

};


}
}

#endif
