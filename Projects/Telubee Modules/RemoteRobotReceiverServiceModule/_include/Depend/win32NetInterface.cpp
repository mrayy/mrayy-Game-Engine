
#include "stdafx.h"
#include "win32NetInterface.h"
#include "Win32Network.h"

namespace mray{
namespace network{

INetwork* createWin32Network(){
	if(Win32Network::isExist())
		return Win32Network::getInstancePtr();
	return new Win32Network();
}

}
}