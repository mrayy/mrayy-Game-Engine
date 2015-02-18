
/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   13:01
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIPanel.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIPanel__
#define __IGUIPanel__

#include "IGUIPanelElement.h"
#include "ITexture.h"

namespace mray
{
	namespace video
	{
		class TextureUnit;
	}
namespace GUI
{

class MRAY_DLL IGUIPanel:public IGUIPanelElement
{
private:
protected:
public:
	static const GUID ElementType;

	IGUIPanel(IGUIManager*mngr);
	virtual~IGUIPanel();

	virtual void SetBackGroundTexture(video::ITextureCRef  tex)=0;
	virtual video::TextureUnit* GetBackGroundTexture()=0;
};


}
}

#endif