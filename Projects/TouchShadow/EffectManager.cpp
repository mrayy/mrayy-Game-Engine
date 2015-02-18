

#include "stdafx.h"
#include "EffectManager.h"
#include "IdleShapesEffect.h"
#include "SparkleEffect.h"

namespace mray
{

EffectManager::EffectManager()
{
	AddFactory(new IdleShapesEffectFactory());
	AddFactory(new SparkleEffectFactory());
}

EffectManager::~EffectManager()
{
	ClearEffects();
}


void EffectManager::AddFactory(IEffectFactory* fact)
{
	m_factories[fact->GetType()]=fact;
}

void EffectManager::AddEffect(IEffect* eff)
{
	m_effects.push_back(eff);
}


IEffect* EffectManager::CreateEffect(const core::string& type,EffectCreatePack* desc)
{
	std::map<core::string,IEffectFactory*>::iterator it= m_factories.find(type);
	if(it==m_factories.end())
		return 0;
	IEffect*e= it->second->CreateEffect(desc);
	if(e)
		m_effects.push_back(e);
	return e;
}

void EffectManager::Draw(const math::rectf& rc)
{
	for(std::list<IEffect*>::iterator it=m_effects.begin();it!=m_effects.end();++it)
	{
		(*it)->Draw(rc);
	}

}

void EffectManager::Update(float dt)
{
	std::list<IEffect*>::iterator it,it2;
	it=m_effects.begin();
	while(it!=m_effects.end())
	{
		(*it)->Update(dt);
		if((*it)->IsDone())
		{
			it2=it;
			++it2;
			delete *it;
			m_effects.erase(it);
			it=it2;
		}else
			++it;
	}
}


void EffectManager::ClearEffects()
{	
	for(std::list<IEffect*>::iterator it=m_effects.begin();it!=m_effects.end();++it)
	{
		delete *it;
	}
}

}
