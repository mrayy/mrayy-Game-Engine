


/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   12:02
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\IConnector.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	IConnector
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IConnector__
#define __IConnector__

#include "CanvasCommon.h"
#include "MouseEvent.h"

namespace mray
{
namespace canvas
{
	class ICanvasObject;
	class IConnectionCurve;

	enum EConnectorPlacement
	{
		ECP_Top,
		ECP_Bot,
		ECP_Left,
		ECP_Right
	};

class IConnector
{
private:
protected:
	uint m_id;
	ICanvasObject* m_owner;

	std::list<IConnectionCurve*> m_connections;

	float m_relativePos;
	EConnectorPlacement m_placement;

	PointList m_points;
	math::rectf m_lastRect;

	video::SColor m_bgColor;

	void _updateRect();

public:
	enum EAcceptanceResult
	{
		EAR_No,
		EAR_Source,
		EAR_Destination
	};

	IConnector();
	virtual~IConnector();

	void SetID(uint id){m_id=id;}
	uint GetID(){return m_id;}


	void SetRelativePos(float  p){m_relativePos=p;}
	float GetRelativePos(){return m_relativePos;}

	void SetPlacement(EConnectorPlacement p){m_placement=p;}
	EConnectorPlacement GetPlacement(){return m_placement;}

	void SetOwner(ICanvasObject*o){m_owner=o;}
	ICanvasObject* GetOwner(){return m_owner;}

	virtual void Draw();
	virtual bool ProcessMouse(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);

	virtual void OnConnected(IConnector* other);

	virtual void AddConnection(IConnectionCurve* other);
	bool HasConnection(IConnector* other);
	void RemoveConnection(IConnector* other);
	void ClearConnections();
	void OnConnectionRemoved(IConnectionCurve*o);
	const std::list<IConnectionCurve*>& GetConnections(){return m_connections;}
	

	virtual math::vector2d GetConnectionPoint();
	virtual math::vector2d GetConnectionNormal();

	bool IsPointIn(const math::vector2d& pt);

	virtual EAcceptanceResult Accepts(IConnector* other){return EAR_Source;}


	const math::rectf& GetRect();
	const math::rectf& GetLastRect(){return m_lastRect;}
};

}
}

#endif
