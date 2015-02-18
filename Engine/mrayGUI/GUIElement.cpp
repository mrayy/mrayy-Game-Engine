
 
#include "GUIElement.h"
#include "GUIManager.h"
#include "GUIThemeManager.h"
#include "CPropertie.h"
#include "GUIEffector.h"
#include "FontResourceManager.h"
#include "MouseEvent.h"
#include "GUIEffectorFactory.h"
#include "LanguageFontSpecification.h"
#include "GUIElementFactory.h"

namespace mray{
namespace GUI{



GUIElement::GUIElement(GUIManager* manager):
	device(0),activeElem(0),parent(0),
	m_event(0),guiMngr(0),useDeviceTiming(0),
	Dock(this,&GUIElement::setDock,&GUIElement::getDock,mT("EDock"),mT("Dock")),
	Id(this,&GUIElement::setId,&GUIElement::getId,EPT_INT,mT("ID"),mT("Element ID")),
	Name(this,&GUIElement::setName,&GUIElement::getName,EPT_STRING,mT("Name"),mT("Element Name")),
	Visible(this,&GUIElement::setVisible,&GUIElement::getVisible,EPT_BOOLEAN,mT("Visible"),mT("Element Visibilty")),
	Selectable(this,&GUIElement::setSelectable,&GUIElement::getSelectable,EPT_BOOLEAN,mT("Selectable"),mT("can or can't select this element")),
	Enable(this,&GUIElement::setEnable,&GUIElement::getEnable,EPT_BOOLEAN,mT("Enable"),mT("disable/enable the element")),
	Origin(this,&GUIElement::setOrigin,&GUIElement::getOrigin,EPT_VECTOR2Df,mT("Origin"),mT("Element Origin")),
	MainSize(this,&GUIElement::setMainSize,&GUIElement::getMainSize,EPT_VECTOR2Df,mT("Size"),mT("Element Main Size")),
	MainPos(this,&GUIElement::setMainPos,&GUIElement::getMainPos,EPT_VECTOR2Df,mT("Pos"),mT("Element Main Position")),
	Color(this,&GUIElement::setColor,&GUIElement::getColor,EPT_COLOR,mT("BackColor"),mT("BackColor")),
	EnableClip(this,&GUIElement::setEnableClip,&GUIElement::getEnableClip,EPT_BOOLEAN,mT("UseClip"),mT("clip or not children of the element")),
	UseHint(this,&GUIElement::setUseHint,&GUIElement::getUseHint,EPT_BOOLEAN,mT("UseHint"),mT("disable/enable hint message")),
	Alpha(this,&GUIElement::setAlpha,&GUIElement::getAlpha,EPT_BOOLEAN,mT("Alpha"),mT("Alpha Value")),
	AlphaEffect(this,&GUIElement::setAlphaEffect,&GUIElement::getAlphaEffect,EPT_BOOLEAN,mT("AlphaEffect"),mT("makes element get Alpha transperncy on selection")),
	AlphaFade(this,&GUIElement::setAlphaFade,&GUIElement::getAlphaFade,EPT_FLOAT,mT("AlphaSpeed"),mT("speed value for Alpha effect")),
	MinAlpha(this,&GUIElement::setMinAlpha,&GUIElement::getMinAlpha,EPT_FLOAT,mT("minAlpha"),mT("minimum value for Alpha effect")),
	MaxAlpha(this,&GUIElement::setMaxAlpha,&GUIElement::getMaxAlpha,EPT_FLOAT,mT("maxAlpha"),mT("maximum value for Alpha effect")),
	ShowCaption(this,&GUIElement::setShowCaption,&GUIElement::getShowCaption,EPT_BOOLEAN,mT("ShowCaption"),mT("show/hide the Caption")),
	Caption(this,&GUIElement::setCaption,&GUIElement::getCaption,EPT_STRING,mT("Caption"),mT("Caption of the element")),
	HintStr(this,&GUIElement::setHintStr,&GUIElement::getHintStr,EPT_STRING,mT("Hint"),mT("Element Hint")),
	Angle(this,&GUIElement::setAngle,&GUIElement::getAngle,EPT_FLOAT,mT("Angle"),mT("Element Rotation Angle")),
	HintPos(this,&GUIElement::setHintPos,&GUIElement::getHintPos,EPT_VECTOR2Df)
{
	anchor=A_LEFT | A_TOP;
	m_dock=ED_NONE;

	Id=0;
	Visible=1;
	Selectable=0;
	Enable=1;
	MainSize=0;
	MainPos=0;
	Color=255;
	EnableClip=1;
	UseHint=0;
	AlphaEffect=0;
	m_Alpha=255;
	AlphaFade=1;
	MinAlpha=0.5;
	MaxAlpha=1;
	Angle=0;
	m_bShowCaption=1;

	m_lastPos=0;
	m_lastAngle=0;

	parentEnabled=1;

	SetManager(manager);
	if(manager){
		SetDevice(manager->getDevice());
	}
	updatePos();
	updateAbsTransformationMat(true);
	fillProperties();

};

GUIElement::GUIElement(GUIManager* manager,GUIElement* _parent,const math::Point2df& pos,const math::Point2df& size,
					   IEventReciver* event,int id):
	device(0),activeElem(0),parent(_parent),
	m_event(event),guiMngr(0),useDeviceTiming(0),
	Dock(this,&GUIElement::setDock,&GUIElement::getDock,mT("EDock"),mT("Dock")),
	Id(this,&GUIElement::setId,&GUIElement::getId,EPT_INT,mT("ID"),mT("Element ID")),
	Name(this,&GUIElement::setName,&GUIElement::getName,EPT_STRING,mT("Name"),mT("Element Name")),
	Visible(this,&GUIElement::setVisible,&GUIElement::getVisible,EPT_BOOLEAN,mT("Visible"),mT("Element Visibilty")),
	Selectable(this,&GUIElement::setSelectable,&GUIElement::getSelectable,EPT_BOOLEAN,mT("Selectable"),mT("can or can't select this element")),
	Enable(this,&GUIElement::setEnable,&GUIElement::getEnable,EPT_BOOLEAN,mT("Enable"),mT("disable/enable the element")),
	MainSize(this,&GUIElement::setMainSize,&GUIElement::getMainSize,EPT_VECTOR2Df,mT("Size"),mT("Element Size")),
	MainPos(this,&GUIElement::setMainPos,&GUIElement::getMainPos,EPT_VECTOR2Df,mT("Pos"),mT("Element Position")),
	Origin(this,&GUIElement::setOrigin,&GUIElement::getOrigin,EPT_VECTOR2Df,mT("Origin"),mT("Element Origin")),
	Color(this,&GUIElement::setColor,&GUIElement::getColor,EPT_COLOR,mT("BackColor"),mT("Back Color")),
	EnableClip(this,&GUIElement::setEnableClip,&GUIElement::getEnableClip,EPT_BOOLEAN,mT("UseClip"),mT("clip or not children of the element")),
	UseHint(this,&GUIElement::setUseHint,&GUIElement::getUseHint,EPT_BOOLEAN,mT("UseHint"),mT("disable/enable hint message")),
	Alpha(this,&GUIElement::setAlpha,&GUIElement::getAlpha,EPT_BOOLEAN,mT("Alpha"),mT("Alpha Value")),
	AlphaEffect(this,&GUIElement::setAlphaEffect,&GUIElement::getAlphaEffect,EPT_BOOLEAN,mT("AlphaEffect"),mT("makes element get Alpha transperncy on selection")),
	AlphaFade(this,&GUIElement::setAlphaFade,&GUIElement::getAlphaFade,EPT_FLOAT,mT("AlphaSpeed"),mT("speed value for Alpha effect")),
	MinAlpha(this,&GUIElement::setMinAlpha,&GUIElement::getMinAlpha,EPT_FLOAT,mT("minAlpha"),mT("minimum value for Alpha effect")),
	MaxAlpha(this,&GUIElement::setMaxAlpha,&GUIElement::getMaxAlpha,EPT_FLOAT,mT("maxAlpha"),mT("maximum value for Alpha effect")),
	ShowCaption(this,&GUIElement::setShowCaption,&GUIElement::getShowCaption,EPT_BOOLEAN,mT("ShowCaption"),mT("show/hide the Caption")),
	Caption(this,&GUIElement::setCaption,&GUIElement::getCaption,EPT_STRING,mT("Caption"),mT("Caption of the element")),
	HintStr(this,&GUIElement::setHintStr,&GUIElement::getHintStr,EPT_STRING,mT("Hint"),mT("Element Hint")),
	Angle(this,&GUIElement::setAngle,&GUIElement::getAngle,EPT_FLOAT,mT("Angle"),mT("Element Rotation Angle")),
	HintPos(this,&GUIElement::setHintPos,&GUIElement::getHintPos,EPT_VECTOR2Df)
{
	anchor=A_LEFT | A_TOP;
	m_dock=ED_NONE;
	Id=id;
	Visible=1;
	Selectable=0;
	Enable=1;
	m_mainSize=size;
	m_mainPos=pos;
	Color=255;
	EnableClip=1;
	UseHint=0;
	m_Alpha=255;
	AlphaEffect=0;
	AlphaFade=1;
	MinAlpha=0.5;
	MaxAlpha=1;
	Angle=0;
	m_lastPos=0;
	m_lastAngle=0;
	m_bShowCaption=1;

	clipRect=math::rectf(pos,(pos+size));
	parentEnabled=1;
	if(parent)
	{
		parent->addChild(this);
		parentPos=parent->getPos();
		clipRect+=parentPos;
		clipParentPos();
		parentEnabled=parent->parentEnabled;
	//	clipRect.clipRect(parent->getClipRect());
	}

	SetManager(manager);
	if(manager){
		SetDevice(manager->getDevice());
	}

	updatePos();
	updateAbsTransformationMat(true);

	fillProperties();

}

GUIElement::~GUIElement(){
	removeAll();
}
void GUIElement::fillProperties(){

	addPropertie(&Id,mT("Design"));
	addPropertie(&Name,mT("Design"));

	addPropertie(&Visible,mT("Behavior"));
	addPropertie(&Enable,mT("Behavior"));
	addPropertie(&Selectable,mT("Behavior"));
	addPropertie(&EnableClip,mT("Behavior"));
	addPropertie(&HintStr,mT("Behavior"));
	addPropertie(&UseHint,mT("Behavior"));

	addPropertie(&Angle,mT("Layout"));
	addPropertie(&MainPos,mT("Layout"));
	addPropertie(&MainSize,mT("Layout"));
	addPropertie(&Origin,mT("Layout"));
	addPropertie(&Dock,mT("Layout"));

	addPropertie(&Caption,mT("Apperance"));
	addPropertie(&ShowCaption,mT("Apperance"));
	addPropertie(&Color,mT("Apperance"));
	addPropertie(&AlphaEffect,mT("Apperance"));
	addPropertie(&AlphaFade,mT("Apperance"));
	addPropertie(&MinAlpha,mT("Apperance"));
	addPropertie(&MaxAlpha,mT("Apperance"));
}

const core::UTFString& GUIElement::getCaption() 
{ 
	return m_Caption; 
}
void GUIElement::setCaption(const core::UTFString&  val) 
{ 
	m_renderText[ETT_Caption].dirty=true;
	m_Caption = val; 
}



const core::string& GUIElement::getName() { return m_name; }
void GUIElement::setName(const core::string&  val) { m_name = val; }

const bool& GUIElement::getVisible() { return m_isVisible; }
void GUIElement::setVisible(const bool& val) { m_isVisible = val; }

const bool& GUIElement::getEnable() { return m_isEnable; }
void GUIElement::setEnable(const bool& val) { m_isEnable = val; }

const bool& GUIElement::getEnableClip() { return m_enableClip; }
void GUIElement::setEnableClip(const bool& val) { m_enableClip = val; }


const core::UTFString &GUIElement::getHintStr() { return m_hintStr; }
void GUIElement::setHintStr(const core::UTFString& val) 
{
	m_renderText[ETT_Hint].dirty=true;
	m_hintStr = val; 
}

const bool& GUIElement::getUseHint() { return m_useHint; }
void GUIElement::setUseHint(const bool& val) { m_useHint = val; }

const bool& GUIElement::getShowCaption() { return m_bShowCaption; }
void GUIElement::setShowCaption(const bool& val) { m_bShowCaption = val; }

const math::Point2df& GUIElement::getMainPos() { return m_mainPos; }

const math::Point2df&  GUIElement::getMainSize() { return m_mainSize; }



const int& GUIElement::getId() { return m_Id; }
void GUIElement::setId(const int& val) { m_Id = val; }

const bool& GUIElement::getSelectable() { return m_Selectable; }
void GUIElement::setSelectable(const bool& val) { m_Selectable = val; }

const bool& GUIElement::getAlphaEffect() { return m_bAlphaEffect; }
void GUIElement::setAlphaEffect(const bool& val) { m_bAlphaEffect = val; }

const float& GUIElement::getAlphaFade() { return m_AlphaFade; }
void GUIElement::setAlphaFade(const float& val) { m_AlphaFade = val; }

const float& GUIElement::getMinAlpha() { return m_minAlpha; }
void GUIElement::setMinAlpha(const float& val) { m_minAlpha = val; }

const float& GUIElement::getMaxAlpha()  { return m_maxAlpha; }
void GUIElement::setMaxAlpha(const float& val) { m_maxAlpha = val; }

const video::SColor& GUIElement::getColor() { return m_Color; }
void GUIElement::setColor(const video::SColor& val) { m_Color = val; }

const float& GUIElement::getAlpha() { return m_Alpha; }
void GUIElement::setAlpha(const float& val) { m_Alpha = val; }

const float& GUIElement::getAngle() { return m_angle; }
void GUIElement::setAngle(const float& val) { m_angle = val; }

const math::vector2d& GUIElement::getOrigin() { return m_origin; }
void GUIElement::setOrigin(const math::vector2d &val) { m_origin = val; }


core::UTFString& GUIElement::_UpdateRenderText(SRenderText& t,const core::UTFString&str)
{
	if(t.dirty){
		t.dirty=false;
		if(guiMngr )
			m_renderText[ETT_Caption].str=guiMngr->GetLanguageSpecification()->ProcessText(str);
	}
	return t.str;
}

const core::UTFString& GUIElement::getRenderString()
{
	return _UpdateRenderText(m_renderText[ETT_Caption],m_Caption);
}

void GUIElement::SetDevice(video::IVideoDevice* dev){
	device=dev;
	std::list<GCPtr<GUIElement> >::iterator it= Children.begin();
	for(;it!=Children.end();++it){
		(*it)->SetDevice(dev);
	}
}
void GUIElement::SetManager(GUIManager* mngr){
	guiMngr=mngr;
	std::list<GCPtr<GUIElement> >::iterator it= Children.begin();
	for(;it!=Children.end();++it){
		(*it)->SetManager(mngr);
	}
};

video::IVideoDevice* GUIElement::getDevice(){return device;}
GUIManager* GUIElement::getManager(){return guiMngr;}

void GUIElement::setReciver(IEventReciver*event){m_event=event;}

void GUIElement::addEffector(const GUIEffectorPtr&eff){
	effectors.push_back(eff);
//	eff->addRef();
}

FontAttributes* GUIElement::getFontAttributes(){
	return 0;
}

void GUIElement::removeEffector(const GUIEffectorPtr&eff){

	std::list<GCPtr<GUIEffector>>::iterator it=effectors.begin();
	for(;it!=effectors.end();++it)
		if((*it)==eff){
			effectors.remove(it);
			return;
		}
}
bool GUIElement::bringToFront(GUIElement* elem)
{
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			GCPtr<GUIElement> tmp=elem;
			Children.remove(it);
			Children.push_back(elem);
			return 1;
		}
	}
	return 0;
}
bool GUIElement::dropToBack(GUIElement* elem)
{
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			GCPtr<GUIElement> tmp=elem;
			Children.remove(it);
			Children.push_front(elem);
			return 1;
		}
	}
	return 0;
}
void GUIElement::move(math::vector2d &abs)
{
	MainPos=MainPos()+abs;
	updatePos();
}

