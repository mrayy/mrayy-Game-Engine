


/********************************************************************
	created:	2011/12/02
	created:	2:12:2011   23:47
	filename: 	d:\Development\mrayEngine\Engine\mrayCanvas\GUICanvasObject.h
	file path:	d:\Development\mrayEngine\Engine\mrayCanvas
	file base:	GUICanvasObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GUICanvasObject__
#define __GUICanvasObject__

#include "IGUIElement.h"

namespace mray
{
namespace canvas
{
	class CanvasManager;
}
namespace GUI
{

class GUICanvasObject:public IGUIElement
{
	DECLARE_RTTI;

private:
protected:
	canvas::CanvasManager* m_canvasManager;
public:
	static const GUID ElementType;

	GUICanvasObject(IGUIManager*mngr);
	virtual~GUICanvasObject();

	void SetCanvasManager(canvas::CanvasManager*m){m_canvasManager=m;}
	canvas::CanvasManager* GetCanvasManager(){return m_canvasManager;}

	virtual void Draw(video::IRenderArea*vp);
	
	virtual IGUIElement* Duplicate();
};

}
}

#endif
