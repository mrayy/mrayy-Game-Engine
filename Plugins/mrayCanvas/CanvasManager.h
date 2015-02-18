

/********************************************************************
	created:	2011/11/29
	created:	29:11:2011   9:03
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\CanvasManager.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	CanvasManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __CanvasManager__
#define __CanvasManager__

#include "CanvasCommon.h"
#include "FontAttributes.h"
#include "MouseEvent.h"
#include "TextureUnit.h"
#include <list>

namespace mray
{
	namespace GUI
	{
		class IGUIRenderer;
	}
namespace canvas
{

	class ICanvasObject;
	class IConnectionCurve;
	class IConnector;

class CanvasManager
{
private:
protected:
	video::IVideoDevice* m_device;
	std::list<ICanvasObject*> m_objects;
	std::list<IConnectionCurve*> m_curves;

	math::vector2d m_translate;
	float m_rotate;
	float m_scale;

	math::matrix4x4 m_transformation;
	math::matrix4x4 m_transformationInv;

	math::recti m_vpRect;

	GUI::FontAttributes m_fontAttrs;
	GUI::IGUIRenderer* m_renderer;

	video::TextureUnitPtr m_background;

	ICanvasObject* m_focusObject;
	IConnector* m_selectedConnector;
	IConnectionCurve* m_selectedCurve;

	math::vector2d m_mousePos;
	bool _ProcessObjectsMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);
	bool _ProcessConnectorsMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);
	bool _ProcessCurvesMouseEvent(const math::vector2d&pt,MOUSE_EVENT_TYPE evt);

	void _updateTransformation();

	bool m_enableGrid;
	float m_gridSpacing;

protected:
	bool m_dragging;
	math::vector2d m_startPos;
public:
	CanvasManager(video::IVideoDevice* dev);
	virtual~CanvasManager();


	virtual IConnectionCurve* CreateConnection(IConnector* source,IConnector* destination);

	void SetTranslation(const math::vector2d &pos);
	const math::vector2d &GetTranslation(){return m_translate;}

	void SetBackgroundTexture(video::ITextureCRef tex);
	video::TextureUnitCRef GetBackgroundTextureUnit();

	float AlignToGrid(float v);

	bool IsGridEnabled(){return m_enableGrid;}
	void SetGridEnabled(bool e){m_enableGrid=e;}

	float GetGridSpacing(){return m_gridSpacing;}
	void SetGridSpacing(float s){m_gridSpacing=s;}

	void SetRotation(float angle);
	float GetRotation(){return m_rotate;}

	void SetScale(float scale);
	float GetScale(){return m_scale;}

	void AddObject(ICanvasObject*o);
	void RemoveObject(ICanvasObject*o);
	void ClearObjects();

	void AddCurve(IConnectionCurve*o);
	void RemoveCurve(IConnectionCurve*o);
	void ClearCurves();

	void SendBack(ICanvasObject*o);
	void SendFront(ICanvasObject*o);

	virtual void DrawObjects(const math::recti& rc);

	void SetTransformation(const math::vector2d& pos,float angle,const math::vector2d& scale);

	virtual void DrawLines(const PointList& points,const video::SColor& clr,float thickness=1);

	virtual void DrawString(const core::UTFString& str,const math::rectf& rc,const video::SColor& clr);

	virtual void DrawShapeWired(const PointList& points,const video::SColor& clr,float thickness=1);
	virtual void DrawShapeFilled(const PointList& points,const video::SColor& clr);
	virtual void DrawShapeFilledGrad(const PointList& points,const video::SColor* clr);

	virtual void DrawShapeTextured(const PointList& points,const video::SColor& clr,
		const math::vector2d* coords,video::TextureUnitCRef tex);
	
	void SetActiveCurve(IConnectionCurve*c);
	void SetFocus(ICanvasObject*o);
	bool ProcessMouseEvent(MouseEvent* evt);

	IConnector* GetSelectedConnector(){return m_selectedConnector;}
};


}
}

#endif

