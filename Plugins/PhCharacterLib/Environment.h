/********************************************************************
	created:	2010/05/24
	created:	24:5:2010   2:24
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Environment.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter
	file base:	Environment
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef Environment_h__
#define Environment_h__

//#include "PhysXManager.h"
#include "compileConfig.h"
#include "IPhysicUserContactReport.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class ActorPair
		{
		public:
			ActorPair(physics::IPhysicalNode* first, physics::IPhysicalNode* second)
			{
				if (first>second)
				{
					m_first = first;
					m_second = second;
				}
				else
				{
					m_first = second;
					m_second = first;
				}
			}
			
			physics::IPhysicalNode* m_first;
			physics::IPhysicalNode* m_second;
		};
		class ActorPairComparator
		{
		public:
			bool operator()(ActorPair a, ActorPair b) const
			{
				if (a.m_first == b.m_first)
					if (a.m_second < b.m_second)
						return true;
					else
						return false;
				else if (a.m_first < b.m_first)
					return true;
				else
					return false;
			}
		};

		struct ContactData
		{
		public:
			ContactData()
			{
				isContacted =false;
				numOfRegisters = 0;
			}
			bool isContacted;
			int numOfRegisters;
		};
		class MRAY_PH_CHARACTER_DLL Environment: public physics::IPhysicUserContactReport
		{
		public:
			Environment();
			~Environment();
			void addGrounds(std::list<physics::IPhysicalNode*>& ground);
			void addGroundNode(physics::IPhysicalNode* ground);
			void registerActor(physics::IPhysicalNode* actor);
			void unRegisterActor(physics::IPhysicalNode* actor);
			bool isContacted(physics::IPhysicalNode* actor);
			//void onContactNotify(NxContactPair& pair, NxU32 events);

			void OnSetPhysicsManager(physics::IPhysicManager*phMngr);

			void OnPairTouch(physics::PhysicContactPair* pair);

			float getHeightToGround(const math::vector3d& pos);
			math::vector3d getGroundNormal(const math::vector3d& pos);
			void update(float dt);
		protected:
			physics::IPhysicManager* phMngr;
			std::list<physics::IPhysicalNode*> m_ground;
			std::map<ActorPair, ContactData, ActorPairComparator> contactMap;

			void registerPair(ActorPair& pair);
			void unRegisterPair(ActorPair& pair);
			bool isContacted(ActorPair& pair);
		private:
		};
	}
}
#endif // Environment_h__