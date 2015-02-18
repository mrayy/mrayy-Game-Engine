


#include "stdafx.h"
#include "EffectsScheme.h"
#include "DemoSharedData.h"
#include "DeviceController.h"
#include "TSApplication.h"
#include "OpenNICaptureDevice.h"



namespace mray
{

EffectsScheme::EffectsScheme()
{
}

EffectsScheme::~EffectsScheme()
{
}


void EffectsScheme::PlayRandom(const math::vector2d& pos,float strength)
{
	int i=math::Randomizer::rand(m_effects.size());
	EffectCreatePack p=m_effects[i].pack;

	openni::VideoMode mode= DemoSharedData::Instance()->DeviceController->GetDevice()->GetDepthStream().getVideoMode();
	math::vector2d sz= DemoSharedData::Instance()->App->GetRenderWindow(0)->GetSize();
	
	math::vector2d ratio(sz.x/(float)mode.getResolutionX(),sz.y/(float)mode.getResolutionY());
	p.pos=pos*ratio;
	p.pos.x=DemoSharedData::Instance()->App->GetRenderWindow(0)->GetSize().x-p.pos.x;
	//p.pos.y=DemoSharedData::Instance()->App->GetRenderWindow(0)->GetSize().y-p.pos.y;
	p.params["Strength"]=core::StringConverter::toString(strength);
	DemoSharedData::Instance()->EffectManager->CreateEffect(m_effects[i].effect,&p);
}


void EffectsScheme::LoadFromXML(xml::XMLElement* e)
{
	m_effects.clear();
	xml::XMLElement* elem= e->getSubElement("Effect");
	while(elem)
	{
		InteractEffectData d;
		d.LoadFromXML(elem);
		m_effects.push_back(d);
		elem=elem->nextSiblingElement("Effect");
	}
}

}
