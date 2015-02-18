


#include "stdafx.h"
#include "SoundRenderingManager.h"
#include "GameEntityManager.h"
#include "IPhysicManager.h"
#include "IPhysicUserContactReport.h"



namespace mray
{
namespace VT
{


	class SoundRenderingManagerImpl:public physics::IPhysicUserContactReport
	{
	protected:
	public:
		SoundRenderingManagerImpl()
		{
		}
		~SoundRenderingManagerImpl()
		{
		}
		void Init(game::GameEntityManager* mngr)
		{
			mngr->GetPhysicsManager()->AddContactReport(this);
		}


		//////////////////////////////////////////////////////////////////////////


		virtual void OnPairStartTouch(physics::PhysicContactPair* pair){}
		virtual void OnPairTouch(physics::PhysicContactPair* pair){}
		virtual void OnPairEndTouch(physics::PhysicContactPair* pair){}
		virtual void OnPairImpact(physics::PhysicContactPair* pair){}
		virtual void OnPairRoll(physics::PhysicContactPair* pair){}
		virtual void OnPairSlide(physics::PhysicContactPair* pair){}

		virtual void OnPairStartTouchForceThreshold(physics::PhysicContactPair* pair){}
		virtual void OnPairTouchForceThreshold(physics::PhysicContactPair* pair){}
		virtual void OnPairEndTouchForceThreshold(physics::PhysicContactPair* pair){}
	};

SoundRenderingManager::SoundRenderingManager()
{
	m_impl=new SoundRenderingManagerImpl();
}
SoundRenderingManager::~SoundRenderingManager()
{
	delete m_impl;
}


void SoundRenderingManager::Init(game::GameEntityManager* mngr)
{
	m_impl->Init(mngr);
}


}
}