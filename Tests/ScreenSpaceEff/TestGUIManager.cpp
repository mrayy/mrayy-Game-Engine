
#include "TestGUIManager.h"

#include "IGUIElement.h"
#include "IVideoDevice.h"
#include "GUIBatchRenderer.h"

#include "MouseEvent.h"

namespace mray
{
namespace GUI
{

	class SimpleGUIElement:public IGUIElement
	{
		video::SColor m_clr;
	public:
		static const GUID ElementType;
		SimpleGUIElement(IGUIManager* creator):IGUIElement(ElementType,creator)
		{
			m_clr=video::SColor(255,0,0,255);
		}
		virtual bool _OnMouseEvent(MouseEvent*evt)
		{
			if(this->GetClippedRect().IsPointInside(evt->pos))
			{
				m_clr=video::SColor(0,255,0,255);
				return true;
			}
			m_clr=video::SColor(255,0,0,255);
			return false;
		}
		virtual void Draw()
		{
			m_creator->GetRenderQueue()->AddQuad(0,GetClippedRect(),math::rectf(0,0,1,1),m_clr);//draw2DRectangle(GetClippedRect(),video::SColor(255,255,255,255));
		}
	};

	const GUID SimpleGUIElement::ElementType("SimpleGUIElement");

TestGUIManager::TestGUIManager(video::IVideoDevice*dev)
{
	SetDevice(dev);
	m_renderQueue=new GUIBatchRenderer();
	dynamic_cast<GUIBatchRenderer*>(m_renderQueue)->SetDevice(m_device);
}
TestGUIManager::~TestGUIManager()
{
}

void TestGUIManager::OnEvent(Event* event)
{

	if(m_rootElement){
		m_rootElement->OnEvent(event);
	}
}
void TestGUIManager::DrawAll()
{
	m_renderQueue->Prepare();
	if(m_rootElement){
		m_rootElement->Draw();
	}
	m_renderQueue->Flush();
}

void TestGUIManager::Update(float dt)
{
	if(m_rootElement){
		m_rootElement->Update(dt);
	}
}

IGUIElement* TestGUIManager::CreateElement(const GUID& type)
{
	IGUIElement* res=new SimpleGUIElement(this);
	return res;
}

}
}
