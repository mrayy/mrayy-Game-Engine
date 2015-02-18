
#include "stdafx.h"
#include "InteractionRuleImpl.h"
#include "DemoSharedData.h"
#include "UserFrame.h"
#include "AudioWaveSet.h"
#include "InteractionDemo.h"
#include "TSApplication.h"
#include "ChannelMap.h"
#include "ChannelManager.h"


namespace mray
{
	
CollectionRulePremise::~CollectionRulePremise()
{
	for (int i=0;i<m_parts.size();++i)
	{
		delete m_parts[i];
	}
}
void CollectionRulePremise::LoadFromXML(xml::XMLElement* elem)
{
	xml::xmlSubElementsMapIT it=elem->getElementsBegin();
	xml::xmlSubElementsMapIT end=elem->getElementsEnd();
	for (;it!=end;++it)
	{
		if((*it)->GetType()==xml::ENT_Element)
		{
			xml::XMLElement* e=dynamic_cast<xml::XMLElement*>(*it);
			IRulePremise* p=Create(e->getName());
			if(p)
			{
				p->LoadFromXML(e);
				m_parts.push_back(p);
			}
		}
	}
}

bool RuleAndPremise::Evalute()
{
	for (int i=0;i<m_parts.size();++i)
	{
		if(!m_parts[i]->Evalute())
			return false;
	}
	return true;
}
bool RuleOrPremise::Evalute()
{
	for (int i=0;i<m_parts.size();++i)
	{
		if(m_parts[i]->Evalute())
			return true;
	}
	return false;
}

bool RuleOnKeyPremise::Evalute()
{
	InputManager* inputMngr= DemoSharedData::Instance()->App->GetInputManager();

	if(inputMngr->getKeyboard()->getKeyState(EKEY_CODE::KEY_SPACE))
	{
		return true;
	}
	return false;
}
void RuleOnKeyPremise::LoadFromXML(xml::XMLElement* elem)
{
	m_key=elem->getValueString("Key");
}

bool RuleAreaTouchingPremise::Evalute()
{
	UserFrame* f= DemoSharedData::Instance()->ActiveUserFrame;
	if(!f)
		return false;
	InteractionArea* a= f->GetArea(m_areaName);
	if(!a)
		return false;
	if(a->intersectionAmount*100>m_threshold)
		return true;
	return false;
}
void RuleAreaTouchingPremise::LoadFromXML(xml::XMLElement* elem)
{
	m_areaName=elem->getValueString("Area");
	m_threshold=elem->getValueFloat("Threshold");
}

//////////////////////////////////////////////////////////////////////////

bool RuleRunChannelResult::Execute()
{
	UserFrame* f= DemoSharedData::Instance()->ActiveUserFrame;
	if(!f)
		return false;
	float vol=0;
	for(int i=0;i<m_inputs.size();++i)
	{

		InteractionArea* a= f->GetArea(m_inputs[i].area);
		if(!a)
			continue;
		vol+=a->intersectionAmount*m_inputs[i].weight;
	}
//	if(!m_stopped)
//		return true;
	m_stopped=false;

	CAudioWave* wave= DemoSharedData::Instance()->ActiveDemo->GetAudioWave()->GetWave(m_audioName);
	if(!wave)
		return false;
	const SingleChannelMap* cmap= DemoSharedData::Instance()->ActiveDemo->GetChannels()->GetChannel(m_channelName);
	if(!cmap)
		return false;
	CChannel* c= DemoSharedData::Instance()->ChannelManager->GetChannel(cmap->channelPort);
	if(!c || !c->GetChannel())
		return false;
	c->GetChannel()->ScheduleFrames(&wave->waveFrames,vol);
	c->GetChannel()->GetOwner()->Start();
	return true;
}
void RuleRunChannelResult::Stop()
{
	m_stopped=true;
}
void RuleRunChannelResult::CInput::LoadFromXML(xml::XMLElement*e)
{
	area=e->getValueString("Area");
	weight=e->getValueFloat("weight");
}

void RuleRunChannelResult::LoadFromXML(xml::XMLElement* elem)
{
	m_channelName=elem->getValueString("Channel");
	m_audioName=elem->getValueString("Audio");
	xml::XMLElement* e= elem->getSubElement("Input");
	while(e)
	{
		CInput inp;
		inp.LoadFromXML(e);
		m_inputs.push_back(inp);
		e=e->nextSiblingElement("Input");
	}
}


bool RulePrintResult::Execute()
{
	if(!m_stopped)return true;
	m_stopped=false;
	printf("Rule Executed: %s\n",m_label.c_str());
	return true;
}
void RulePrintResult::LoadFromXML(xml::XMLElement* elem)
{
	m_label=elem->getValueString("Label");
}

}

