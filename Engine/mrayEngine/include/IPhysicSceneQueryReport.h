


/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   21:57
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IPhysicSceneQueryReport.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IPhysicSceneQueryReport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IPhysicSceneQueryReport__
#define __IPhysicSceneQueryReport__

#include "PhysicsRaycaseHit.h"
#include "PhysicsSweepQueryHit.h"



namespace mray
{
namespace physics
{

	enum EQueryReportResult 
	{
		EQResult_Continue,		
		EQResult_AbortQuery,	
		EQResult_AbortAllQueries
	};

class IPhysicSceneQueryReport
{
private:
protected:
public:
	IPhysicSceneQueryReport(){}
	virtual~IPhysicSceneQueryReport(){}

	virtual EQueryReportResult OnRaycastQuery(void*userData,const std::vector<PhysicsRaycaseHit>& hits)=0;

	virtual EQueryReportResult OnShapeQuery(void*userData,const std::vector<IPhysicalShape*>& hits)=0;

	virtual EQueryReportResult OnSweepQuery(void*userData,const std::vector<PhysicsSweepQueryHit>& hits)=0;

};


}
}

#endif
