/********************************************************************
	created:	2010/06/22
	created:	22:6:2010   9:39
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\HumanGait.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	HumanGait
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef HumanGait_h__
#define HumanGait_h__
#include "PoseModifier.h"
#include "HumanGait_BalanceParams.h"
#include "GaitParams.h"
#include "../PhysicalModel/Segment.h"
#include "../Environment.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class HumanGait:public PoseModifier
		{
			public:
				HumanGait();
				virtual~HumanGait();
				virtual void load(xml::XMLElement*elem, Character* character);
				virtual BalanceParams* createBalanceParams(xml::XMLElement*elem);
				virtual void setBalanceParams(BalanceParams* params);
				virtual void setControllingParams(ControllingParams* params);
				virtual void modifyTargetPose(float dt);
			protected:
				Segment* m_root;
				Segment* m_leftHip;
				Segment* m_rightHip;
				Segment* m_leftAnkle;
				Segment* m_rightAnkle;
				Environment* m_environement;
				HumanGait_BalanceParams* m_balanceParams;
				GaitParams* m_controllingParams;
				bool flag;
		};
	}
}
#endif // HumanGait_h__