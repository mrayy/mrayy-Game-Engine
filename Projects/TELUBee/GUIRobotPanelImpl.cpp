

#include "stdafx.h"
#include "GUIRobotPanelImpl.h"
#include "IGUIManager.h"
#include "IGUIStaticImage.h"
#include "TextureResourceManager.h"

namespace mray
{
namespace GUI
{

void GUIRobotPanelImpl::SetRobotInfo(const TBee::TBRobotInfo& ifo)
{
	NameLbl->SetText(ifo.name);
	IPLbl->SetText(ifo.IP);
	core::string latLng=mT("(")+core::StringConverter::toString((float)((int)ifo.lat*100)*0.01)+" , "+core::StringConverter::toString((float)((int)ifo.lng*100)*0.01)+mT(")");
	LatLngLbl->SetText(latLng);
	if(ifo.Avaliable)
	{
		AvailableLbl->GetFontAttributes()->fontColor.Set(0.5,1,0.5,1);
	}else
	{
		AvailableLbl->GetFontAttributes()->fontColor.Set(1,0.5,0.5,1);
	}
	//LatLbl->SetText(core::StringConverter::toString(ifo.lat));
	//LngLbl->SetText(core::StringConverter::toString(ifo.lng));
	//ConnectedLbl->SetText(core::string(ifo.Connected?"Yes":"No"));
	AvailableLbl->SetText(core::string(ifo.Avaliable?"Yes":"No"));
}

void GUIRobotPanelImpl::Show()
{
	Root->SetVisible(true);
	m_visible=true;
}

void GUIRobotPanelImpl::Hide()
{
	//Root->SetVisible(false);
	m_visible=false;
}

void GUIRobotPanelImpl::Update(float dt)
{
	if(!m_visible && Root->IsVisible())
	{
		m_alpha-=dt;
		Root->SetAlpha(m_alpha);
		if(m_alpha<=0)
		{
			m_alpha=0;
			Root->SetVisible(false);
		}
	}
	if(m_visible && m_alpha<1)
	{
		m_alpha+=dt;
		if(m_alpha<=0)
		{
			m_alpha=1;
		}
		Root->SetAlpha(m_alpha);
		
	}
}

}
}
