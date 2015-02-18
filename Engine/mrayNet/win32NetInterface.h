
/********************************************************************
	created:	2009/05/24
	created:	24:5:2009   23:43
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayNet\win32NetInterface.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayNet
	file base:	win32NetInterface
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___win32NetInterface___
#define ___win32NetInterface___

#include "INetwork.h"
#include "CompileConfig.h"

namespace mray{
namespace network{

	MRAY_NET_DLL INetwork* createWin32Network();

}
}


#endif //___win32Network___
