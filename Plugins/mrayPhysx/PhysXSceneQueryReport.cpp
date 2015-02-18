#include "stdafx.h"

#include "PhysXSceneQueryReport.h"
#include "IPhysicSceneQueryReport.h"

#include "physXCommon.h"
#include "PhysicsSweepQueryHit.h"

#include <NxScene.h>

namespace mray
{
namespace physics
{
	
NxQueryReportResult	PhysXSceneQueryReport::onBooleanQuery(void* userData, bool result)
{
	return NX_SQR_CONTINUE;
}

NxQueryReportResult	PhysXSceneQueryReport::onRaycastQuery(void* userData, NxU32 nbHits, const NxRaycastHit* hits)
{
	std::vector<PhysicsRaycaseHit> hVec;
	hVec.resize(nbHits);

	for (int i=0;i<nbHits;++i)
	{
		hVec[i].distance=hits[i].distance;
		hVec[i].worldImpact=ToVec3(hits[i].worldImpact);
		hVec[i].worldNormal=ToVec3(hits[i].worldNormal);
		hVec[i].shape=(IPhysicalShape*)hits[i].shape->userData;
		hVec[i].materialID=hits[i].materialIndex;
	}

	EQueryReportResult r= m_report->OnRaycastQuery(userData,hVec);
	switch(r)
	{
	case EQResult_AbortQuery:
		return NX_SQR_ABORT_QUERY;
	case EQResult_AbortAllQueries:
		return NX_SQR_ABORT_ALL_QUERIES;
	default:
		return NX_SQR_CONTINUE;
	}
}

NxQueryReportResult	PhysXSceneQueryReport::onShapeQuery(void* userData, NxU32 nbHits, NxShape** hits)
{
	std::vector<IPhysicalShape*> hVec;
	hVec.resize(nbHits);

	for (int i=0;i<nbHits;++i)
	{
		hVec[i]=(IPhysicalShape*)hits[i]->userData;
	}

	EQueryReportResult r= m_report->OnShapeQuery(userData,hVec);
	switch(r)
	{
	case EQResult_AbortQuery:
		return NX_SQR_ABORT_QUERY;
	case EQResult_AbortAllQueries:
		return NX_SQR_ABORT_ALL_QUERIES;
	default:
		return NX_SQR_CONTINUE;
	}
}

NxQueryReportResult	PhysXSceneQueryReport::onSweepQuery(void* userData, NxU32 nbHits, NxSweepQueryHit* hits)
{
	std::vector<PhysicsSweepQueryHit> hVec;
	hVec.resize(nbHits);

	for (int i=0;i<nbHits;++i)
	{
		hVec[i].t=hits[i].t;
		hVec[i].normal=ToVec3(hits[i].normal);
		hVec[i].point=ToVec3(hits[i].point);
		
		hVec[i].hitShape=(IPhysicalShape*)hits[i].hitShape->userData;
		hVec[i].sweepShape=(IPhysicalShape*)hits[i].sweepShape->userData;
	}

	EQueryReportResult r= m_report->OnSweepQuery(userData,hVec);
	switch(r)
	{
	case EQResult_AbortQuery:
		return NX_SQR_ABORT_QUERY;
	case EQResult_AbortAllQueries:
		return NX_SQR_ABORT_ALL_QUERIES;
	default:
		return NX_SQR_CONTINUE;
	}
}




}
}