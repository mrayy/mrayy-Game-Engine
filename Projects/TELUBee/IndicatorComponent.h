

/********************************************************************
	created:	2013/02/01
	created:	1:2:2013   12:54
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\IndicatorComponent.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	IndicatorComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IndicatorComponent__
#define __IndicatorComponent__

#include "IGameComponent.h"

namespace mray
{
	namespace game
	{
		class SceneComponent;
	}
namespace TBee
{

class IndicatorComponent:public game::IGameComponent
{
	DECLARE_RTTI;
protected:
	float m_time;
	core::string m_nodeName;
	game::SceneComponent* m_scenenode;
	bool m_rotate;
	bool m_translate;
	float m_speed;
public:
	DECLARE_PROPERTY_TYPE(NodeName,core::string,);
	DECLARE_PROPERTY_TYPE(Translate,bool,);
	DECLARE_PROPERTY_TYPE(Rotate,bool,);
	DECLARE_PROPERTY_TYPE(Speed,float,);
	
	DECLARE_SETGET_PROP(NodeName,core::string);
	DECLARE_SETGET_PROP(Rotate,bool);
	DECLARE_SETGET_PROP(Translate,bool);
	DECLARE_SETGET_PROP(Speed,float);
public:
	IndicatorComponent(game::GameEntityManager* mngr);
	virtual~IndicatorComponent();

	virtual bool InitComponent();
	virtual void Update(float dt);
};

DECLARE_GAMECOMPONENT_FACTORY(IndicatorComponent);

}
}

#endif
