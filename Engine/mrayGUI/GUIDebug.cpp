

#include "GUIDebug.h"
#include "GUIManager.h"
#include "FontResourceManager.h"

namespace mray{
namespace GUI{

GUIDebug::GUIDebug(GUIManager* manager,math::Point2df pos,math::Point2df size):
	GUIElement(manager,0,pos,size,0,0)
{
	setTypeStr(mT("Debug"));

	m_watches=new CPropertieSet();
}
GUIDebug::~GUIDebug(){
}

void GUIDebug::addWatch(IProperty*prop){
	if(prop)
		m_watches->addPropertie(prop);
}

void GUIDebug::draw(float dt){
	if(!Visible)return;

	GUI::IFont*font=gFontResourceManager.getFontByName(m_FontAttributes.fontName);;
	if(!font)return;

	TPropertieListIT it=m_watches->getProperties();
	TPropertieListIT end=m_watches->getPropertiesEnd();

	math::rectf rc=getRect();
	rc.clipRect(getClipRect());
	rc.ULPoint.x=getRect().ULPoint.x;
	rc.BRPoint.x=getDevice()->getViewportRect().getWidth();

	float baseY=rc.ULPoint.y;
	float maxX=0;
//	 mchar buffer1[32];
//	 mchar buffer2[256];
	 core::string tmp;
	for(;it!=end;++it){
		tmp=it->second->getName();
		tmp+=mT("=")+it->second->toString()+mT("[")+it->second->getInfo()+mT("]");
//		wsprintf(buffer2,mT("%s=%s [%s]",buffer1,it->second->toString(),it->second->getInfo());
//		font->print(rc,&m_FontAttributes,0,tmp,guiMngr->GetRenderQueue());

	//	if(watchesPerCol>0){
//			math::vector2d sz=font->getTextDimension(tmp,&m_FontAttributes,&rc);
//			maxX=math::Max<float>(maxX,sz.x);
//			rc.ULPoint.y+=sz.y;
	//	}else
	//		rc.ULPoint.y+=font->getDimension(mT("A",fontSize).y;

		if(rc.ULPoint.y>=rc.BRPoint.y){
			rc.ULPoint.y=baseY;
			rc.ULPoint.x+=maxX+20;
		}
	}

	
	GUIElement::draw(dt);
}

FontAttributes* GUIDebug::getFontAttributes(){
	return &m_FontAttributes;
}
}
}


