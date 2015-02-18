

/********************************************************************
	created:	2011/06/03
	created:	3:6:2011   14:10
	filename: 	d:\Development\mrayEngine\Projects\BallanceBall\Application.h
	file path:	d:\Development\mrayEngine\Projects\BallanceBall
	file base:	Application
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __Application__
#define __Application__

#include "CMRayApplication.h"

namespace mray
{

class Application:public CMRayApplication
{
private:
protected:
public:
	Application();
	virtual~Application();

	virtual void init(core::array<SOptionElement> *extraOptions);
	virtual void update(float dt);
	virtual void draw();
	virtual void onDone();
};


}

#endif
