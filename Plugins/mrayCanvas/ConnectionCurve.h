

/********************************************************************
	created:	2011/12/01
	created:	1:12:2011   9:53
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\ConnectionCurve.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	ConnectionCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ConnectionCurve__
#define __ConnectionCurve__

#include "CanvasCommon.h"
#include "IConnectionCurve.h"

namespace mray
{
namespace canvas
{

class ConnectionCurve:public IConnectionCurve
{
private:
protected:

	PointList m_points;
	bool m_dirty;

	math::vector2d m_lastSourcePos;
	math::vector2d m_lastDestPos;

	virtual bool _updateCurve();

	bool m_selected;

public:
	ConnectionCurve(CanvasManager*m);
	virtual~ConnectionCurve();

	virtual void SetSource(IConnector* s);
	virtual void SetDestination(IConnector* s);

	virtual void Draw();

	virtual void ReceivedFocus(){m_selected=true;}
	virtual void LostFocus(){m_selected=false;}

	virtual EMouseStatus ProcessMouse(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);
};


}
}

#endif
