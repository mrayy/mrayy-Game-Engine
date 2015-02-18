/********************************************************************
	created:	2013/01/22
	created:	22:1:2013   13:49
	filename: 	C:\Development\mrayEngine\Engine\mrayGameLayer\SceneComponentGI.h
	file path:	C:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	SceneComponentGI
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SceneComponentGI__
#define __SceneComponentGI__

#include "GameComponentGI.h"


namespace mray
{
namespace game
{

	class SceneComponent;

class SceneComponentGI:public GameComponentGI
{
protected:
public:
	SceneComponentGI(SceneComponent* owner);
	virtual~SceneComponentGI();


	virtual void CreateWrite(OS::StreamWriter* stream);
	virtual void CreateRead(OS::StreamReader* stream);

	virtual void UpdateWrite(OS::StreamWriter* stream);
	virtual void UpdateRead(OS::StreamReader* stream);
};

}
}


#endif

