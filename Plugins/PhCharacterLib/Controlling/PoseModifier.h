/********************************************************************
	created:	2010/06/18
	created:	18:6:2010   15:28
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\PoseModifier.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	PoseModifier
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef PoseModifier_h__
#define PoseModifier_h__

#include "BalanceParams.h"
#include "ControllingParams.h"

namespace mray
{
	namespace xml
	{
		class XMLElement;
	}

	namespace PhysicsBasedCharacter
	{
		class Character;
		class PoseModifier 
		{
			public:
				PoseModifier(){};
				~PoseModifier(){};
				virtual BalanceParams* createBalanceParams(xml::XMLElement*elem)=0;
				virtual void setBalanceParams(BalanceParams* params)=0;
				virtual void setControllingParams(ControllingParams* params)=0;
				virtual void modifyTargetPose(float dt)=0;
				virtual void load(xml::XMLElement*elem, Character* character)=0;
			protected:
			private:
		};
	}
}
#endif // PoseModifier_h__