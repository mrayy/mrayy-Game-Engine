

/********************************************************************
	created:	2013/06/04
	created:	4:6:2013   15:22
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\IMovableComponent.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IMovableComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IMovableComponent__
#define __IMovableComponent__


#include "IGameComponent.h"

namespace mray
{
namespace scene
{
	class IMovable;
}
namespace game
{


class MRAY_GAME_DLL IMovableComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:
public:
	IMovableComponent()

	{}
	virtual~IMovableComponent()
	{}

	virtual scene::IMovable* GetMovableObject()=0;

};

}
}


#endif
