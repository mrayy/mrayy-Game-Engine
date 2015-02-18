

/********************************************************************
	created:	2011/05/13
	created:	13:5:2011   22:35
	filename: 	d:\Development\mrayEngine\Engine\mrayPhysx\PhysXSceneQueryReport.h
	file path:	d:\Development\mrayEngine\Engine\mrayPhysx
	file base:	PhysXSceneQueryReport
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhysXSceneQueryReport__
#define __PhysXSceneQueryReport__

#include <NxSceneQuery.h>

namespace mray
{
namespace physics
{
	class IPhysicSceneQueryReport;

class PhysXSceneQueryReport:public NxSceneQueryReport
{
private:
protected:
	IPhysicSceneQueryReport* m_report;
public:
	PhysXSceneQueryReport(IPhysicSceneQueryReport*r):m_report(r)
	{}
	virtual~PhysXSceneQueryReport(){}

	NxQueryReportResult	onBooleanQuery(void* userData, bool result)								;
	NxQueryReportResult	onRaycastQuery(void* userData, NxU32 nbHits, const NxRaycastHit* hits)	;
	NxQueryReportResult	onShapeQuery(void* userData, NxU32 nbHits, NxShape** hits)				;
	NxQueryReportResult	onSweepQuery(void* userData, NxU32 nbHits, NxSweepQueryHit* hits)		;

};


}
}

#endif