void GUIElement::setAnchor(int a){
	anchor=a;
}
void GUIElement::updateAnchor(){
	if(anchor == A_CENTER){
		if(parent){
			m_Pos=(parent->getSize() - getSize())*0.5;
		}else
			m_Pos=m_mainPos;
		return;
	}

	if(anchor & A_LEFT){
		m_Pos.x=m_mainPos.x;
		if(anchor & A_RIGHT){
			if(parent)
				m_Pos.x=(parent->getSize().x -parent->getMainSize().x ) +m_mainPos.x;
		}
	}else
		if(anchor & A_RIGHT){
			if(parent)
				m_Pos.x=(parent->getSize().x -parent->getMainSize().x ) +m_mainPos.x;
		}
	if(anchor & A_TOP){
		m_Pos.y=m_mainPos.y;
	}
	if(anchor & A_BOTTOM){
		if(parent)
			m_Pos.y=(parent->getSize().y -parent->getMainSize().y ) +m_mainPos.y;
	}
}
void GUIElement::updateDock(){

	if(ED_NONE)
		return;
	m_Pos=m_mainPos;
	m_Size=m_mainSize;
	math::vector2d psize;
	if(parent){
		psize=parent->getSize();
	}else{
		if(getManager() && getManager()->getDevice())
			psize=getManager()->getDevice()->getViewportRect().getSize();
		else
			psize=m_mainSize;
	}
	switch(m_dock){
	case ED_FILL:
		m_Pos=0;
		m_Size=psize;
		break;
	case ED_LEFT:
		m_Size.x=m_mainSize.x;
		m_Pos=0;
		m_Size.y=psize.y;
		break;
	case ED_RIGHT:
		m_Size.x=m_mainSize.x;
		m_Pos.x=psize.x-m_Size.x;
		m_Pos.y=0;
		m_Size.y=psize.y;
		break;
	case ED_TOP:
		m_Size.y=m_mainSize.y;
		m_Pos=0;
		m_Size.x=psize.x;
		break;
	case ED_BOTTOM:
		m_Size.y=m_mainSize.y;
		m_Pos.x=0;
		m_Pos.y=psize.y-m_Size.y;
		m_Size.x=psize.x;
		break;
	}
}
void GUIElement::preDraw(float dt){
	updatePos();


	if(AlphaEffect){
		if(getManager()->hasMouseOn(this) || getManager()->hasFocus(this)){
			if(!useDeviceTiming){
				useDeviceTiming=1;
//				device->increaseDrawingObjects();
			}
			if(AlphaFade<=0)
				Alpha=MaxAlpha;
			else 
				Alpha=math::clamp<float>(Alpha+dt*AlphaFade,MinAlpha,MaxAlpha);

		}else{
			if(AlphaFade<=0){
				Alpha=MinAlpha;
				if(useDeviceTiming){
					useDeviceTiming=0;
//					device->decreaseDrawingObjects();
				}
			}
			else
				Alpha=math::clamp<float>(Alpha-dt*AlphaFade,MinAlpha,MaxAlpha);
		}

	}
	std::list<GCPtr<GUIEffector>>::iterator eit=effectors.begin();
	for(;eit!=effectors.end();e++it)
	{
		(*eit)->effect(this,dt);
	}


	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		(*it)->preDraw(dt);
	}
}


