

/********************************************************************
	created:	2011/12/02
	created:	2:12:2011   23:34
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\DirectedConnector.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	DirectedConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __DirectedConnector__
#define __DirectedConnector__

#include "ConnectionCurve.h"

namespace mray
{
namespace canvas
{

class DirectedConnector:public ConnectionCurve
{
private:
protected:
	virtual bool _updateCurve();
public:
	DirectedConnector(CanvasManager*m);
	virtual~DirectedConnector();

};


}
}

#endif
