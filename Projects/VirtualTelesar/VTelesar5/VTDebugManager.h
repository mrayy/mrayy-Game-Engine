
/********************************************************************
	created:	2013/03/04
	created:	4:3:2013   14:04
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\VTDebugManager.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	VTDebugManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _VTDebugManager_
#define _VTDebugManager_

#include "IVTDebugInterface.h"
#include "FontAttributes.h"
#include "GUIBatchRenderer.h"

namespace mray
{
namespace VT
{

class VTDebugManager:public IVTDebugInterface
{
protected:
	GUI::FontAttributes m_fontAttrs;
	GCPtr<GUI::GUIBatchRenderer> m_renderBatch;
public:
	VTDebugManager();
	virtual~VTDebugManager();

	void Render(const math::rectf& rc);
};


}
}

#endif
