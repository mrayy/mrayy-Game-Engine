
#include "GUIHistogram.h"

#include "GUIManager.h"
#include "CPropertie.h"
#include "FontResourceManager.h"
  
namespace mray{
namespace GUI{

GUIHistogram::GUIHistogram(GUIManager* manager,const  mchar*Caption,int maxSamples,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent):
GUIElement(manager,parent,pos,size,0,0),samplesSum(0),average(0),samplesCnt(0),sampleInterval(0.1),m_tm(0),
MaxSamples(this,&GUIHistogram::setMaxSamples,&GUIHistogram::getMaxSamples,EPT_INT,mT("maxSamples"),mT("max samples can hold"))
{
	MaxSamples=maxSamples;
	setCaption(Caption);
	this->m_maxVal=1;
	samples.reallocate(2*MaxSamples());

	setTypeStr(mT("Histogram"));
	fillProperties();
}

GUIHistogram::~GUIHistogram(){
	samples.clear();
}
void GUIHistogram::fillProperties(){
	addPropertie(&MaxSamples,mT("Behavior"));
}

void GUIHistogram::setSampleInterval(float interval){
	sampleInterval=interval;
}

void GUIHistogram::fixSamples(){
	
	if(samples.size()<2*MaxSamples())return;

	for(int i=0;i<MaxSamples();++i){
		samples[i]=samples[i+MaxSamples()];
	}

	samples.resize(MaxSamples());
}

void GUIHistogram::addSample(float val){

	samples.push_back(val);
	if(val>m_maxVal)
		m_maxVal=val;

	samplesSum+=val;
	samplesCnt++;
	if(samplesCnt)
		average=samplesSum/(float)samplesCnt;

	fixSamples();

}

void GUIHistogram::draw(float dt){
	if(!Visible)return;

	math::rectf rc=getRect();
	
	getDevice()->draw2DRectangle(rc,video::SColor(255,255,255,200),1,0);
	getDevice()->draw2DRectangle(rc,video::SColor(0,0,0,255),0,0);

	m_tm+=dt;
	if(m_tm>sampleInterval){
		m_tm=0;
		addSample(getSample());
	}

	if(Visible || !MaxSamples() || !m_maxVal || !samples.size())return;
	int start;
	if(samples.size()<MaxSamples())
		start=0;
	else start=samples.size()-MaxSamples();

	float xRatio=rc.getWidth()/(float)MaxSamples();
	float yRatio=rc.getHeight()/m_maxVal;

	math::vector2d rp=rc.ULPoint;
	rp.y+=rc.getHeight();
	math::vector2d lp;
	lp.x=0;
	lp.y=-samples[start]*yRatio;
/*
	glDisable(GL_DEPTH_TEST);
	getDevice()->set2DMode();*/
	for(int i=start+1;i<samples.size();++i){
		math::vector2d p;
		p.x=(i-start)*xRatio;
		p.y=-samples[i]*yRatio;
		getDevice()->draw2DLine(rp+lp,rp+p,video::SColor(0,0,0,255));
		lp=p;
	}

	GUI::IFont*font=gFontResourceManager.getFontByName(m_FontAttributs.fontName);
	if(!font)return;
//	font->print(math::rectf(rc.ULPoint+math::vector2d(3),0),&m_FontAttributs,0,core::StringConverter::toString(m_maxVal),guiMngr->GetRenderQueue());
	rp.y-=font->getCharDimension(' ',m_FontAttributs.fontSize).y;
	core::string msg=getCaption()+core::StringConverter::toString(samples.getLast())
		+core::StringConverter::toString(average);
//	font->print(math::rectf(rp,0),&m_FontAttributs,0,msg,guiMngr->GetRenderQueue());


	GUIElement::draw(dt);
	
}

FontAttributes* GUIHistogram::getFontAttributes(){
	return &m_FontAttributs;
}


}
}

