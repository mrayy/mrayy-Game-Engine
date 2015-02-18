#include "stdafx.h"

#include "PhysXSceneQuery.h"
#include "PhysXSceneQueryReport.h"
#include "PhysXCommon.h"
#include "PhysXObjectConverter.h"
#include <NxRay.h>
#include <NxSphere.h>
#include <NxCapsule.h>
#include <NxPlane.h>
#include <NxBox.h>
#include <NxBounds3.h>

namespace mray
{
namespace physics
{


PhysXSceneQuery::PhysXSceneQuery(NxSceneQuery* q,IPhysicSceneQueryReport*r,ESceneQueryExecMode execMode)
:IPhysicSceneQuery(r,execMode),m_query(q)
{
	m_physXReport=(PhysXSceneQueryReport*)m_query->getQueryReport();
}

PhysXSceneQuery::~PhysXSceneQuery()
{
	delete m_physXReport;
}



void PhysXSceneQuery::Execute()
{
	m_query->execute();
}

bool PhysXSceneQuery::Finish(bool block)
{
	return m_query->finish(block);
}


bool PhysXSceneQuery::RaycaseAnyShape(const math::Ray3d& ray,EShapeType shapeType,float maxDistance,void*userData)
{
	return m_query->raycastAnyShape(NxRay(ToNxVec3(ray.Start),ToNxVec3(ray.Dir)),PhysXObjectConverter::convertShapeType(shapeType),-1,maxDistance,0,0,userData);
}

IPhysicalShape* PhysXSceneQuery::RaycaseClosestShape(const math::Ray3d& ray,EShapeType shapeType,PhysicsRaycaseHit&hit,float maxDistance,void*userData)
{
	NxRaycastHit hres;
	hres.shape=0;
	/*NxShape*s=*/m_query->raycastClosestShape(NxRay(ToNxVec3(ray.Start),ToNxVec3(ray.Dir)),PhysXObjectConverter::convertShapeType(shapeType),hres,0xffffffff,maxDistance,0xffffffff,0,0,userData);

/*

	hit.distance=hres.distance;
	hit.worldImpact=ToVec3(hres.worldImpact);
	hit.worldNormal=ToVec3(hres.worldNormal);
	if(hres.shape)
		hit.shape=(IPhysicalShape*)hres.shape->userData;
	hit.materialID=hres.materialIndex;

	if(!s)return 0;
	return (IPhysicalShape*)s->userData;
	*/

	return 0;
}


uint PhysXSceneQuery::RaycaseAllShapes(const math::Ray3d& ray,EShapeType shapeType,float maxDistance,void*userData)
{
	return m_query->raycastAllShapes(NxRay(ToNxVec3(ray.Start),ToNxVec3(ray.Dir)),PhysXObjectConverter::convertShapeType(shapeType),
		-1,maxDistance,-1,0,userData);
}

uint PhysXSceneQuery::OverlapSphereShapes(const math::Sphere& sphere,EShapeType shapeType,void*userData)
{
	return m_query->overlapSphereShapes(NxSphere(ToNxVec3(sphere.center),sphere.radius),PhysXObjectConverter::convertShapeType(shapeType),
		-1,0,userData);
}

uint PhysXSceneQuery::OverlapAABBShapes(const math::box3d& aabb,EShapeType shapeType,void*userData)
{
	NxBounds3 b;
	b.set(ToNxVec3(aabb.MinP),ToNxVec3(aabb.MaxP));
	return m_query->overlapAABBShapes(b,PhysXObjectConverter::convertShapeType(shapeType),
		-1,0,userData);
}

uint PhysXSceneQuery::OverlapOBBShapes(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType,void*userData)
{
	math::vector3d c=aabb.getCenter();
	math::vector3d e=aabb.getExtent();
	NxMat33 m;
	m.setRowMajor(rot.mat);
	return m_query->overlapOBBShapes(NxBox(ToNxVec3(c),ToNxVec3(e),m),PhysXObjectConverter::convertShapeType(shapeType),
		-1,0,userData);
}

uint PhysXSceneQuery::OverlapCapsuleShapes(const math::line3d& segment,float radius,EShapeType shapeType,void*userData)
{
	return m_query->overlapCapsuleShapes(NxCapsule(NxSegment(ToNxVec3(segment.pStart),ToNxVec3(segment.pEnd)),radius),PhysXObjectConverter::convertShapeType(shapeType),
		-1,0,userData);
}


bool PhysXSceneQuery::CheckOverlapSphere(const math::Sphere& sphere,EShapeType shapeType,void*userData)
{
	return m_query->checkOverlapSphere(NxSphere(ToNxVec3(sphere.center),sphere.radius),PhysXObjectConverter::convertShapeType(shapeType),-1,0,userData);
}

bool PhysXSceneQuery::CheckOverlapAABB(const math::box3d& aabb,EShapeType shapeType,void*userData)
{
	NxBounds3 b;
	b.set(ToNxVec3(aabb.MinP),ToNxVec3(aabb.MaxP));
	return m_query->checkOverlapAABB(b,PhysXObjectConverter::convertShapeType(shapeType),-1,0,userData);
}

bool PhysXSceneQuery::CheckOverlapOBB(const math::box3d& aabb,const math::matrix3x3&rot,EShapeType shapeType,void*userData)
{
	math::vector3d c=aabb.getCenter();
	math::vector3d e=aabb.getExtent();
	NxMat33 m;
	m.setRowMajor(rot.mat);
	return m_query->checkOverlapOBB(NxBox(ToNxVec3(c),ToNxVec3(e),m),PhysXObjectConverter::convertShapeType(shapeType),-1,0,userData);
}

bool PhysXSceneQuery::CheckOverlapCapsule(const math::line3d& segment,float radius,EShapeType shapeType,void*userData)
{
	return m_query->checkOverlapCapsule(NxCapsule(NxSegment(ToNxVec3(segment.pStart),ToNxVec3(segment.pEnd)),radius),PhysXObjectConverter::convertShapeType(shapeType),-1,0,userData);
}


uint PhysXSceneQuery::CullShapes(const std::vector<math::Plane>&planes,EShapeType shapeType,void*userData)
{
	std::vector<NxPlane> nxPlanes;
	nxPlanes.resize(planes.size());
	for(int i=0;i<planes.size();++i)
	{
		nxPlanes[i].set(ToNxVec3(planes[i].Normal),planes[i].D);
	}
	return m_query->cullShapes(nxPlanes.size(),&nxPlanes[0],PhysXObjectConverter::convertShapeType(shapeType),-1,0,userData);
}


uint PhysXSceneQuery::LinearSweepOBB(const math::box3d& aabb,const math::matrix3x3&rot,const math::vector3d&motion,void*userData)
{
	math::vector3d c=aabb.getCenter();
	math::vector3d e=aabb.getExtent();
	NxMat33 m;
	m.setRowMajor(rot.mat);
	return m_query->linearOBBSweep(NxBox(ToNxVec3(c),ToNxVec3(e),m),ToNxVec3(motion),0,-1,0,userData);
}

uint PhysXSceneQuery::LinearSweepCapsule(const math::line3d& segment,float radius,const math::vector3d&motion,void*userData)
{
	return m_query->linearCapsuleSweep(NxCapsule(NxSegment(ToNxVec3(segment.pStart),ToNxVec3(segment.pEnd)),radius),ToNxVec3(motion),0,-1,0,userData);
}



}
}
