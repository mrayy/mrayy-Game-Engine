
#include "stdafx.h"
#include "Environment.h"
//#include "CPhysXNode.h"
#include "PhysicContactPair.h"
#include "Character.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		Environment::Environment()
		{
			phMngr=0;
			//phMngr= dynamic_cast<physics::PhysXManager*>(_phMngr);;
			//phMngr->getPhysXScene()->setUserContactReport(this);
		}
		Environment::~Environment()
		{
			OnSetPhysicsManager(0);
			contactMap.clear();
			m_ground.clear();
		}
		void Environment::OnSetPhysicsManager(physics::IPhysicManager*m)
		{
			if(phMngr!=m )
			{
				if(phMngr!=0)
					phMngr->RemoveContactReport(this);
			}else
			{
				return;
			}
			phMngr=m;
			if(phMngr)
				phMngr->AddContactReport(this);
		}
		void Environment::addGrounds(std::list<physics::IPhysicalNode*>& ground)
		{
			for(std::list<physics::IPhysicalNode*>::iterator it=ground.begin();it!=ground.end();++it)
				addGroundNode(*it);
		}
		void Environment::addGroundNode(physics::IPhysicalNode* ground)
		{
			m_ground.push_back(ground);
		}

		void Environment::registerPair(ActorPair& pair )
		{
			std::map<ActorPair, ContactData, ActorPairComparator>::iterator pos = contactMap.find(pair);
			if (pos == contactMap.end())
			{			
				ContactData contactData;
				contactMap[pair] = contactData;
				pos = contactMap.find(pair);
				phMngr->SetActorPairFlag(pair.m_first, pair.m_second, physics::ECRF_Touch,true);
				//phMngr->getPhysXScene()->setActorPairFlags(*pair.m_first, *pair.m_second, NX_NOTIFY_ON_TOUCH);
			}
			pos->second.numOfRegisters++;
		}

		void Environment::unRegisterPair(ActorPair& pair )
		{
			std::map<ActorPair, ContactData,ActorPairComparator>::iterator pos = contactMap.find(pair);
			pos->second.numOfRegisters--;
			if (pos->second.numOfRegisters == 0)
			{
				phMngr->SetActorPairFlag(pair.m_first, pair.m_second, physics::ECRF_Touch,false);
				//phMngr->getPhysXScene()->setActorPairFlags(*pair.m_first, *pair.m_second, NX_IGNORE_PAIR);
				contactMap.erase(pos);
			}
		}

		bool Environment::isContacted( ActorPair& pair )
		{
			std::map<ActorPair, ContactData,ActorPairComparator>::iterator pos = contactMap.find(pair);
			return pos->second.isContacted;
		}

		void Environment::OnPairTouch(physics::PhysicContactPair* pair)
		{
			if(pair->nodes[0]->GetUserData()!=pair->nodes[1]->GetUserData())
			{
				//check if any is a Character
				Character* c=0;
				bool isChar=false;
				c=dynamic_cast<Character*>(pair->nodes[0]->GetUserData());
				if(c)
				{
					isChar=true;
					c->OnContact(pair->nodes[1]);
				}
				c=dynamic_cast<Character*>(pair->nodes[1]->GetUserData());
				if(c)
				{
					isChar=true;
					c->OnContact(pair->nodes[0]);
				}

				ActorPair myPair(pair->nodes[0],pair->nodes[1]);
				std::map<ActorPair, ContactData,ActorPairComparator>::iterator pos = contactMap.find(myPair);
				if(pos!=contactMap.end())
					pos->second.isContacted = true;

			}
		}

		float Environment::getHeightToGround( const math::vector3d& pos )
		{
			math::Ray3d worldRay;
			worldRay.Dir.set(0,-1,0);
			worldRay.Start.set(pos.x,pos.y,pos.z);
			physics::PhysicsRaycaseHit hit;
			phMngr->RaycastClosestShape(worldRay, physics::EShape_Fixed, hit);
			return hit.distance;
		}

		void Environment::update( float dt )
		{
			for (std::map<ActorPair, ContactData,ActorPairComparator>::iterator it = contactMap.begin();it!=contactMap.end();++it)
			{
				(it)->second.isContacted = false;
			}
		}

		math::vector3d Environment::getGroundNormal(const math::vector3d& pos )
		{
			math::Ray3d worldRay;
			worldRay.Dir.set(0,-1,0);
			worldRay.Start.set(pos.x,pos.y,pos.z);
			physics::PhysicsRaycaseHit hit;
			phMngr->RaycastClosestShape(worldRay, physics::EShape_Fixed, hit);
			return hit.worldNormal;
		}

		void Environment::registerActor( physics::IPhysicalNode* actor )
		{
			for (std::list<physics::IPhysicalNode*>::iterator it=m_ground.begin();it!=m_ground.end();++it)
			{
				ActorPair pair(actor, (*it));
				registerPair(pair);
			}
		}

		void Environment::unRegisterActor( physics::IPhysicalNode* actor )
		{
			for (std::list<physics::IPhysicalNode*>::iterator it=m_ground.begin();it!=m_ground.end();++it)
			{
				ActorPair pair(actor, (*it));
				unRegisterPair(pair);
			}
		}


		bool Environment::isContacted( physics::IPhysicalNode* actor )
		{
			for (std::list<physics::IPhysicalNode*>::iterator it=m_ground.begin();it!=m_ground.end();++it)
			{
				ActorPair pair(actor, (*it));
				if(isContacted(pair))
					return true;
			}
			return false;
		}
	}
}