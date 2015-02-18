
#include "stdafx.h"
#include "RBSManager.h"
#include "RuleBasedSystem.h"

namespace mray
{
namespace AI
{

RBSManager::RBSManager()
{
}

RBSManager::~RBSManager()
{
	ClearRBSs();
}


void RBSManager::AddRBS(RuleBasedSystem*rbs)
{
	m_rbs[rbs->GetName()]=rbs;
}

RuleBasedSystem* RBSManager::GetRBS(const core::string&name)
{
	RBSMap::iterator it=m_rbs.find(name);
	if(it==m_rbs.end())
		return 0;
	return it->second;
}

void RBSManager::RemoveRBS(const core::string&name)
{
	m_rbs.erase(name);
}


void RBSManager::ClearRBSs()
{
	m_rbs.clear();
}

}
}
