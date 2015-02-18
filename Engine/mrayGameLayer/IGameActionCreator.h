

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   16:56
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\IGameActionCreator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	IGameActionCreator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	interface responsible to create game actions 
*********************************************************************/

#ifndef IGameActionCreator_h__
#define IGameActionCreator_h__

#include "mstring.h"
#include "ObjectFactory.h"

namespace mray
{
namespace game
{

	class IGameAction;

class IGameActionCreator:public ObjectFactory<IGameAction>
{
private:
protected:
	core::string m_type;
public:
	IGameActionCreator(const core::string&type):m_type(type){}
	virtual ~IGameActionCreator(){}

	virtual const core::string& GetType(){return m_type;}
	virtual core::string GetDescription(){return mT("No description avaliable.");}

};

}
}

#endif // IGameActionCreator_h__
