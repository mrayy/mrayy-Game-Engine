

#include "GUICornerFrame.h"
#include "ITexture.h"
#include "IVideoDevice.h"


namespace mray{
namespace GUI{

GUICornerFrame::GUICornerFrame(GUIManager* manager,math::vector2d pos,math::vector2d size,IEventReciver* event,GUIElement* parent,int id):
GUIImage(manager,pos,size,event,parent,id)
{
	Corners[0]=Corners[1]=Corners[2]=Corners[3]=0;
	setTypeStr(mT("CornerFrame"));
}
GUICornerFrame::~GUICornerFrame()
{
	
}


void GUICornerFrame::setCornerTexture(ECorner corner,const video::ITexturePtr  tex,bool front,math::vector2d size,video::SColor clr){
	if(!Corners[corner])
		Corners[corner]=new SCorner(tex,front);
	else
		Corners[corner]->setTexture(tex,front);
	Corners[corner]->setSize(size);
	Corners[corner]->setColor(clr);
}

void GUICornerFrame::draw(float dt){
	if(!Visible)return;
	if(!guiMngr)return;
	math::vector2d p=getPos();
	math::vector2d sz=getSize();
	const math::vector2d cpos[]={
		math::vector2d(0,0),
		math::vector2d(1,0),
		math::vector2d(0,1),
		math::vector2d(1,1),
	}; 
	math::rectf *pClip=0;
	if(getParent())
		pClip=&getParent()->getClipRect();
	//draw back corners
	for(int i=0;i<EC_COUNT;++i)
	{
		if(Corners[i] && Corners[i]->isFront()==false && Corners[i]->getTexture()){
			video::TextureUnit* ctex=Corners[i]->getTexture();
			math::vector2d tsz=Corners[i]->getSize();
			math::vector2d center=p+sz*cpos[i];
			math::rectf crect(center-tsz*0.5f,center+tsz*0.5f);
			getDevice()->draw2DImage(crect,ctex,Corners[i]->getColor(),pClip);
		}
	}

	GUIImage::draw(dt);
	//draw front corners
	for(int i=0;i<EC_COUNT;++i)
	{
		if(Corners[i] && Corners[i]->isFront()==true && Corners[i]->getTexture()){
			video::TextureUnit* ctex=Corners[i]->getTexture();
			math::vector2d tsz=Corners[i]->getSize();
			math::vector2d center=p+sz*cpos[i];
			math::rectf crect(center-tsz*0.5f,center+tsz*0.5f);
			getDevice()->draw2DImage(crect,ctex,Corners[i]->getColor(),pClip);
		}
	}
}



void GUICornerFrame::loadXMLSettings(xml::XMLElement* elem){

}


}
}

