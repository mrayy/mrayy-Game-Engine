#include "stdafx.h"
#include "GUIOculusDetectImpl.h"


namespace mray
{
void GUIOculusDetectImpl::SetState(GUIOculusDetectImpl::EState s)
{
	m_state=s;
	DoneLoadingElements();
}
void GUIOculusDetectImpl::DoneLoadingElements()
{

	if(StatusLbl)
	{
		if(m_state==EFound)
		{
			this->StatusLbl->SetText(core::string("Oculus was found!"));
			StatusLbl->GetFontAttributes()->fontColor=(video::SColor(0,1,0,1));
		}
		else
		{
			this->StatusLbl->SetText(core::string("Failed to detect Oculus!"));
			StatusLbl->GetFontAttributes()->fontColor=(video::SColor(1,0,0,1));
		}
	}
}


}

