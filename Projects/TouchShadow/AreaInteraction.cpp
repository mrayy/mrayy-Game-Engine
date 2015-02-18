
#include "stdafx.h"
#include "AreaInteraction.h"
#include "AudioWaveSet.h"
#include "DemoSharedData.h"
#include "InteractionDemo.h"

#include "UserFrame.h"
#include "ChannelMap.h"
#include "AudioSoundInterface.h"
#include "ChannelManager.h"
#include "EffectsScheme.h"
#include "TSApplication.h"

namespace mray
{




	float AreaInteraction::AreaWeight::Calculate(float threshold)
	{
		UserFrame* f= DemoSharedData::Instance()->ActiveUserFrame;
		if(!f)
			return 0;
		InteractionArea* a= f->GetArea(areaName);
		if(!a)
			return 0;
		if(a->intersectionAmount*100>threshold)
			return a->intersectionAmount*weight;
		return 0;
	}
	void AreaInteraction::AreaWeight::LoadFromXML(xml::XMLElement* elem)
	{
		areaName=elem->getValueString("Name");
		weight=elem->getValueFloat("Weight")*0.01f;
	}


	bool AreaInteraction::ChannelArea::Calculate(float threshold)
	{
		float w=0;
		float time=gTimer.getSeconds();
		for (int i=0;i<areas.size();++i)
		{
			float x=areas[i].Calculate(threshold);

			if(x>0 && (time-areas[i].lastPlayedEff)>100)
			{
				areas[i].lastPlayedEff=time;
				math::vector2d pos;

				UserFrame* f= DemoSharedData::Instance()->ActiveUserFrame;
				InteractionArea* a= f->GetArea(areas[i].areaName);
				pos.x= a->intersectRect.ULPoint.x+math::Randomizer::rand01()*a->intersectRect.getWidth();
				pos.y= a->intersectRect.ULPoint.y+math::Randomizer::rand01()*a->intersectRect.getHeight();
// 				pos.x=DemoSharedData::Instance()->App->GetRenderWindow(0)->GetSize().x-pos.x;
// 				pos.y=DemoSharedData::Instance()->App->GetRenderWindow(0)->GetSize().y-pos.y;
				DemoSharedData::Instance()->ActiveDemo->GetEffects()->PlayRandom(pos,x);
			}

			w+=x;
		}
		w=math::clamp(w,0.0f,1.0f);
		CAudioWave* wave=0;
		if(w>0)
		{/*
			if(!isActive)
				wave=enter;
			else wave=stay;*/
			wave=0;
			isActive=true;

		}else
		{
			if(isActive)
			{
				isActive=false;
				wave=loopWav;
				//wave=leave;
			}
		}

		const SingleChannelMap* cmap= DemoSharedData::Instance()->ActiveDemo->GetChannels()->GetChannel(channelName);
		if(cmap && wave)
		{
			CChannel* c= DemoSharedData::Instance()->ChannelManager->GetChannel(cmap->channelPort);
			if(!c || !c->GetChannel())
				return false;
			//flush first
			c->GetChannel()->FlushScheduledFrames();
			c->GetChannel()->ScheduleFrames(&wave->waveFrames,1);
			c->GetChannel()->GetOwner()->Start();
		//	printf("Playing : Channel:%s , %s , %f\n",channelName.c_str(),wave->name.c_str(),w);

		}
		return w>0;
	}
	void AreaInteraction::ChannelArea::LoadFromXML(xml::XMLElement* elem,InteractionDemo* d)
	{
		core::string loop;
		//enter=elem->getValueString("OnEnter");
		loop=elem->getValueString("Wave");
		//leave=elem->getValueString("OnLeave");

		//m_onEnter= d->GetAudioWave()->GetWave(enter);
		loopWav= d->GetAudioWave()->GetWave(loop);
		//m_onLeave= d->GetAudioWave()->GetWave(leave);

		channelName=elem->getValueString("Channel");
		xml::XMLElement* e=elem->getSubElement("Area");
		while(e)
		{
			AreaWeight a;
			a.LoadFromXML(e);
			areas.push_back(a);
			e=e->nextSiblingElement("Area");
		}
	}

AreaInteraction::AreaInteraction()
{

}
AreaInteraction::~AreaInteraction()
{

}

bool AreaInteraction::CheckAreas()
{
	bool ok=false;
	for (int i=0;i<m_channels.size();++i)
	{
		ok|=m_channels[i].Calculate(m_threashold);
	}
	return ok;
}

void AreaInteraction::LoadFromXML(xml::XMLElement* elem,InteractionDemo* d)
{
	m_threashold=elem->getValueFloat("Threshold");
	xml::XMLElement* e=elem->getSubElement("MapChannel");
	while(e)
	{
		ChannelArea a;
		a.LoadFromXML(e,d);
		m_channels.push_back(a);
		e=e->nextSiblingElement("MapChannel");
	}
}


}


