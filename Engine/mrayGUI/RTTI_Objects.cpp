
#include "stdafx.h"
#include "GUIButton.h"
#include "GUICheckBox.h"
#include "GUIEditBox.h"
#include "GUIWindow.h"
#include "GUIPanel.h"
#include "GUIListBox.h"
#include "GUIComboList.h"
#include "GUIStaticImage.h"
#include "GUIPropertyGrid.h"
#include "GUIStackPanel.h"
#include "GUICatagoryPanel.h"
#include "GUISliderBar.h"
#include "GUITreeBox.h"




namespace mray{
namespace GUI{


IMPLEMENT_RTTI(GUIButton,IGUIElement)
IMPLEMENT_RTTI(GUICheckBox,IGUIElement);
IMPLEMENT_RTTI(GUIEditBox,IGUIElement)
IMPLEMENT_RTTI(GUIListBox,IGUIListBox);
IMPLEMENT_RTTI(GUIComboList,IGUIElement);
IMPLEMENT_RTTI(GUIStaticImage,IGUIElement);
IMPLEMENT_RTTI(GUIPropertyGrid,IGUIElement);
IMPLEMENT_RTTI(GUISliderBar,IGUIElement);
IMPLEMENT_RTTI(GUITreeBox,IGUIElement);

IMPLEMENT_RTTI(GUIPanel,IGUIPanelElement);
IMPLEMENT_RTTI(GUIWindow,IGUIPanelElement);
IMPLEMENT_RTTI(GUIStackPanel,IGUIPanelElement);
IMPLEMENT_RTTI(GUICatagoryPanel,IGUIPanelElement);
/*
IMPLEMENT_RTTI(GUIEffector,IObject);

IMPLEMENT_RTTI(GUIElement,IObject);
IMPLEMENT_RTTI(GUIComboList,GUIElement);
IMPLEMENT_RTTI(GUIConsole,GUIElement);
IMPLEMENT_RTTI(GUIContextMenu,GUIElement);
IMPLEMENT_RTTI(GUIDebug,GUIElement);
IMPLEMENT_RTTI(GUIDummy,GUIElement);
IMPLEMENT_RTTI(GUIFileBox,GUIWindow);
IMPLEMENT_RTTI(GUIGrid,GUIElement);
IMPLEMENT_RTTI(GUIHistogram,GUIElement);
IMPLEMENT_RTTI(GUIImage,GUIElement);
IMPLEMENT_RTTI(GUIList,GUIElement);
IMPLEMENT_RTTI(GUILoadingBar,GUIElement);
IMPLEMENT_RTTI(GUILogger,GUIElement);
IMPLEMENT_RTTI(GUIMenu,GUIElement);
IMPLEMENT_RTTI(GUIPanel,GUIElement);
IMPLEMENT_RTTI(GUIParticle,GUIElement);
IMPLEMENT_RTTI(GUIParticleSystem,GUIElement);
IMPLEMENT_RTTI(GUIPropertyGrid,GUIElement);
IMPLEMENT_RTTI(GUIRadioButton,GUIElement);
IMPLEMENT_RTTI(GUISceneNode,GUIElement);
IMPLEMENT_RTTI(GUISlider,GUIElement);
IMPLEMENT_RTTI(GUITabPanel,GUIElement);
IMPLEMENT_RTTI(GUIToolBar,GUIElement);


IMPLEMENT_RTTI(GUICircularEffector,GUIEffector);
IMPLEMENT_RTTI(GUIFadeEffector,GUIEffector);
IMPLEMENT_RTTI(GUIAnchorEffector,GUIEffector);
IMPLEMENT_RTTI(GUIPercentSizeEffector,GUIEffector);
IMPLEMENT_RTTI(GUITranslateEffector,GUIEffector);

IMPLEMENT_RTTI(GUIPercentAnchorEff,GUIAnchorEffector);
IMPLEMENT_RTTI(GUIAnimatedSprite,GUICellSprite);
IMPLEMENT_RTTI(GUIBenchMark,GUIList);

IMPLEMENT_RTTI(GUICellSprite,GUIRotImage);
IMPLEMENT_RTTI(GUICornerFrame,GUIImage);
IMPLEMENT_RTTI(GUIMessage,GUIWindow);
IMPLEMENT_RTTI(GUIRotImage,GUIImage);*/



}
}