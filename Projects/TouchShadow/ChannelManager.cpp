
#include "stdafx.h"
#include "ChannelManager.h"
#include "XMLTree.h"

#include "DemoSharedData.h"
#include "AudioSoundInterface.h"

namespace mray
{

	void CChannel::LoadFromXML(xml::XMLElement* e)
	{
		m_channelID=e->getValueInt("ID");
		m_targetInterface=e->getValueInt("Interface");
		m_portID=e->getValueInt("Output");

		AudioSoundInterface* interface= DemoSharedData::Instance()->AudioManager->GetInterface(m_targetInterface);
		if(interface)
		{
			if(m_portID==0)
				m_outChannel=interface->GetLeftChannel();
			else
				m_outChannel=interface->GetRightChannel();
		}
	}

	ChannelManager::ChannelManager()
	{
	}
	ChannelManager::~ChannelManager()
	{
		std::map<int,CChannel*>::iterator it= m_channels.begin();
		for (;it!=m_channels.end();++it)
		{
			delete it->second;
		}
	}

	CChannel* ChannelManager::GetChannel(int id)
	{
		std::map<int,CChannel*>::iterator it= m_channels.find(id);
		if(it==m_channels.end())
			return 0;
		return it->second;
	}
void ChannelManager::LoadFromXML(const core::string& path)
{
	xml::XMLTree tree;
	if(!tree.load(path))
		return;
	xml::XMLElement* e=tree.getSubElement("Channels");
	if(!e)
		return;
	e=e->getSubElement("Channel");
	while(e)
	{
		CChannel* c=new CChannel();
		c->LoadFromXML(e);
		m_channels[c->GetID()]=c;
		e=e->nextSiblingElement("Channel");
	}
}

}