void GUIElement::draw(float dt)
{
	if(!Visible)
		return;
	if(parent && (!parent->Enable || !parent->parentEnabled) && parentEnabled)
		parentEnabled=0;
	if(parent && (parent->Enable && parent->parentEnabled) && !parentEnabled)
		parentEnabled=1;

	if(getManager() && m_bShowHint && UseHint){
		getManager()->showHint(1);
		getManager()->setHintPos(m_hintPos);
//		getManager()->setHintString(HintStr);
	}
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		(*it)->draw(dt);
	}
}

void GUIElement::drawHint()
{
//#define
	if(!UseHint || !m_bShowHint || !getManager())return;
	IGUITheme* skin=GUIThemeManager::getInstance().getActiveTheme(); 

	if(!skin)
		return;
	FontAttributes*fattr=skin->getHintFontAttr();
	GUI::IFont*font=gFontResourceManager.getFontByName(fattr->fontName);
	if(!skin || !font)return;

	math::vector2d charDim;//=font->getDimension(mT("A"),fontSize);

	math::rectf rc(m_hintPos,m_hintPos);
	_UpdateRenderText(m_renderText[ETT_Hint],m_hintStr);
	rc.BRPoint.x+=m_renderText[ETT_Hint].str.Length()*(charDim.x);
	rc.BRPoint.y+=charDim.y;
	video::IVideoDevice*dev=getManager()->getDevice();
	if(rc.BRPoint.x>dev->getViewportRect().BRPoint.x)
		rc-=math::vector2d(rc.BRPoint.x-dev->getViewportRect().BRPoint.x -20,0);
	if(rc.BRPoint.y>dev->getViewportRect().BRPoint.y)
		rc-=math::vector2d(0,rc.BRPoint.y-dev->getViewportRect().BRPoint.y);

	rc-=math::vector2d(10,0);
	skin->drawSizableBox(getManager()->GetRenderQueue(),rc,0,mT("Frame"),video::SColor(250,250,250,255));
	font->print(rc,fattr,0,m_renderText[ETT_Hint].str,guiMngr->GetRenderQueue());
	

}

