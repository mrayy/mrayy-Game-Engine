

#include "GUIAnimatedSprite.h"
#include "ITexture.h"

namespace mray{
namespace GUI{



GUIAnimatedSprite::GUIAnimatedSprite(GUIManager* manager,video::ITexturePtr  tex,int rowCnt,int colCnt,int framesCnt,float animSpeed,
									 math::vector2d pos,math::vector2d size,IEventReciver* event,GCPtr<GUIElement> parent,int id):
GUICellSprite(manager,tex,rowCnt,colCnt,framesCnt,pos,size,event,parent,id),speed(animSpeed),autoAnimate(1),m_frame(0),currCycle(-1)
{
	setTypeStr(mT("AnimatedSprite"));

}


GUIAnimatedSprite::~GUIAnimatedSprite(){
	cycles.clear();
}



void GUIAnimatedSprite::draw(float dt){
	if(autoAnimate){
		if(currCycle==-1)
			m_frame+=speed*dt;
		else{
			if(cycles[currCycle].loop)
				m_frame+=cycles[currCycle].speed*dt;
		}
	}
	m_frame=fmod(m_frame,100.0f);

	int currTex=0;
	if(currCycle==-1)
		currTex=(float)(m_frame/100.0f)*MaxFrames;
	else
		currTex=cycles[currCycle].start+(float)(m_frame/100.0f)*(cycles[currCycle].end-cycles[currCycle].start);

	setCell(currTex);

	GUICellSprite::draw(dt);
}

void GUIAnimatedSprite::setFrame(float f){
	m_frame=f;
}
float GUIAnimatedSprite::getFrame(){
	return m_frame;
}

void GUIAnimatedSprite::setAutoAnimate(bool a){
	autoAnimate=a;
}
bool GUIAnimatedSprite::getAutoAnimate(){
	return autoAnimate;
}

void GUIAnimatedSprite::setSpeed(float s){
	speed=s;
}
float GUIAnimatedSprite::getSpeed(){
	return speed;
}


int GUIAnimatedSprite::addCycle(SCycle&c){
	if(c.end<0)c.end=0;
	if(c.end>=MaxFrames)c.end=MaxFrames-1;

	if(c.start<0)c.start=0; 
	if(c.start>c.end)c.start=c.end;
		
	cycles.push_back(c);
	return cycles.size();
}
void GUIAnimatedSprite::setCycle(int c){
	if(c<0 || c>=cycles.size())return;
	currCycle=c;
}
void GUIAnimatedSprite::setCycle(const mchar*name){
	for(int i=0;i<cycles.size();++i)
		if(cycles[i].name.equals_ignore_case(name)){
			setCycle(i);
			return;
		}
}

int GUIAnimatedSprite::getCurrentCycle(){
	return currCycle;
}


}
}