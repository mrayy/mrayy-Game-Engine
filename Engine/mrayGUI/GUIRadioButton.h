

#ifndef ___GUIRADIO_BUTTON___
#define ___GUIRADIO_BUTTON___

#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIRadioButton:public GUIElement
{
protected:
	enum ERadioState{
		ER_Unchecked,
		ER_Checked,
		ER_MouseOn,
		ER_MouseDown
	}m_state;
	GCPtr<GUI::IFont> font;
	bool m_checked;
	bool m_BGOpaque;
	bool m_pushButton;
	FontAttributes m_FontAttributs;

	const bool& getChecked() { return m_checked; }
	void setChecked(const bool &val);

	const bool& getBGOpaque() { return m_BGOpaque; }
	void setBGOpaque(const bool &val) { m_BGOpaque = val; }

	const bool& getPushButton() { return m_pushButton; }
	void setPushButton(const bool &val) { m_pushButton = val; }

	GCPtr<video::TextureUnit>  bkTexture;
	virtual void fillProperties();

public:
	DECLARE_RTTI;

	rwProperty<GUIRadioButton,bool> Checked;
	rwProperty<GUIRadioButton,bool> BGOpaque;
	rwProperty<GUIRadioButton,bool> PushButton;
	
	GUIRadioButton();
	GUIRadioButton(GUIManager* manager,const  core::UTFString& _text,math::Point2df pos,math::Point2df size,IEventReciver* event=0,
		GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);
	virtual~GUIRadioButton();
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);

	virtual void loadTextures();

	virtual GCPtr<GUIElement> getDuplicate();
	virtual FontAttributes* getFontAttributes();

};

}
}

#endif