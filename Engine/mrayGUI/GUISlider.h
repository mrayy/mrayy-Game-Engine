

#ifndef ___GUISLIDER___
#define ___GUISLIDER___

#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUISlider:public GUIElement
{
	enum ESliderState{
		MouseOff,
		MouseOn,
		MouseDown
	}m_state;

	GCPtr<video::TextureUnit>  bkTex;
	GCPtr<video::TextureUnit>  crTex;

	bool mouseDown;

	int m_value;
	int m_minValue;
	int m_maxValue;
	const int& getValue() { return m_value; }
	void setValue(const int &val) { m_value = val; }

	const int& getMinValue() { return m_minValue; }
	void setMinValue(const int &val) { m_minValue = val; }

	const int& getMaxValue() { return m_maxValue; }
	void setMaxValue(const int &val) { m_maxValue = val; }


	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUISlider,int> MaxValue;
	rwProperty<GUISlider,int> Value;
	rwProperty<GUISlider,int> MinValue;

	GUISlider();
	GUISlider(GUIManager* manager,math::Point2df pos,math::Point2df size,IEventReciver* event=0,GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);
	virtual~GUISlider();
	virtual void draw(float dt);
	virtual void onEvent(const EventPtr&  event);
	void setBackTexture(const  mchar*texName);
	void setCursorTexture(const  mchar*texName);
	virtual void loadTextures();
	
	
	virtual GCPtr<GUIElement> getDuplicate();
};


}
}

#endif








