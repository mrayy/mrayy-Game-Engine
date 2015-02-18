
#include "GUIDefaultElements.h"
#include "GUIElementFactory.h"
#include "GUIEffectorFactory.h"

#include "GUIButton.h"
#include "GUICellSprite.h"
#include "GUICheckBox.h"
#include "GUIComboList.h"
#include "GUIEditBox.h"
#include "GUIImage.h"
#include "GUIList.h"
#include "GUIPanel.h"
#include "GUIRadioButton.h"
#include "GUIRotImage.h"
#include "GUISlider.h"
#include "GUITabPanel.h"
#include "GUIStaticText.h"
#include "GUIToolBar.h"
#include "GUIWindow.h"
#include "GUIDummy.h"


#include "GUIAnchorEffector.h"
#include "GUIPercentSizeEffector.h"
#include "GUICircularEffector.h"
#include "GUIFadeEffector.h"
#include "GUIPercentAnchorEff.h"
#include "GUITranslateEffector.h"


namespace mray{
namespace GUI{

class ButtonCreator:public IGUIElementCreator
{
public:
	ButtonCreator(){}
	virtual~ButtonCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIButton();
	}
	virtual const mchar* getType(){
		return mT("GUIButton");
	}
};

class CellSpriteCreator:public IGUIElementCreator
{
public:
	CellSpriteCreator(){}
	virtual~CellSpriteCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUICellSprite();
	}
	virtual const mchar* getType(){
		return mT("GUICellSprite");
	}
};

class CheckboxCreator:public IGUIElementCreator
{
public:
	CheckboxCreator(){}
	virtual~CheckboxCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		GCPtr<GUICheckBox> chbox= new GUICheckBox();
		return chbox;
	}
	virtual const mchar* getType(){
		return mT("GUICheckBox");
	}
};

class ComboListCreator:public IGUIElementCreator
{
public:
	ComboListCreator(){}
	virtual~ComboListCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIComboList();
	}
	virtual const mchar* getType(){
		return mT("GUIComboList");
	}
};

class EditBoxCreator:public IGUIElementCreator
{
public:
	EditBoxCreator(){}
	virtual~EditBoxCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIEditBox();
	}
	virtual const mchar* getType(){
		return mT("GUIEditBox");
	}
};

class ImageCreator:public IGUIElementCreator
{
public:
	ImageCreator(){}
	virtual~ImageCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIImage();
	}
	virtual const mchar* getType(){
		return mT("GUIImage");
	}
};

class ListCreator:public IGUIElementCreator
{
public:
	ListCreator(){}
	virtual~ListCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIList();
	}
	virtual const mchar* getType(){
		return mT("GUIList");
	}
};

class PanelCreator:public IGUIElementCreator
{
public:
	PanelCreator(){}
	virtual~PanelCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIPanel();
	}
	virtual const mchar* getType(){
		return mT("GUIPanel");
	}
};

class RadioCreator:public IGUIElementCreator
{
public:
	RadioCreator(){}
	virtual~RadioCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIRadioButton();
	}
	virtual const mchar* getType(){
		return mT("GUIRadioButton");
	}
};

class RotateImageCreator:public IGUIElementCreator
{
public:
	RotateImageCreator(){}
	virtual~RotateImageCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIRotImage();
	}
	virtual const mchar* getType(){
		return mT("GUIRotImage");
	}
};

class SliderCreator:public IGUIElementCreator
{
public:
	SliderCreator(){}
	virtual~SliderCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUISlider();
	}
	virtual const mchar* getType(){
		return mT("GUISlider");
	}
};

class StaticTextCreator:public IGUIElementCreator
{
public:
	StaticTextCreator(){}
	virtual~StaticTextCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIStaticText();
	}
	virtual const mchar* getType(){
		return mT("GUIStaticText");
	}
};

class TabPanelCreator:public IGUIElementCreator
{
public:
	TabPanelCreator(){}
	virtual~TabPanelCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUITabPanel();
	}
	virtual const mchar* getType(){
		return mT("GUITabPanel");
	}
};

class ToolBarCreator:public IGUIElementCreator
{
public:
	ToolBarCreator(){}
	virtual~ToolBarCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIToolBar();
	}
	virtual const mchar* getType(){
		return mT("GUIToolBar");
	}
};

