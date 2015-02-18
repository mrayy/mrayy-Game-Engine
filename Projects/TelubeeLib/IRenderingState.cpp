

#include "StdAfx.h"
#include "IRenderingState.h"
#include "AppData.h"


namespace mray
{
namespace TBee
{

	void IRenderingState::LoadFromXML(xml::XMLElement* e)
	{
		xml::XMLAttribute* a=e->getAttribute("Name");
		if(a)
			m_name=a->value;
	}
	xml::XMLElement* IRenderingState::WriteToXML(xml::XMLElement* e)
	{
		xml::XMLElement* elem = new xml::XMLElement(m_name);
		e->addSubElement(elem);
		return elem;
	}

	void IRenderingState::InitState()
	{
		AppData* app = AppData::Instance();
		gLogManager.log("Initing Rendering State", ELL_INFO);
		video::IVideoDevice* dev= Engine::getInstance().getDevice();
		int c=1;
		if (app->stereoMode != ERenderStereoMode::None)
			c=2;
		for(int i=0;i<2;++i)
		{
			m_rtTexture[i] = dev->createEmptyTexture2D(true);
			m_rtTexture[i]->setMipmapsFilter(false);

			m_renderTarget[i] = dev->createRenderTarget("", m_rtTexture[i], 0, 0, 0);
		}
	}

	video::IRenderTarget* IRenderingState::Render(const math::rectf& rc,ETargetEye eye)
	{
		int index=GetEyeIndex(eye);
		//math::vector2d ratio=TBAppGlobals::GetStereoScaleRatio();
		if(m_rtTexture[index]->getSize().x!=rc.getWidth() ||
			m_rtTexture[index]->getSize().y!=rc.getHeight())
		{
			m_rtTexture[index]->createTexture(math::vector3d(rc.getWidth(),rc.getHeight(),1),video::EPixel_R8G8B8);
		}
		return m_renderTarget[index];

	}


	bool IRenderingState::CanSleep()
	{
		return true;
	}

}
}

