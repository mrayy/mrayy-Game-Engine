
#include "stdafx.h"
#include "IntroSenario.h"
#include "CharacterDemoGlobals.h"

#include "ScenarioExitCode.h"

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

VideoIntroItem* IntroSenario::createVideoItem(GCPtr<video::IVideoTexture> v){
	GCPtr<VideoIntroItem> i=new VideoIntroItem(v);
	addIntroItem(i);
	return i;
}
ImageIntroItem* IntroSenario::createImageItem(GCPtr<video::ITexture> t,float fadeTime,float freezeTime){
	GCPtr<ImageIntroItem> i=new ImageIntroItem(t,fadeTime,freezeTime);
	addIntroItem(i);
	return i;
}

void IntroSenario::addIntroItem(GCPtr<IntroItem> item){
	m_introItems.push_back(item);
	item->setRect(math::rectf(0,0,1,1));
	if(m_currItem==m_introItems.end())
		m_currItem=m_introItems.begin();
}

void IntroSenario::draw()
{
	GCPtr<video::IVideoDevice> device=CharacterDemoGlobals::getInstance()->getVideoDevice();
	if(m_currItem!=m_introItems.end()){
		device->setClearColor(0);
		device->clearBuffer(video::EDB_COLOR);
		IntroItem* item=(*m_currItem);
		item->draw();
	}
}

void IntroSenario::update(float dt){
	if(m_currItem!=m_introItems.end()){
		IntroItem* item=(*m_currItem);
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