

/********************************************************************
	created:	2010/05/31
	created:	31:5:2010   22:55
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayOIS\mrayOIS.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayOIS
	file base:	mrayOIS
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef mrayOIS_h__
#define mrayOIS_h__

#include "InputManager.h"
#include "GCPtr.h"

namespace mray
{

	GCPtr<InputManager> CreateOISInputManager(InputCreationPack&pack);

}
#endif // mrayOIS_h__