void GUIElement::onEvent(const EventPtr& e){	
	if(!Visible)return;
	if(!Enable)return;
	if(!parentEnabled)return;
	if(activeElem)
		activeElem->onEvent(e);
	else{
		/*
		if(e->getType()==ET_Mouse){
			GCPtr<MouseEvent> event=e;
			if(getRect().IsPointInside(math::vector2d(event->x,event->y))){
				switch(event->Event){
				case MET_LEFTDOWN:
					break;
				case MET_LEFTUP:
					break;
				case MET_MOVED:
					break;
				}
			}else{
			}
		}*/
		std::list<GCPtr<GUIElement> >::iterator it=Children.last();
		for(;it!=Children.end()&& (!e->isRecived());it--)
			(*it)->onEvent(e);
	}
 }




float GUIElement::getWidth(){return m_Size.x;}
float GUIElement::getHeight(){return m_Size.y;}

GUIElement*  GUIElement::getParent(){return parent;}

void GUIElement::loadTextures(){};


math::rectf GUIElement::getRect()const{
	return math::rectf(getAbsPos(),getAbsPos()+m_Size);
};

void GUIElement::setParentPos(math::Point2df p){parentPos=p;}
math::Point2df GUIElement::getParentPos()const{
	math::Point2df pos=0;
	if(parent){
		pos=parent->getPos()+parent->getParentPos();
	}
	return pos;
}

