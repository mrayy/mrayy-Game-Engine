
#include "Stdafx.h"

#include "GUIManager.h"
#include "IGUIElement.h"

#include "IGUITheme.h"
#include "ICursorControl.h"

#include "GUIButton.h"
#include "GUIPanel.h"
#include "GUICheckBox.h"
#include "GUIEditBox.h"
#include "GUIStaticText.h"
#include "GUIListBox.h"
#include "GUIComboList.h"
#include "GUIStaticImage.h"
#include "GUIWindow.h"
#include "GUITreeBox.h"
#include "GUIPropertyGrid.h"
#include "GUICatagoryPanel.h"
#include "GUIStackPanel.h"
#include "GUISliderBar.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "IVideoDevice.h"


#include "GUIElementFactory.h"

/*
#include "GUIWindow.h"
#include "GUIStaticText.h"
#include "GUIImage.h"
#include "GUIRadioButton.h"
#include "GUISlider.h"
#include "GUIComboList.h"
#include "GUIDummy.h"
#include "GUIGrid.h"
#include "GUIToolBar.h"
#include "GUITabPanel.h"
#include "GUIFileBox.h"
#include "GUIContextMenu.h"
#include "GUIMenu.h"
#include "GUIAnimatedSprite.h"
#include "GUICornerFrame.h"
#include "GUILoadingBar.h"
#include "GUILogger.h"
//#include "GUISceneNode.h"
#include "GUIHistogram.h"
#include "GUIParticleSystem.h"

#include "GUIEffector.h"
#include "GUIFadeEffector.h"
#include "GUICircularEffector.h"
#include "GUITranslateEffector.h"
#include "GUIRotImage.h"
#include "GUIThemeManager.h"
#include "IFont.h"*/


#include "ITimer.h"
#include "Tracemanager.h"
#include "BenchmarkItem.h"
#include "Engine.h"

//#include "Engine.h"



