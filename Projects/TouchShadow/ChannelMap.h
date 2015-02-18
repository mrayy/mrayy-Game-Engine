

/********************************************************************
	created:	2013/09/08
	created:	8:9:2013   0:52
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\ChannelMap.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	ChannelMap
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ChannelMap__
#define __ChannelMap__




namespace mray
{

	struct SingleChannelMap
	{
	public:
		core::string name;
		int channelPort;

		bool LoadFromXML(xml::XMLElement* e)
		{
			name=e->getValueString("name");
			channelPort=e->getValueInt("Port");
			return true;
		}
	};

class ChannelMap
{
protected:
	std::map<core::string,SingleChannelMap> m_channels;
public:
	ChannelMap();
	virtual~ChannelMap();

	const SingleChannelMap* GetChannel(const core::string& name)const;
	const std::map<core::string,SingleChannelMap>& GetChannels()const{return m_channels;}

	void Clear(){m_channels.clear();}
	bool LoadFromXML(xml::XMLElement* e);

};

}


#endif
