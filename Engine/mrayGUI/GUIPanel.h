
/********************************************************************
	created:	2009/04/08
	created:	8:4:2009   23:42
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIPanel.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIPanel
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIPanel___
#define ___GUIPanel___

#include "CompileConfig.h"
#include "IGUIPanel.h"
#include "ITexture.h"
#include "SColor.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
namespace GUI
{

class MRAYGUI_API GUIPanel:public IGUIPanel
{
	DECLARE_RTTI;
private:
protected:
	video::TextureUnit* m_bgTexture;
	virtual bool _OnMouseEvent(MouseEvent*evt);
	bool m_drawBackground;

public:
	DECLARE_PROPERTY_TYPE(Background,bool,MRAYGUI_API);

	GUIPanel(IGUIManager*mngr);
	virtual~GUIPanel();

	virtual void Draw(const math::rectf*vp);

	void SetBackGroundTexture(video::ITextureCRef  tex);
	video::TextureUnit* GetBackGroundTexture();

	bool GetBackground() ;
	bool SetBackground(bool val);


	virtual IGUIElement* Duplicate();
};
DECLARE_ELEMENT_FACTORY(GUIPanel);

}
}


#endif //___GUIPanel___