void GUIElement::setMainPos(const math::Point2df&  val)
{
	m_mainPos=val;
	m_Pos=val;
	clipRect=math::rectf(m_Pos,(m_Pos+m_Size));

	this->updatePos();
}
void GUIElement::setMainSize(const math::Point2df&  val)
{
	m_mainSize=val;
	m_Size=val;
	clipRect=math::rectf(m_Pos,(m_Pos+m_Size));

	this->updatePos();
}

GCPtr<GUIElement> GUIElement::getDuplicate(){return 0;};


void GUIElement::removeFromParent()
{
	if(parent)
		parent->removeChild(this);
}

void GUIElement::removeChild(GUIElement* elem)
{
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		if((*it).pointer()==elem)
		{
			(*it)->parent=0;
			Children.remove(it);
			return;
		}
	}
}

void GUIElement::addChild(GUIElement* elem)
{
	if(elem)
	{
		elem->removeFromParent();
		elem->parent=this;
		Children.push_back(elem);
		updatePos();
	}
}


GUIElement*GUIElement::getElementFromPoint(math::vector2d pos)
{
	if(!Visible)return GUIElementPtr::Null;
	std::list<GCPtr<GUIElement> >::iterator it=Children.last();
	for(;it!=Children.end();)
	{
		const GUIElementPtr& elem=(*it)->getElementFromPoint(pos);
		if(elem)
			return elem;

		it--;
	}

	if(getRect().IsPointInside(pos))
		return this;
	return GUIElementPtr::Null;
}

