

#include "stdafx.h"
#include "GUITreeNode.h"
#include "IFont.h"
#include "IGUITheme.h"
#include "IGUITreeBox.h"
#include "IGUIManager.h"
#include "IVideoDevice.h"
#include "DefaultColors.h"
#include "IGUIRenderer.h"
#include "MouseEvent.h"


namespace mray
{
namespace GUI
{

void GUITreeNode::Draw(GUITreeBoxDrawInfo& ifo)
{
	math::rectf tmpRc(ifo.pos,ifo.pos+math::vector2d(0,ifo.fontAttrs->fontSize));
	//add expand/collapse size
	if(GetNodes().size()>0)
		tmpRc.BRPoint.x+=ifo.fontAttrs->fontSize;


	bool hasImg=!GetImageSet().isNull() && GetImageIndex()!="";
	video::ImageRegion* region=0;
	if(hasImg)
	{
		region=m_imageSet->GetRegion(m_imageIndex);
		hasImg&=(region!=0);
	}
	//tmpRc.ULPoint.x+=5;
	//tmpRc.BRPoint.x+=5;
	//add image size
	if(hasImg)
	{
		tmpRc.BRPoint.x+=ifo.fontAttrs->fontSize*1.5;
	}

	tmpRc.BRPoint.x+=ifo.font->getTextDimension(GetText(),ifo.fontAttrs,&ifo.rc).x;


	IGUITheme* skin=ifo.owner->GetCreator()->GetActiveTheme();
	ifo.owner->GetCreator()->GetDevice()->draw2DRectangle(tmpRc,GetBackColor());
	if(GetNodes().size()>0)
	{
		core::string profileName;
		if(IsExpanded())
			profileName="TreeboxExpand";
		else
			profileName="TreeboxCollapse";
		skin->drawBox(ifo.owner->GetCreator()->GetRenderQueue(),math::rectf(tmpRc.ULPoint,tmpRc.ULPoint+ifo.fontAttrs->fontSize),profileName,0,video::DefaultColors::White);

		tmpRc.ULPoint.x+=ifo.fontAttrs->fontSize;//*2;
	}

	//draw icon
	if(hasImg)
	{
		ifo.owner->GetCreator()->GetRenderQueue()->AddQuad(m_imageSet->GetTexture(),
			math::rectf(tmpRc.ULPoint,tmpRc.ULPoint+ifo.fontAttrs->fontSize),
			region->GetTextureRect(),video::DefaultColors::White,0,0);
		tmpRc.ULPoint.x+=ifo.fontAttrs->fontSize*1.5;
	}

	ifo.font->print(tmpRc,ifo.fontAttrs,&ifo.rc,GetText(),ifo.owner->GetCreator()->GetRenderQueue());

	ifo.pos.y+= tmpRc.getHeight();

	if(IsExpanded())
	{
		ifo.pos.x+=ifo.fontAttrs->fontSize*2;
		for(int i=0;i<m_nodes.size();++i)
		{
			m_nodes[i]->Draw(ifo);
			if(ifo.pos.y>=ifo.rc.BRPoint.y)
				break;
		}
		ifo.pos.x-=ifo.fontAttrs->fontSize*2;
	}
}

bool GUITreeNode::OnMouseEvent(MouseEvent* e,GUITreeBoxDrawInfo& ifo)
{
	math::rectf tmpRc(ifo.pos,ifo.pos+math::vector2d(0,ifo.fontAttrs->fontSize));
	ifo.pos.y+= tmpRc.getHeight();

	//add expand/collapse size
	if(GetNodes().size()>0)
		tmpRc.BRPoint.x+=ifo.fontAttrs->fontSize;
	else
		return false;
	if(tmpRc.IsPointInside(e->pos))
		return true;

	if(IsExpanded())
	{
		ifo.pos.x+=ifo.fontAttrs->fontSize*2;
		for(int i=0;i<m_nodes.size();++i)
		{
			if(m_nodes[i]->OnMouseEvent(e,ifo))
				return true;
			if(ifo.pos.y>=ifo.rc.BRPoint.y)
				break;
		}
		ifo.pos.x-=ifo.fontAttrs->fontSize*2;
	}
	return false;
}


}
}


