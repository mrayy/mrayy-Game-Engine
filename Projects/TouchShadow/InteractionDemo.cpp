

#include "stdafx.h"
#include "InteractionDemo.h"
#include "XMLTree.h"
#include "UserFrameSchema.h"
#include "ChannelMap.h"
#include "TextureResourceManager.h"
#include "InteractionRule.h"
#include "EffectsScheme.h"
#include "AudioWaveSet.h"
#include "AreaInteraction.h"
#include "ToolObject.h"

namespace mray
{

InteractionDemo::InteractionDemo()
{
	m_imageTexture=0;
	m_userSchema=new UserFrameSchema;
	m_channels=new ChannelMap;
	//m_rules=new InteractionRules();
	m_effects=new EffectsScheme();
	m_audioWave=new AudioWaveSet;
	m_areaInteraction=new AreaInteraction();

	m_tool=new ToolObject;
}

InteractionDemo::~InteractionDemo()
{
	delete m_channels;
	delete m_userSchema;
	//delete m_rules;
	delete m_effects;
	delete m_audioWave;
	delete m_areaInteraction;
	delete m_tool;
}


void InteractionDemo::Run()
{
	//m_rules->RunRules();
	if(m_areaInteraction->CheckAreas())
		m_tool->Run();
	else
		m_tool->Stop();
}

void InteractionDemo::Start()
{
	m_tool->Open();
}
void InteractionDemo::Stop()
{
	m_tool->Close();
}

void InteractionDemo::LoadFromXML(xml::XMLElement* e)
{
	m_name=e->getValueString("Name");
	m_image=e->getValueString("Image");
	m_imageTexture=gTextureResourceManager.loadTexture2D(m_image);

	xml::XMLElement* elem=e->getSubElement("UserFrame");
	if(elem)
		m_userSchema->LoadFromXML(elem);
	elem=e->getSubElement("Audio");
	if(elem)
		m_audioWave->LoadFromXML(elem);
	elem=e->getSubElement("ChannelMap");
	if(elem)
		m_channels->LoadFromXML(elem);
	elem=e->getSubElement("Effects");
	if(elem)
		m_effects->LoadFromXML(elem);
	elem=e->getSubElement("Tool");
	if(elem)
		m_tool->LoadFromXML(elem);
	elem=e->getSubElement("Interactions");
	if(elem)
		m_areaInteraction->LoadFromXML(elem,this);
	/*elem=e->getSubElement("Interactions");
	if(elem)
		m_rules->LoadFromXML(elem);*/
}

bool InteractionDemo::LoadFromXML(const core::string& path)
{
	xml::XMLTree t;
	if(!t.load(path))
		return false;
	xml::XMLElement* e=t.getSubElement("Demo");
	if(!e)
		return false;
	LoadFromXML(e);
	return true;
}

}

