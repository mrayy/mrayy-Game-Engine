
/********************************************************************
	created:	2014/02/12
	created:	12:2:2014   20:49
	filename: 	C:\Development\mrayEngine\Plugins\WiimoteController\CWiimote.h
	file path:	C:\Development\mrayEngine\Plugins\WiimoteController
	file base:	CWiimote
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CWiimote__
#define __CWiimote__

#include "wiimote.h"

namespace mray
{
namespace controllers
{

class CWiimote :public wiimote
{
protected:
public:
	CWiimote(){}
	virtual~CWiimote(){}
};

}
}


#endif
