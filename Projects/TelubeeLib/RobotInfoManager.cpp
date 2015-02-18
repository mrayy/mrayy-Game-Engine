

#include "stdafx.h"
#include "RobotInfoManager.h"

#include "XMLTree.h"

namespace mray
{
namespace TBee
{
class RobotInfoManagerImpl
{
public:
	std::vector<TBRobotInfo> robots;
	std::map<int,int> robotsMap;
};

RobotInfoManager::RobotInfoManager()
{
	m_impl=new RobotInfoManagerImpl();
}
RobotInfoManager::~RobotInfoManager()
{
	delete m_impl;
}

void RobotInfoManager::AddRobotInfo(const TBRobotInfo& ifo)
{
	m_impl->robots.push_back(ifo);
	m_impl->robotsMap[ifo.ID]=m_impl->robots.size()-1;
	FIRE_LISTENR_METHOD(OnRobotAdded,(this,ifo));
}

TBRobotInfo* RobotInfoManager::GetRobotInfo(int index)
{
	if(index>=m_impl->robots.size())
		return 0;
	return &m_impl->robots[index];
}
TBRobotInfo* RobotInfoManager::GetRobotInfoByID(int id)
{
	std::map<int,int>::iterator it= m_impl->robotsMap.find(id);
	if(it==m_impl->robotsMap.end())
		return 0;
	return &m_impl->robots[it->second];
}

const std::vector<TBRobotInfo>& RobotInfoManager::GetRobots()
{
	return m_impl->robots;
}

void RobotInfoManager::ClearRobots()
{
	m_impl->robots.clear();
}
void RobotInfoManager::LoadRobots(const core::string& path)
{
	xml::XMLTree tree;
	if (!tree.load(path))
		return;

	xml::XMLElement* root= tree.getSubElement("Robots");
	xml::XMLElement* e = root->getSubElement("Robot");
	while (e)
	{
		TBRobotInfo ifo;
		ifo.IP = e->getValueString("IP");
		ifo.name = e->getValueString("Name");
		ifo.Location = e->getValueString("Location");
		AddRobotInfo(ifo);
		e = e->nextSiblingElement("Robot");
	}

}


}
}