class WindowCreator:public IGUIElementCreator
{
public:
	WindowCreator(){}
	virtual~WindowCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIWindow();
	}
	virtual const mchar* getType(){
		return mT("GUIWindow");
	}
};

class DummyCreator:public IGUIElementCreator
{
public:
	DummyCreator(){}
	virtual~DummyCreator(){}
	virtual GCPtr<GUIElement> createElement(){
		return new GUIDummy();
	}
	virtual const mchar* getType(){
		return mT("GUIDummy");
	}
};

//////////////////////////////////////////////////////////////////////////


class AnchorEffCreator:public IGUIEffectorCreator
{
public:
	AnchorEffCreator(){}
	virtual~AnchorEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUIAnchorEffector(GUI::EPAnchor_UL);
	}
	virtual const mchar* getType(){
		return mT("GUIAnchorEffector");
	}
};

class CircularEffCreator:public IGUIEffectorCreator
{
public:
	CircularEffCreator(){}
	virtual~CircularEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUICircularEffector(0,0,0);
	}
	virtual const mchar* getType(){
		return mT("GUICircularEffector");
	}
};

class FadeEffCreator:public IGUIEffectorCreator
{
public:
	FadeEffCreator(){}
	virtual~FadeEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUIFadeEffector(0,0,0,0);
	}
	virtual const mchar* getType(){
		return mT("GUIFadeEffector");
	}
};

class PercentAnchorEffCreator:public IGUIEffectorCreator
{
public:
	PercentAnchorEffCreator(){}
	virtual~PercentAnchorEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUIPercentAnchorEff(GUI::EPAnchor_UL,0);
	}
	virtual const mchar* getType(){
		return mT("GUIPercentAnchorEff");
	}
};

class TranslateEffCreator:public IGUIEffectorCreator
{
public:
	TranslateEffCreator(){}
	virtual~TranslateEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUITranslateEffector(0,0,0);
	}
	virtual const mchar* getType(){
		return mT("GUITranslateEffector");
	}
};

class SizeEffCreator:public IGUIEffectorCreator
{
public:
	SizeEffCreator(){}
	virtual~SizeEffCreator(){}
	virtual GCPtr<GUIEffector> createEffector(){
		return new GUIPercentSizeEffector(1,1);
	}
	virtual const mchar* getType(){
		return mT("GUIPercentSizeEffector");
	}
};

//////////////////////////////////////////////////////////////////////////
GUIDefaultElements::GUIDefaultElements(){}
GUIDefaultElements::~GUIDefaultElements(){}

void GUIDefaultElements::installElements(){
	GUIElementFactory::getInstance().addElementCreator(new ButtonCreator());
	GUIElementFactory::getInstance().addElementCreator(new CellSpriteCreator());
	GUIElementFactory::getInstance().addElementCreator(new CheckboxCreator());
	GUIElementFactory::getInstance().addElementCreator(new EditBoxCreator());
	GUIElementFactory::getInstance().addElementCreator(new ImageCreator());
	GUIElementFactory::getInstance().addElementCreator(new PanelCreator());
	GUIElementFactory::getInstance().addElementCreator(new RadioCreator());
	GUIElementFactory::getInstance().addElementCreator(new RotateImageCreator());
	GUIElementFactory::getInstance().addElementCreator(new SliderCreator());
	GUIElementFactory::getInstance().addElementCreator(new StaticTextCreator());
	GUIElementFactory::getInstance().addElementCreator(new TabPanelCreator());
	GUIElementFactory::getInstance().addElementCreator(new ToolBarCreator());
	GUIElementFactory::getInstance().addElementCreator(new WindowCreator());
	GUIElementFactory::getInstance().addElementCreator(new DummyCreator());
	GUIElementFactory::getInstance().addElementCreator(new ComboListCreator());


	GUIEffectorFactory::getInstance().addEffectorCreator(new AnchorEffCreator());
	GUIEffectorFactory::getInstance().addEffectorCreator(new CircularEffCreator());
	GUIEffectorFactory::getInstance().addEffectorCreator(new FadeEffCreator());
	GUIEffectorFactory::getInstance().addEffectorCreator(new PercentAnchorEffCreator());
	GUIEffectorFactory::getInstance().addEffectorCreator(new TranslateEffCreator());;
	GUIEffectorFactory::getInstance().addEffectorCreator(new SizeEffCreator());
}
void GUIDefaultElements::removeElements(){
}

}
}
