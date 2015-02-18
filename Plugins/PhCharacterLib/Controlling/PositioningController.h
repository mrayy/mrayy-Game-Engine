/********************************************************************
	created:	2010/06/29
	created:	29:6:2010   1:42
	filename: 	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling\PositioningController.h
	file path:	d:\Character Animation\Solution\Project\PhysicsBasedCharacter\Controlling
	file base:	PositioningController
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/

#ifndef PositioningController_h__
#define PositioningController_h__
#include "Controller.h"
#include "PositioningParams.h"
#include "../PhysicalModel/Segment.h"
#include "../Character.h"
namespace mray
{
	namespace PhysicsBasedCharacter
	{
		class PositioningController:public Controller
		{
			public:
				PositioningController();
				virtual~PositioningController();
				virtual void load(const core::string& descFile, Character* character);
				virtual void setControllingParams(ControllingParams* params);
				virtual void reset();
				virtual void stop();
				virtual bool isFinished();
				virtual void update(float dt);
				virtual const core::string& getName();	
			protected:
				core::string m_name;
				PositioningParams* m_params;
				Segment* m_root;
				float speed;
				float m_elapsed, m_target;
				SegmentList m_children;
				float m_ks, m_kd;
		};
	}
}
#endif // PositioningController_h__