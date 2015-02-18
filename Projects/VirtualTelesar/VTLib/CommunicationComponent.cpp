
#include "StdAfx.h"
#include "CommunicationComponent.h"
#include "CommunicatorScheme.h"


namespace mray
{
namespace VT
{

CommunicationComponent::CommunicationComponent(game::GameEntityManager*mngr)
{
	m_scheme=new CommunicatorScheme();
}

CommunicationComponent::~CommunicationComponent()
{
	delete m_scheme;
}


bool CommunicationComponent::InitComponent()
{
	if(!ICommunicatorComponent::InitComponent())
		return false;
	m_scheme->SetName(GetName());

	const std::list<IObjectComponent*>& lst= GetComponentsList();
	std::list<IObjectComponent*>::const_iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		ICommunicatorComponent* c=dynamic_cast<ICommunicatorComponent*>(*it);
		if(c)
		{
			m_commComps.push_back(c);
			m_scheme->AddSubScheme(c->GetScheme());
		}
	}

	return true;
}


CommunicatorScheme* CommunicationComponent::GetScheme()
{
	return m_scheme;
}


void CommunicationComponent::ReceiveData(const const core::string& target,const CommValueList& values)
{
	for (int i=0;i<m_commComps.size();++i)
	{
		if(m_commComps[i]->GetName()==target)
		{
			m_commComps[i]->ReceiveData(target,values);
			break;
		}
	}
}


}
}