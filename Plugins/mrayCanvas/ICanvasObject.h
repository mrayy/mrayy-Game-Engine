


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   12:02
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\ICanvasObject.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	ICanvasObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ICanvasObject__
#define __ICanvasObject__

#include "MouseEvent.h"
#include "IShape.h"
#include "IObject.h"
#include "TypedProperty.h"

namespace mray
{
namespace canvas
{
	class CanvasManager;
	class IConnector;

class ICanvasObject:public IObject
{
	DECLARE_RTTI
private:
protected:

	uint m_id;
	std::vector<IShape*> m_shapes;
	std::vector<IConnector*> m_connectors;
	CanvasManager* m_canvasManager;

	math::vector2d m_pos;
	float m_angle;
	math::rectf m_boundingRect;

	math::vector2d m_originalPos;
	math::vector2d m_originalMousePos;
	EMouseStatus m_status;

	bool m_isDraging;


	void _ProcessDragging(const math::vector2d& pt,MOUSE_EVENT_TYPE evt);
public:
public:
	ICanvasObject();
	virtual~ICanvasObject();

	void SetID(uint id){m_id=id;}
	uint GetID(){return m_id;}

	void SetCanvasManager(CanvasManager*m){m_canvasManager=m;}
	CanvasManager* GetCanvasManager(){return m_canvasManager;}

	virtual void AddShape(IShape* s);
	virtual void ClearShapes();

	virtual void AddConnector(IConnector* s);
	virtual void ClearConnectors();

	void SetPos(const math::vector2d& pos){m_pos=pos;}
	const math::vector2d& GetPos(){return m_pos;}

	void SetAngle(float a){m_angle=a;}
	float GetAngle(){return m_angle;}

	virtual void Draw();

	IShape* GetShapeFromPoint(const math::vector2d& pt);
	IConnector* GetConnectorFromPoint(const math::vector2d& pt);

	bool ProcessMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);
	void LostFocus();

	const math::rectf& GetBoundingRect(){return m_boundingRect;}
	math::rectf GetTransformedRect();

	virtual void ConnectorConnected(IConnector* conn,IConnector* other);

};

}
}

#endif
