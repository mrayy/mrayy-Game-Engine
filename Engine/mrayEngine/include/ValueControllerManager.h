

/********************************************************************
	created:	2014/03/03
	created:	3:3:2014   15:52
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\ValueControllerManager.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	ValueControllerManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ValueControllerManager__
#define __ValueControllerManager__


#include "ISingleton.h"

namespace mray
{

class ValueControllerManager:public ISingleton<ValueControllerManager>
{
protected:
public:
	ValueControllerManager();
	virtual~ValueControllerManager();
};

}


#endif
