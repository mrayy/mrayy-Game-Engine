

/********************************************************************
	created:	2011/06/06
	created:	6:6:2011   13:32
	filename: 	d:\Development\mrayEngine\Projects\BallanceBall\IntroGameState.h
	file path:	d:\Development\mrayEngine\Projects\BallanceBall
	file base:	IntroGameState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IntroGameState__
#define __IntroGameState__

#include "IGameState.h"

namespace mray
{
namespace Ball
{

class IntroGameState:public IGameState
{
private:
protected:
public:
	IntroGameState();
	virtual~IntroGameState();

	virtual void OnEvent(Event*e);

	virtual void Enter(IGameState*prev);
	virtual void Update(float dt);
	virtual void Exit();
};


}
}

#endif