void GUIElement::updatePos()
{
	if(parent)
		setParentPos(parent->getPos()+parent->getParentPos());
	m_Size=MainSize;
	updateDock();
	if(m_dock==ED_NONE)
		updateAnchor();
	clipParentPos();
	updateAbsTransformationMat(true);
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it){
		(*it)->updatePos();
	//	(*it)->setParentPos(Pos);
	}
} 


bool GUIElement::hasChanged()const{
	return getPos()!=m_lastPos || Angle()!=m_lastAngle;
}
bool GUIElement::updateRelTransformationMat(){
	if(hasChanged()){
		m_relTransformationMatrix.setAngle(m_angle);
		m_relTransformationMatrix.translate(getPos());

		m_lastAngle=m_angle;
		m_lastPos=getPos();
		return true;
	}
	return false;
}
bool GUIElement::updateAbsTransformationMat(bool shouldUpdate){
	bool u=updateRelTransformationMat();
	bool res=false;
	if(shouldUpdate || u){
		if(getParent()){
			m_absTransformationMatrix=getParent()->getAbsTransformationMat()*m_relTransformationMatrix;
			res=true;
		}
		else {
			if(u){
				m_absTransformationMatrix=m_relTransformationMatrix;
				res=true;
			}else
				res=false;
		}
		if(res){
			std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
			for(;it!=Children.end();++it){
				(*it)->updateAbsTransformationMat(true);
			}
		}
	}
	return res;
}
const math::matrix3x3 &GUIElement::getAbsTransformationMat()const{
	return m_absTransformationMatrix;
}
const math::matrix3x3 &GUIElement::getRelTransformationMat()const{
	return m_relTransformationMatrix;
}
math::vector2d GUIElement::getAbsPos()const{
	return m_absTransformationMatrix.getTranslate();
}