namespace mray{
namespace GUI{

bool s_defaultElementRegistered=false;

GUIManager::GUIManager(video::IVideoDevice*dev)
:m_Skin(0)
{	
	m_benchmarkItem=new BenchmarkItem(mT("GUIManager"));
	m_renderBI=m_benchmarkItem->addSubItem(mT("Render"));
	m_updateBI=m_benchmarkItem->addSubItem(mT("Update"));

	m_renderQueue=new GUIBatchRenderer();

	m_specifications=new LanguageFontSpecification();

	SetDevice(dev);
	if(!s_defaultElementRegistered)
	{
		s_defaultElementRegistered=true;
		_loadDefaultFactories();
	}
}
GUIManager::~GUIManager()
{
	delete m_specifications;
	delete m_renderQueue;
	UnregisterBenchmark();

	delete m_benchmarkItem;
}



void GUIManager::_loadDefaultFactories()
{
	REGISTER_GUIElement_FACTORY(GUIButton);
	REGISTER_GUIElement_FACTORY(GUIPanel);
	REGISTER_GUIElement_FACTORY(GUICheckBox);
	REGISTER_GUIElement_FACTORY(GUIEditBox);
	REGISTER_GUIElement_FACTORY(GUIStaticText);
	REGISTER_GUIElement_FACTORY(GUIListBox);
	REGISTER_GUIElement_FACTORY(GUIStaticImage);
	REGISTER_GUIElement_FACTORY(GUIComboList);
	REGISTER_GUIElement_FACTORY(GUIWindow);
	REGISTER_GUIElement_FACTORY(GUIPropertyGrid);
	REGISTER_GUIElement_FACTORY(GUIStackPanel);
	REGISTER_GUIElement_FACTORY(GUICatagoryPanel);
	REGISTER_GUIElement_FACTORY(GUISliderBar);
	REGISTER_GUIElement_FACTORY(GUITreeBox);
}

void GUIManager::RegisterBenchmark()
{
	Engine::getInstance().getRootBenchmarking()->addSubItem(m_benchmarkItem);
}
void GUIManager::UnregisterBenchmark()
{
	Engine::getInstance().getRootBenchmarking()->removeSubItem(m_benchmarkItem);
}

void GUIManager::SetDevice(video::IVideoDevice* dev)
{
	IGUIManager::SetDevice(dev);
	((GUIBatchRenderer*)m_renderQueue)->SetDevice(m_device);
}

void GUIManager::SetActiveTheme(IGUITheme* theme)
{
	m_Skin=theme;
	FIRE_LISTENR_METHOD(OnChangeGUITheme, (this, theme));
}
IGUITheme*	GUIManager::GetActiveTheme()
{
	return m_Skin;
}
IGUIRenderer* GUIManager::GetRenderQueue()
{
	return m_renderQueue;
}


IGUIElement* GUIManager::GetElementFromPoint(float x,float y)
{
	return 0;
}


LanguageFontSpecification* GUIManager::GetLanguageSpecification()
{
	return m_specifications;
}

bool GUIManager::OnEvent(Event* event, const math::rectf*vp)
{
	MouseEvent tmp;
	Event* evt = event;
	if (event->getType() == ET_Mouse && vp)
	{
		tmp = *(MouseEvent*)event;
		tmp.pos -= vp->ULPoint;
		evt = &tmp;
	}
	bool res=false;
	if(m_focusElement!=0)
		res=m_focusElement->OnEvent(evt);
	if (!evt->isRecived() && m_rootElement)
		res = m_rootElement->OnEvent(evt);
	if (!res || !evt->isRecived())
	{
		if (evt->getType() == ET_Mouse)
		{
			MouseEvent*e=dynamic_cast<MouseEvent*>(event);
				
			//RemoveMouseOn();
			if(e->event==MET_LEFTDOWN || e->event==MET_RIGHTDOWN )
				RemoveFocus();
		}
	}

	if (evt->isRecived() && evt == &tmp)
		event->setRecived();
	return res;
	//GUIElementContainer::onEvent(event);
	/*
	if(!event->isRecived()){
		RemoveMouseOn();
		if(event->getType()==ET_Mouse ){
			MouseEvent*e=(MouseEvent*)event.pointer();
			if(e->Event==MET_LEFTDOWN || e->Event==MET_RIGHTDOWN)
				removeFocus();
		}
		gCursorControl.setCursor(EC_Arrow);
	}
	if(event->getType()==ET_GuiEvent){

		if(EventQueue::isExist())
			EventQueue::getInstance().pushEvent(event);
	}
	if(m_listener){
		m_listener->onEvent(event);
	}
	if( event->getType()==ET_Keyboard){ 
		KeyboardEvent*e=(KeyboardEvent*)event.pointer();
		if(e->key==KEY_TAB && e->press)
		{
			if(focusElement==GUIElementPtr::Null)
			{
				if(m_Children.getSize()){
					focusElement=*m_Children.begin();
				}
			}else{
				focusElement=getNextChild(focusElement);
				if(!focusElement)
					focusElement=*m_Children.begin();
			}
			if(focusElement){
				const GUIElementPtr&  tFocus=focusElement;
				while(!focusElement->Selectable || !focusElement->Visible){
					focusElement=getNextChild(focusElement);
					if(!focusElement)
						focusElement=*m_Children.begin();
					if(focusElement==tFocus){
						focusElement=0;
						break;
					}
				}
			}
		}
	}*/
}




void GUIManager::DrawAll(const math::rectf*vp)
{
	_AUTO_BENCHMARK(m_renderBI);
	m_device->set2DMode();
	m_renderQueue->Prepare();
	if(m_Skin && m_Skin->getMaterial() && m_Skin->getMaterial()->GetActiveTechnique())
	{
		m_device->useRenderPass(m_Skin->getMaterial()->GetActiveTechnique()->GetPassAt(0));
	}
	else m_device->useRenderPass(0);
/*
	std::list<IGUIElement*  >::iterator it=m_elements.begin();
	for(;it!=m_elements.end();++it)
	{
		(*it)->Draw();
	}*/
	FIRE_LISTENR_METHOD(OnGUIDrawBegin, (this));

	if(m_rootElement)
		m_rootElement->Draw(vp);
	
	//draw post elements
	std::list<IGUIElement*>::iterator it= m_postDrawElements.begin();
	std::list<IGUIElement*>::iterator end= m_postDrawElements.end();
	for(;it!=end;++it)
		(*it)->PostDraw(vp);
	m_postDrawElements.clear();
	FIRE_LISTENR_METHOD(OnGUIDrawDone, (this));

	m_renderQueue->Flush();
	//m_renderQueue->Clear();
}

void GUIManager::Update(float dt)
{
	_AUTO_BENCHMARK(m_updateBI);
	/*std::list<IGUIElement*  >::iterator it=m_elements.begin();
	for(;it!=m_elements.end();++it)
	{
		(*it)->Update(dt);
		}*/
	if(m_rootElement)
		m_rootElement->Update(dt);
}

IGUIElement* GUIManager::CreateElement(const GUID& type)
{
	IGUIElement* e= GUIElementFactory::getInstance().CreateElement(type.GetString(),this);
	FIRE_LISTENR_METHOD(OnAddGUIElement, (this,e));
	return e;

}


}
}
