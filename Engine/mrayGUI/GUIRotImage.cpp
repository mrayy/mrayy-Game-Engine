

#include "GUIRotImage.h"
#include "GUIManager.h"
#include "CPropertie.h"
#include "ITexture.h"

namespace mray{
namespace GUI{


GUIRotImage::GUIRotImage():
	GUIImage(),
	RotationSpeed(this,&GUIRotImage::setRotationSpeed,&GUIRotImage::getRotationSpeed,EPT_FLOAT,mT("angleSpeed"),mT("speed of the rotation(per second)"))
{
	RotationSpeed=0;
	setTypeStr(mT("RotImage"));
	fillProperties();
}
GUIRotImage::GUIRotImage(GUIManager* manager,math::vector2d pos,math::vector2d size,GCPtr<GUIElement> parent,int id):
GUIImage(manager,pos,size,0,parent,id),
RotationSpeed(this,&GUIRotImage::setRotationSpeed,&GUIRotImage::getRotationSpeed,EPT_FLOAT,mT("angleSpeed"),mT("speed of the rotation(per second)"))
{
	RotationSpeed=0;
	setTypeStr(mT("RotImage"));
	fillProperties();
};

GUIRotImage::~GUIRotImage(){
}

void GUIRotImage::fillProperties(){
	addPropertie(&RotationSpeed,mT("Apperance"));
}

void GUIRotImage::draw(float dt){
	if(!Visible)return;
	if(!guiMngr)return;
	if(AutoStretch && m_textureUnit->GetTexture()){
		MainSize=math::vector2di(m_textureUnit->GetTexture()->getSize().x,m_textureUnit->GetTexture()->getSize().y);
	}
	math::rectf rc=getRect();
	if(getParent() && getParent()->EnableClip)
		rc.clipRect(getParent()->getClipRect());

	float angle=getAbsTransformationMat().getAngle();
	if(m_textureUnit->GetTexture())
		getDevice()->draw2DImageRot(rc,Origin(),m_textureUnit,video::SColor(Color().R,Color().G,Color().B,AlphaEffect ? Alpha:Color().A),
				Angle,&TexCoords());
	else 
		getDevice()->draw2DRectangleRot(rc,Origin(),video::SColor(Color().R,Color().G,Color().B,AlphaEffect ? Alpha:Color().A),Angle);

	Angle=Angle+RotationSpeed*dt;
	GUIElement::draw(dt);
}

void GUIRotImage::onEvent(const EventPtr& event){
	if(!parentEnabled)return;
}





}
}
