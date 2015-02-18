/********************************************************************
	created:	2010/06/28
	created:	28:6:2010   12:51
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\CharacterGait.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter
	file base:	CharacterGait
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef CharacterGait_h__
#define CharacterGait_h__

#include "./PhysicalModel/Segment.h"

#include "CompileConfig.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class Character;
		class Environment;
		class GaitParams;
		class ActorPair;
		class MRAY_PH_CHARACTER_DLL CharacterGait
		{
			public:
				CharacterGait(xml::XMLElement*elem, Character* character);
				~CharacterGait();

				virtual float	GetMass();

				virtual void SetGlobalPosition(const math::vector3d&pos);
				virtual math::vector3d GetGlobalPosition();

				virtual void SetGlobalOrintation(const math::quaternion&or);
				virtual math::quaternion GetGlobalOrintation();

				virtual void SetLinearDamping(float d){};
				virtual float GetLinearDamping(){return 0;};

				virtual void SetLinearVelocity(const math::vector3d&v);
				virtual math::vector3d GetLinearVelocity();

				virtual void SetAngularVelocity(const math::vector3d& v);
				virtual math::vector3d GetAngularVelocity();

				virtual void SetMaxAngularVelocity(float m){};
				virtual float GetMaxAngularVelocity();

				virtual void SetMaxSpeed(float s){};
				virtual float GetMaxSpeed();

				virtual math::vector3d GetUp()
				{
					return GetGlobalOrintation()*math::vector3d::YAxis;
				}
				virtual math::vector3d GetHeading()
				{
					return GetGlobalOrintation()*math::vector3d::ZAxis;
				}

				virtual void update(float dt);

			protected:
				core::string resolveSpeed(float speed);
				Character* m_character;
				int m_slot;
				core::string standingController;
				core::string slowController;
				core::string mediumController;
				core::string fastController;
				core::string positioningController;
				core::string gaitController;
				GaitParams* params;
				float maxLinearSpeed, maxAngularSpeed;
				float fallingThreshold;
				float desiredHeight;
				float linearSpeed;

				bool isFalling, isPositioning;
				//ActorPair* ground_root_pair;
				SegmentList segments;

				bool isWalking;
				physics::IPhysicalNode* m_actor;

		};
	}
}
#endif // CharacterGait_h__