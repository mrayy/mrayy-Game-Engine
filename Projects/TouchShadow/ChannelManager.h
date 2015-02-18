

/********************************************************************
	created:	2013/08/30
	created:	30:8:2013   18:04
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\ChannelManager.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	ChannelManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ChannelManager__
#define __ChannelManager__




namespace mray
{
	class OutputChannel;

	class CChannel
	{
	protected:
		int m_channelID;
		int m_targetInterface;//interface index
		int m_portID; //left/right

		OutputChannel* m_outChannel;
	public:
		CChannel():m_outChannel(0)
		{
		}

		int GetID(){return m_channelID;}
		int GetTargetInterface(){return m_targetInterface;}
		int GetPortID(){return m_portID;}

		OutputChannel* GetChannel(){return m_outChannel;}
		void LoadFromXML(xml::XMLElement* e);

	};
class ChannelManager
{
protected:
	std::map<int,CChannel*> m_channels;
public:
	ChannelManager();
	virtual~ChannelManager();

	CChannel* GetChannel(int id);

	void LoadFromXML(const core::string& path);
};

}


#endif
