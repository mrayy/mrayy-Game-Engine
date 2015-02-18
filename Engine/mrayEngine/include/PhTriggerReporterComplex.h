

/********************************************************************
	created:	2012/02/16
	created:	16:2:2012   14:20
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\PhTriggerReporterComplex.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	PhTriggerReporterComplex
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __PhTriggerReporterComplex__
#define __PhTriggerReporterComplex__

#include "IPhysicalTriggerReporter.h"

namespace mray
{
namespace physics
{

class PhTriggerReporterComplex:public IPhysicalTriggerReporter
{
private:
protected:
	typedef std::list<IPhysicalTriggerReporter*> ReportersList;
	ReportersList m_reporters;
public:
	PhTriggerReporterComplex(){}
	virtual~PhTriggerReporterComplex()
	{
		ClearReporters();
	}

	void AddReporter(IPhysicalTriggerReporter*r)
	{
		m_reporters.push_back(r);
	}
	void RemoveReporter(IPhysicalTriggerReporter*r)
	{
		ReportersList::iterator it=m_reporters.begin();
		for(;it!=m_reporters.end();++it)
		{
			if(*it==r)
			{
				m_reporters.erase(it);
				break;
			}
		}
	}
	void ClearReporters()
	{
		m_reporters.clear();
	}
	void OnTrigger(IPhysicalShape* triggerShape,IPhysicalShape* shape,ETriggerStatus status)
	{
		ReportersList::iterator it=m_reporters.begin();
		for(;it!=m_reporters.end();++it)
		{
			(*it)->OnTrigger(triggerShape,shape,status);
		}
	}

};


}
}

#endif

