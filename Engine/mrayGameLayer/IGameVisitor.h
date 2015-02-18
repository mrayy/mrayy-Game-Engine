


/********************************************************************
	created:	2012/02/12
	created:	12:2:2012   22:04
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IGameVisitor.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IGameVisitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGameVisitor__
#define __IGameVisitor__


namespace mray
{
namespace game
{
	class GameEntity;
	class IGameComponent;
	class IGameAction;

class IGameVisitor
{
private:
protected:
public:
	IGameVisitor(){}
	virtual~IGameVisitor(){}

	virtual void OnVisit(GameEntity*){};
	virtual void OnVisit(IGameComponent*){};
	virtual void OnVisit(IGameAction*){};
};

}
}

#endif
