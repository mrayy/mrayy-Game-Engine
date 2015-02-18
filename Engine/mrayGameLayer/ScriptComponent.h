

/********************************************************************
	created:	2012/02/13
	created:	13:2:2012   0:41
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\ScriptComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	ScriptComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ScriptComponent__
#define __ScriptComponent__

#include "IGameComponent.h"
#include "IScript.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL ScriptComponent:public IGameComponent
{
	DECLARE_RTTI;
private:
protected:
	script::IScript* m_node;
public:
	ScriptComponent(GameEntityManager*):m_node(0){}
	ScriptComponent(script::IScript* node):m_node(node)
	{}
	virtual~ScriptComponent(){}

	script::IScript* GetScript(){return m_node;}
	void SetScript(script::IScript* node){m_node=node;}

	virtual IObject* GetAttachedObject(){return m_node;}

};
DECLARE_GAMECOMPONENT_FACTORY(ScriptComponent);


}
}

#endif

