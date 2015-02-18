
#include "stdafx.h"
#include "VTDebugManager.h"
#include "FontResourceManager.h"

namespace mray
{
namespace VT
{

VTDebugManager::VTDebugManager()
{
	m_renderBatch=new GUI::GUIBatchRenderer();
	m_renderBatch->SetDevice(Engine::getInstance().getDevice());
	m_fontAttrs.hasShadow=false;
}
VTDebugManager::~VTDebugManager()
{
	m_renderBatch=0;
}

void VTDebugManager::Render(const math::rectf& rc)
{
	GUI::IFont* f= gFontResourceManager.getFontByName(m_fontAttrs.fontName);
	if(!f)
		f=gFontResourceManager.getDefaultFont();
	if(f)
	{
		math::rectf r=rc;
		r.BRPoint.y=r.ULPoint.y+ m_fontAttrs.fontSize;
		core::string msg;
		ObjectList::iterator it=m_objects.begin();
		for (;it!=m_objects.end();++it)
		{
			 msg=(*it)->GetName();
			 msg+=": ";
			 msg+=(*it)->GetDebugString();
			f->print(r,&m_fontAttrs,&rc,msg,m_renderBatch);

			 r.ULPoint.y+=m_fontAttrs.fontSize;
			 r.BRPoint.y+=m_fontAttrs.fontSize;
			 if(r.ULPoint.y>rc.BRPoint.y)
				 break;
		}
		m_renderBatch->Flush();
	}
}

}
}


