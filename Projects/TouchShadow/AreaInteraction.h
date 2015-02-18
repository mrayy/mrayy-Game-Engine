

/********************************************************************
	created:	2013/09/10
	created:	10:9:2013   19:56
	filename: 	C:\Development\mrayEngine\Projects\TouchShadow\AreaInteraction.h
	file path:	C:\Development\mrayEngine\Projects\TouchShadow
	file base:	AreaInteraction
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AreaInteraction__
#define __AreaInteraction__




namespace mray
{

	class CAudioWave;
	class InteractionDemo;

class AreaInteraction
{
protected:
	struct AreaWeight
	{
	public:

		AreaWeight()
		{
			lastPlayedEff=0;
		}
		core::string areaName;
		float weight;
		void LoadFromXML(xml::XMLElement* elem);

		float Calculate(float threshold);
		float lastPlayedEff;
	};
	class ChannelArea
	{
	public:
		ChannelArea():isActive(false)
		{
		}
		//CAudioWave* m_onEnter;
		CAudioWave* loopWav;
		//CAudioWave* m_onLeave;
		bool isActive;
		core::string channelName;
		std::vector<AreaWeight> areas;
		void LoadFromXML(xml::XMLElement* elem,InteractionDemo* d);
		bool Calculate(float threshold);
	};

	float m_threashold;
	std::vector<ChannelArea> m_channels;
public:
	AreaInteraction();
	virtual~AreaInteraction();

	bool CheckAreas();

	void LoadFromXML(xml::XMLElement* elem,InteractionDemo* d);
};

}


#endif
