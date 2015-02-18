


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   9:02
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\ICShape.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	ICShape
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ICShape__
#define __ICShape__

#include "CanvasCommon.h"
#include "IShape.h"
#include "MouseEvent.h"

namespace mray
{

namespace canvas
{
	class ICanvasObject;
	class ICShape;

	class ICShapeListener
	{
	public:
		virtual void OnShapeClicked(ICShape*shape)=0;
	};

class ICShape:public IShape
{
private:
protected:
	PointList m_points;
	math::rectf m_boundingRect;

public:
	enum EMouseResult
	{
		ENone,
		EMoved,
		EClicked
	};
public:
	ICShape();
	virtual~ICShape();

	virtual void Draw(EMouseStatus status);

	virtual EMouseResult ProcessMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);

	virtual void SetShapePoints(const PointList &points);
	const PointList& GetShapePoints();
	const math::rectf& GetBoundingRect();

	bool IsPointIn(const math::vector2d& pt);
};

}
}

#endif
