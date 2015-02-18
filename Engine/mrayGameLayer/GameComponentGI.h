

/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   15:20
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\GameComponentGI.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	GameComponentGI
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GameComponentGI__
#define __GameComponentGI__

#include "IGhostInterface.h"


namespace mray
{
namespace game
{
	class IGameComponent;

class GameComponentGI:public IGhostInterface
{
protected:
	IGameComponent* m_owner;
public:
	GameComponentGI(IGameComponent* owner);
	virtual~GameComponentGI();

	IGameComponent* GetOwner(){return m_owner;}

	virtual void CreateWrite(OS::StreamWriter* stream);
};

}
}


#endif
