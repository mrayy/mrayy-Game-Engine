
#include "IntroSenario.h"
#include "ImageIntroItem.h"
#include "VideoIntroItem.h"
#include "IEaglesManager.h"

#include "ScenarioExitCode.h"
#include <ICursorControl.h>

namespace mray{
namespace gameMod{

IntroSenario::IntroSenario():IScenarioNode()
{
	m_currItem=m_introItems.end();
}
IntroSenario::~IntroSenario(){
	m_introItems.clear();
}


void IntroSenario::onEnter(){
	gCursorControl.setVisible(0);
}

void IntroSenario::onExit(){
	gCursorControl.setVisible(1);
}

void IntroSenario::createVideoItem(GCPtr<video::IVideoTexture> v){
	GCPtr<VideoIntroItem> i=new VideoIntroItem(v);
	addIntroItem(i);
}
void IntroSenario::createImageItem(GCPtr<video::ITexture> t,float fadeTime,float freezeTime){
	GCPtr<ImageIntroItem> i=new ImageIntroItem(t,fadeTime,freezeTime);
	addIntroItem(i);
}

void IntroSenario::addIntroItem(GCPtr<IntroItem> item){
	m_introItems.push_back(item);
	item->setRect(math::rectf(0,0,1,1));
	if(m_currItem==m_introItems.end())
		m_currItem=m_introItems.begin();
}

void IntroSenario::update(float dt){
	GCPtr<video::IVideoDevice> device=eagles::IEaglesManager::getInstance().getVideoDevice();
	if(m_currItem!=m_introItems.end()){
		device->setClearColor(0);
		device->clearBuffer(video::EDB_COLOR);
		GCPtr<IntroItem> item=(*m_currItem);
		item->draw();
		item->update(dt);
		if(item->isDone()){
			m_currItem++;
		}
	}
}
void IntroSenario::onEvent(GCPtr<Event> event){
	if(event->getType()!=ET_Keyboard)
		return;
	if(m_currItem!=m_introItems.end()){
		(*m_currItem)->sendSkip();
	}
}

int IntroSenario::isDone(){
	if(m_currItem==m_introItems.end())
		return SenarioGeneric_Done;
	return 0;
}

}
}