

#include "stdafx.h"
#include "ContactCollisionDebugger.h"
#include "Engine.h"
#include "DebugDrawManager.h"
#include "GameEntityManager.h"
#include "IPhysicUserContactReport.h"
#include "IPhysicContactStreamIterator.h"
#include "PhysicContactPair.h"


namespace mray
{
namespace VT
{


class ContactCollisionDebuggerImpl:public physics::IPhysicUserContactReport
{
protected:
	scene::DebugDrawManager* m_contactDebugger;
	scene::DebugDrawManager* m_touchDebugger;
	int m_lastContactsCount;
	float m_totalForce;
public:
	ContactCollisionDebuggerImpl()
	{
		m_contactDebugger=new scene::DebugDrawManager(Engine::getInstance().getDevice());
		m_touchDebugger=new scene::DebugDrawManager(Engine::getInstance().getDevice());
		m_lastContactsCount=0;
		m_totalForce=0;
	}
	virtual~ContactCollisionDebuggerImpl()
	{
		delete m_contactDebugger;
		delete m_touchDebugger;
	}
	void Init(game::GameEntityManager* mngr)
	{
		mngr->GetPhysicsManager()->AddContactReport(this);
	}

	void Draw(scene::IViewNode* cam)
	{
		m_contactDebugger->StartDraw(cam);
		m_touchDebugger->StartDraw(cam);

		m_contactDebugger->EndDraw();
		m_touchDebugger->EndDraw();
	}


	virtual void _processPair(physics::PhysicContactPair* pair)
	{
		const physics::IPhysicContactStreamIterator::PairsInfoList& pairs= pair->contactStream->GetPairs();
		physics::IPhysicContactStreamIterator::PairsInfoList::const_iterator it=pairs.begin();
		for(;it!=pairs.end();++it)
		{

			physics::IPhysicalShape* shape;
			shape=(*it).s1;
			const physics::IPhysicContactStreamIterator::PatchInfoList& patchList=(*it).patches;
			physics::IPhysicContactStreamIterator::PatchInfoList::const_iterator patchIt=patchList.begin();

			for(;patchIt!=patchList.end();++patchIt)
			{
				const physics::IPhysicContactStreamIterator::PointsInfoList& pointhList=(*patchIt).points;
				physics::IPhysicContactStreamIterator::PointsInfoList::const_iterator pointIt=(*patchIt).points.begin();

				for(;pointIt!=pointhList.end();++pointIt)
				{
					if((*pointIt).force<0.05)
						continue;
					++m_lastContactsCount;
					m_totalForce+=(*pointIt).force;
					//m_contactDebugger->AddCross((*pointIt).point,(*pointIt).force,video::SColor(0,1,0,1),2);
					m_contactDebugger->AddLine((*pointIt).point,(*pointIt).point+(*patchIt).normal*(*pointIt).force,video::SColor(1,0,0,1),2,2);
				}
			}
		}
	}
	void Update(float dt)
	{
		m_totalForce=0;
		m_lastContactsCount=0;
		m_contactDebugger->Update(dt);
	}
	virtual void OnPairTouch(physics::PhysicContactPair* pair){}
	virtual void OnPairStartTouch(physics::PhysicContactPair* pair){}
	virtual void OnPairEndTouch(physics::PhysicContactPair* pair){}
	virtual void OnPairImpact(physics::PhysicContactPair* pair){}
	virtual void OnPairRoll(physics::PhysicContactPair* pair){}
	virtual void OnPairSlide(physics::PhysicContactPair* pair){}

	virtual void OnPairStartTouchForceThreshold(physics::PhysicContactPair* pair){_processPair(pair);}
	virtual void OnPairTouchForceThreshold(physics::PhysicContactPair* pair){_processPair(pair);}
	virtual void OnPairEndTouchForceThreshold(physics::PhysicContactPair* pair){}

	int GetLastContactCount(){return m_lastContactsCount;}
	float GetAverageForce()
	{
		if(m_lastContactsCount==0)
			return 0;
		return m_totalForce/(float)m_lastContactsCount;
	}
};

ContactCollisionDebugger::ContactCollisionDebugger()
{
	m_impl=new ContactCollisionDebuggerImpl();
}
ContactCollisionDebugger::~ContactCollisionDebugger()
{
	delete m_impl;
}

void ContactCollisionDebugger::Init(game::GameEntityManager* mngr)
{
	m_impl->Init(mngr);
}

void ContactCollisionDebugger::Draw(scene::IViewNode* cam)
{
	m_impl->Draw(cam);
}
void ContactCollisionDebugger::Update(float dt)
{
	m_impl->Update(dt);
}

int ContactCollisionDebugger::GetLastContactCount()
{
	return m_impl->GetLastContactCount();
}

float ContactCollisionDebugger::GetAverageForce()
{
	return m_impl->GetAverageForce();
}

}
}


