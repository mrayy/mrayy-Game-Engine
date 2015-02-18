
#include "stdafx.h"
#include "ChannelMap.h"


namespace mray
{


ChannelMap::ChannelMap()
{
}
ChannelMap::~ChannelMap()
{
}

const SingleChannelMap* ChannelMap::GetChannel(const core::string& name)const
{
	std::map<core::string,SingleChannelMap>::const_iterator it= m_channels.find(name);
	if(it==m_channels.end())
		return 0;
	return &it->second;
}
bool ChannelMap::LoadFromXML(xml::XMLElement* e)
{
	xml::XMLElement* elem=e->getSubElement("Channel");
	while(elem)
	{
		SingleChannelMap c;
		if(c.LoadFromXML(elem))
			m_channels[c.name]=c;
		elem=elem->nextSiblingElement("Channel");
	}
	return true;
}

}