void GUIElement::clipParentPos()
{
	clipRect=getRect();
	if(parent)
	{
		if(parent->EnableClip){
			clipRect.clipRect(parent->getClipRect());
		}
	}
}
void GUIElement::setActiveElem(GUIElement* elem){
	activeElem=elem;
}
GUIElement*  GUIElement::getActiveElem(){
	return activeElem;
}

GUIElement *GUIElement::getElementById(int id,bool searchChildren)
{
	GUIElement *et;
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		if((*it)->Id()==id)
			return (*it);
		if(searchChildren)
			et=(*it)->getElementById(id,searchChildren);
		if(et)return et;
	}
	return 0;
}
GUIElement *GUIElement::getElementByName(const core::string&name,bool searchChildren){
	GUIElement *et;
	if(m_name==name)
		return this;
	if(searchChildren){
		std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
		for(;it!=Children.end();++it)
		{
	// 		if((*it)->m_name==name)
	// 			return (*it);
			et=(*it)->getElementByName(name,true);
			if(et)return et;
		}
	}
	return 0;
}

bool GUIElement::isElementChild(GUIElement*  elem)
{
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		if((*it).pointer()==elem)
			return 1;
	}
	return 0;
}

GUIElement*GUIElement::getNextChild(GUIElement* elem)
{
	if(this==elem && Children.begin()!=Children.end())
		return *Children.begin();
	bool found=0;
	GCPtr<GUIElement> et=0;
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	std::list<GCPtr<GUIElement> >::iterator tmp;
	for(;it!=Children.end();++it)
	{
		if((!(*it)->isElementChild(elem) && (*it).pointer()!=elem )&& !found)continue;
		if(!found){
			if(elem->Visible && elem->Selectable){
			et=(*it)->getNextChild(elem);
			if(et)
				return et;
			}
		}else{
			return *it;
			tmp=(*it)->getChildren()->begin();
			if(tmp!=elem->getChildren()->end())
				return *tmp;
		}
		found=1;
	}
	return 0;
}
void GUIElement::removeAll()
{
	std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
	for(;it!=Children.end();++it)
	{
		(*it)->parent=0;
	}
	Children.clear();
	effectors.clear();
}




	

