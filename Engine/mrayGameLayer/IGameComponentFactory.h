

/********************************************************************
	created:	2012/02/27
	created:	27:2:2012   9:26
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IGameComponentFactory.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IGameComponentFactory
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGameComponentFactory__
#define __IGameComponentFactory__


namespace mray
{
namespace game
{

	class GameEntityManager;
	class IGameComponent;

class IGameComponentFactory
{
private:
protected:
public:
	IGameComponentFactory(){}
	virtual~IGameComponentFactory(){}

	virtual const core::string& GetType()const=0;
	virtual IGameComponent* CreateComponent(GameEntityManager*mngr)=0;

};

#define DECLARE_GAMECOMPONENT_FACTORY(Type)\
class GameComponentFactory##Type:public mray::game::IGameComponentFactory\
{\
public:\
	static const core::string s_ComponentType;\
	static GameComponentFactory##Type Instance;\
	const core::string& GetType()const{return s_ComponentType;}\
	mray::game::IGameComponent* CreateComponent(mray::game::GameEntityManager*mngr);\
};

#define IMPLEMENT_GAMECOMPONENT_FACTORY(Type)\
GameComponentFactory##Type GameComponentFactory##Type::Instance;\
const core::string GameComponentFactory##Type::s_ComponentType=Type::getClassType();\
mray::game::IGameComponent* GameComponentFactory##Type::CreateComponent(mray::game::GameEntityManager*mngr)\
{\
	return new Type(mngr);\
}


}
}

#endif

