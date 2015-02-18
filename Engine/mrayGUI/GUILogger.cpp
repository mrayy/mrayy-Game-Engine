
#include "GUILogger.h"

#include "FontResourceManager.h"
#include "GUIManager.h"

namespace mray{
namespace GUI{

GUILogger::GUILogger(GUIManager* manager,float messageTime,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent):
GUIElement(manager,parent,pos,size,0,0)
{
	this->messageTime=messageTime;

	
	setTypeStr(mT("Logger"));

}
GUILogger::~GUILogger(){
	clear();
}

void GUILogger::addMessage(const  mchar *msg,video::SColor clr){
	SItem*i=new SItem(msg,clr);
	items.push_back(i);
}

void GUILogger::clear(){
	std::list<SItem*>::iterator it=items.begin();
	for(;it!=items.end();++it){
		delete (*it);
	}
	items.clear();
}

void GUILogger::draw(float dt){
	if(!Visible)return;
	IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)
		return;

	math::vector2d iPos=getPos();
	float dimY=font->getCharDimension('A',m_FontAttributs.fontSize).y;

	std::list<SItem*>::iterator it=items.begin();
	for(;it!=items.end();++it){
		if((*it)->msgTime >= messageTime){
			if((*it)->Alpha>0){
				(*it)->Alpha-=255*dt;
				(*it)->Alpha=math::Max<float>(0,(*it)->Alpha);
				(*it)->clr.A=(*it)->Alpha;
			}else{
				std::list<SItem*>::iterator itn=it+1;
				delete (*it);
				items.remove(it);
				it=itn;
				if(it==items.end())
					break;
			}
		}
		m_FontAttributs.fontColor=(*it)->clr;
//		font->print(math::rectf(iPos,0),&m_FontAttributs,0,(*it)->msg,guiMngr->GetRenderQueue());
		iPos.y+=dimY;
		(*it)->msgTime+=dt;
	}
	GUIElement::draw(dt);
}
FontAttributes* GUILogger::getFontAttributes(){
	return &m_FontAttributs;
}


}
}
