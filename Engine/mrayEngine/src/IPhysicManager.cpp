
#include "stdafx.h"
#include "IPhysicManager.h"
#include "PhComplexContactReporter.h"
#include "PhTriggerReporterComplex.h"
#include "PhysicsMaterialManager.h"


namespace mray
{
namespace physics
{

IPhysicManager::IPhysicManager()
{
	m_report=new PhComplexContactReporter();
	m_triggerReport=new PhTriggerReporterComplex();
	m_materialManager=new PhysicsMaterialManager();
}
IPhysicManager::~IPhysicManager()
{
	delete m_report;
	delete m_triggerReport;
	delete m_materialManager;
}


void IPhysicManager::AddContactReport(IPhysicUserContactReport*r)
{
	m_report->AddReporter(r);
}
void IPhysicManager::RemoveContactReport(IPhysicUserContactReport*r)
{
	m_report->RemoveReporter(r);
}
IPhysicUserContactReport* IPhysicManager::GetContactReport()
{
	return m_report;
}

void IPhysicManager::AddTriggerReport(IPhysicalTriggerReporter*r)
{
	m_triggerReport->AddReporter(r);
}

void IPhysicManager::RemovTriggerReport(IPhysicalTriggerReporter*r)
{
	m_triggerReport->RemoveReporter(r);
}

IPhysicalTriggerReporter* IPhysicManager::GetTriggerReport(){return m_triggerReport;}
}
}