void GUIElement::loadXMLSettings(xml::XMLElement* elem){
	if(!elem)return;
	IObject::loadXMLSettings(elem);
	{
		xml::xmlSubElementsMapIT it= elem->getElementsBegin();
		xml::xmlSubElementsMapIT end= elem->getElementsEnd();

		for (;it!=end;++it)
		{
			xml::XMLElement*e=*it;
			if(e->getName().equals_ignore_case(mT("Font"))){
				if(getFontAttributes())
					getFontAttributes()->loadXMLSettings(e);
			}else if(e->getName().equals_ignore_case(mT("Children"))){
				xml::xmlSubElementsMapIT it2= e->getElementsBegin();
				xml::xmlSubElementsMapIT end2= e->getElementsEnd();
				for(;it2!=end2;it2++){
					xml::XMLElement*e2=*it2;
					GCPtr<GUIElement> ce=GUIElementFactory::getInstance().createElement(e2->getName());
					if(ce){
						ce->loadXMLSettings(e2);
						addChild(ce);
					}
				}
			}else if(e->getName().equals_ignore_case(mT("Effectors"))){
				xml::xmlSubElementsMapIT it2= e->getElementsBegin();
				xml::xmlSubElementsMapIT end2= e->getElementsEnd();
				for(;it2!=end2;it2++){
					xml::XMLElement*e2=*it2;
					GCPtr<GUIEffector> ce=GUIEffectorFactory::getInstance().createEffector(e2->getName());
					if(ce){
						ce->loadXMLSettings(e2);
						addEffector(ce);
					}
				}
			}
		}
	}
}

xml::XMLElement*  GUIElement::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement* node=IObject::exportXMLSettings(elem);
/*
	if(SndPlayedMOn && sndMouseOn && sndMouseOn->getData()){
		node->addAttribute(mT("MouseOnSnd"),sndMouseOn->getData()->getResourceName());
	}

	if(SndPlayedMDown && sndMouseDown && sndMouseDown->getData()){
		node->addAttribute(mT("MouseDownSnd"),sndMouseDown->getData()->getResourceName());
	}
*/

	if(Children.getSize()>0){
		xml::XMLElement* child=new xml::XMLElement(mT("Children"));
		node->addSubElement(child);
		std::list<GCPtr<GUIElement> >::iterator it=Children.begin();
		for(;it!=Children.end();++it){
			(*it)->exportXMLSettings(child);
		}
	}
	if(effectors.getSize()>0){
		xml::XMLElement* eff=new xml::XMLElement(mT("Effectors"));
		node->addSubElement(eff);
		std::list<GCPtr<GUIEffector> >::iterator it=effectors.begin();
		for(;it!=effectors.end();++it){
			(*it)->exportXMLSettings(eff);
		}
	}

	if(getFontAttributes())
	{
		getFontAttributes()->exportXMLSettings(node);
	}
	return node;
}

void GUIElement::showHint(bool s){
	this->m_bShowHint=s;
}


}
}
