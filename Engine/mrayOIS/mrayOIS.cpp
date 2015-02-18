
#include "stdafx.h"

#include "mrayOIS.h"
#include "mrayOISInputManager.h"

namespace mray
{

	GCPtr<InputManager> CreateOISInputManager(InputCreationPack&pack)
	{
		return new mrayOISInputManager(pack);
	}

}