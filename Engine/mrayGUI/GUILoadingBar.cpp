



#include "GUILoadingBar.h"
#include "GUIElement.h"
#include "GUIThemeManager.h"
#include "GUIManager.h"
#include "KeyCode.h"
#include "IEventReciver.h"
#include "TextureResourceManager.h"
#include "FontResourceManager.h"


namespace mray{
namespace GUI{

GUILoadingBar::GUILoadingBar(GUIManager* manager,float speed,int stepsCnt,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent,video::SColor color,int id)
:GUIElement(manager,parent,pos,size,0,id),stepSpeed(speed),repeat(0),current(0),stepsCount(stepsCnt)
,crColor(video::SColor(255,255,255,200)),isAuto(0),drawOutline(1),drawPercent(0)
{

	Selectable=false;
	Color=color;

	if(stepsCount==0)
		stepsCount=1;

	bkTex=new video::TextureUnit();
	crTex=new video::TextureUnit();
	layerTex=new video::TextureUnit();

	setTypeStr(mT("LoadingBar"));


}



GUILoadingBar::~GUILoadingBar()
{
}

void GUILoadingBar::setBackTexture(const  mchar*texName)
{
	if(!device)return;
	video::ITexturePtr  tex=gTextureResourceManager.loadTexture2D(texName);
	if(tex && tex->getType()!=video::ETT_2DTex){
		tex=0;
	}
	bkTex=tex;
/*	if(bkTex)
		bkTex->addRef();*/
}


void GUILoadingBar::setLoadingTexture(const  mchar*texName)
{
	if(!device)return;
	video::ITexturePtr  tex=gTextureResourceManager.loadTexture2D(texName);
	if(tex && tex->getType()!=video::ETT_2DTex){
		tex=0;
	}
	crTex=tex;
	/*if(crTex)
		crTex->addRef();*/
}
void GUILoadingBar::setLayerTexture(const  mchar*texName)
{
	if(!device)return;
	video::ITexturePtr  tex=gTextureResourceManager.loadTexture2D(texName);
	if(tex && tex->getType()!=video::ETT_2DTex){
		tex=0;
	}
	layerTex->SetTexture(tex);
/*	if(layerTex)
		layerTex->addRef();*/
}

void GUILoadingBar::loadTextures()
{
	return;
// 	if(!device)return;
// 	setBackTexture(core::string(core::string(getManager()->GuiDataPath())+mT("loadBar_back.tga")).c_str());
// 	setLoadingTexture(core::string(core::string(getManager()->GuiDataPath())+mT("loadBar_load.tga")).c_str());
// 	setLayerTexture(core::string(core::string(getManager()->GuiDataPath())+mT("loadBar_layer.tga")).c_str());
/*
	if(bkTex)
		bkTex->addRef();
	if(crTex )
		crTex->addRef();
	if(layerTex)
		layerTex->addRef();*/
}

void GUILoadingBar::addSteps(float step){
	current+=step;
	current=math::clamp<float>(current,0,stepsCount);
}


void GUILoadingBar::draw(float dt)
{
	if(!Visible)return;
	GUIElement::draw(dt);
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 	

	if(isAuto){
		current+=stepSpeed*dt;
		if(!repeat)
			current=math::clamp<float>(current,0,stepsCount);
		else
			current=math::cycle<float>(current,0,stepsCount);
		if(useLayer && layerTex){
			layerTex->trans.x+=0.1f*dt;
			layerTex->updateMatrix();
		}
	}

	math::rectf tRect=getRect();
	math::rectf tCurRect=tRect;
	tCurRect.BRPoint.x=tCurRect.ULPoint.x+current*tRect.getWidth()/(float)stepsCount;
	tCurRect.ULPoint.y+=1;
	tCurRect.BRPoint.y-=1;

	video::SColor bclr=Color();
	video::SColor crClr=crColor;
	bclr.A=AlphaEffect ? Alpha:Color().A;
	crClr.A=AlphaEffect ? Alpha:crColor.A;
	
	video::SColor clr;
	
	device->draw2DImage(tRect,bkTex,bclr);

	if(drawOutline)
		device->draw2DRectangle(tRect,video::SColor(0,0,0,255),0,&clipRect);
	
	if(!crTex->GetTexture()){
		skin->drawBox(getManager()->GetRenderQueue(),tCurRect,crClr,0,&clipRect,false);
	}
	else 
	{
		device->draw2DImage(tCurRect,crTex,crClr);
	}


	if(drawPercent){
		IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
		if(!font)
			return;
		int per=(int)100*current/(float)stepsCount;
		math::vector2d v=tCurRect.getCenter();
		v.y-=font->getCharDimension('A',m_FontAttributs.fontSize).y*0.5f;
//		font->print(math::rectf(v,0),&m_FontAttributs,0,core::StringConverter::toString(per)+mT("%"),guiMngr->GetRenderQueue());
	}


	if(useLayer && layerTex->GetTexture()){
		device->draw2DImage(tRect,layerTex,video::SColor(255,255,255,bclr.A));
	}

		
}


GCPtr<GUIElement> GUILoadingBar::getDuplicate()
{/*
	GUILoadingBar*dup=guiMngr->addSlider(minValue,maxValue,getPos(),getSize(),Func,getParent(),Color,getId());
	
	float loadSpeed;
	float maxLoad;
	bool repeat;

	float current;
	dup->value=value;

	if(bkTex)
		bkTex->addRef();
	if(crTex)
		crTex->addRef();
	dup->crTex=crTex;
	dup->bkTex=bkTex;
*/
	return 0;
}
FontAttributes* GUILoadingBar::getFontAttributes(){
	return &m_FontAttributs;
}


}
}

