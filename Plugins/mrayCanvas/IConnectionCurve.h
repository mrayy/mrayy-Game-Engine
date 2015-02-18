

/********************************************************************
	created:	2011/12/01
	created:	1:12:2011   9:16
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\IConnectionCurve.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	IConnectionCurve
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IConnectionCurve__
#define __IConnectionCurve__

#include <MouseEvent.h>

namespace mray
{
namespace canvas
{
	class IConnector;
	class CanvasManager;

class IConnectionCurve
{
private:
protected:
	IConnector* m_source;
	IConnector* m_destination;
	CanvasManager* m_canvasManager;
public:
	IConnectionCurve(CanvasManager*m):m_canvasManager(m),m_source(0),m_destination(0)
	{
	}
	virtual~IConnectionCurve(){}

	void SetCanvasManager(CanvasManager*m){m_canvasManager=m;}
	CanvasManager* GetCanvasManager(){return m_canvasManager;}

	virtual void SetSource(IConnector* s){m_source=s;}
	IConnector* GetSource(){return m_source;}

	virtual void SetDestination(IConnector* s){m_destination=s;}
	IConnector* GetDestination(){return m_destination;}

	virtual void Draw()=0;

	virtual void ReceivedFocus(){}
	virtual void LostFocus(){}

	virtual EMouseStatus ProcessMouse(const math::vector2d&pt,MOUSE_EVENT_TYPE evt)=0;
};


}
}

#endif

