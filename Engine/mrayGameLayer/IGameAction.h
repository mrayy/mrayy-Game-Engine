

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   16:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\IGameAction.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	IGameAction
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGameAction_h__
#define IGameAction_h__

#include "CompileConfig.h"
#include "IGameComponent.h"
#include <IValue.h>
#include <map>

namespace mray
{
namespace game
{

	class GameEntity;
	class IGameVisitor;

class MRAY_GAME_DLL IGameAction:public IGameComponent
{
private:
protected:
	mray::ValuesMap m_parameters;

	void AddAttribute(IValue*v);
public:
	IGameAction();
	virtual ~IGameAction();
	
	//const core::string& GetName()const{return m_name;}

	virtual const ValuesMap& GetAttributes();
	virtual IValue* GetAttribute(const core::string&name);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* elem);

	void Visit(IGameVisitor* visitor);
};

}
}

#endif // IGameAction_h__
