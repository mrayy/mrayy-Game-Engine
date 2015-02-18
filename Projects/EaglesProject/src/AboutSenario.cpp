
#include "AboutSenario.h"
#include "IEaglesManager.h"
#include "ScenarioExitCode.h"

#include <GUIStaticText.h>
#include <GUIImage.h>
#include <GUIDummy.h>
#include <TextureResourceManager.h>
#include <SoundManagerFactory.h>
#include <IOSystem.h>
#include <IFileSystem.h>
#include <KeyboardEvent.h>
#include <ILogManager.h>
#include <ICursorControl.h>

namespace mray{
namespace gameMod{

class CAboutText:public IAboutItem
{
public:
	CAboutText(const mchar*text,AboutSenario*credits,math::vector2d size,bool title):IAboutItem(title)
	{
		GCPtr<GUI::GUIStaticText> t=credits->getGUIManager()->addStaticText(text,0,size);
		t->Background=false;
		m_elem=t;
	}
	virtual void update(float dt){
	}

	virtual void setAlpha(float a){
		getElem()->getFontAttributes()->fontColor.A=a;
		getElem()->getFontAttributes()->shadowColor.A=a;
	}

};

class CAboutPicture:public IAboutItem
{
public:
	CAboutPicture(GCPtr<video::ITexture>tex,AboutSenario*credits,math::vector2d size,bool title):IAboutItem(title)
	{
		GCPtr<GUI::GUIImage> t=credits->getGUIManager()->addImage(0,size);
		t->setTexture(tex);
		m_elem=t;
	}
	virtual void update(float dt){

	}
};

AboutSenario::AboutSenario(){
	m_guiMngr=new GUI::GUIManager(eagles::IEaglesManager::getInstance().getVideoDevice());
	m_sndMngr=sound::SoundManagerFactory::getInstance().
		createSoundManager(eagles::IEaglesManager::getInstance().getSoundManagerType().c_str());

	FATAL_ERROR(!m_sndMngr,mT("Sound Manager Not Created"));
	m_bgSound=m_sndMngr->loadSound(mT("Credites.mp3"),1,sound::ESNDT_2D);

	m_state=0;
	m_sorted=0;
	m_maxY=0;
	m_currentState=0;
	m_bgAlpha=0;

	m_fastDisplay=0;

	m_imageController=new ImageController();


	GCPtr<GUI::GUIImage> bgImg=m_guiMngr->addImage(0,1);
	bgImg->Dock=GUI::ED_FILL;
	bgImg->setTexture(gTextureResourceManager.loadTexture2D(mT("AboutBG.png")));


	{
		GCPtr<OS::IDirOS>fos=gOSystem.createDirSystem();
		core::string path=gFileSystem.getAppPath();
		fos->changeDir(path.c_str());
		fos->changeDir(mT("Assets/Credits"));

		int cnt=fos->getFilesCount();
		core::string fname,theme;

		for(int i=0;i<cnt;++i){
			if(!fos->isFileDirectory(i)){
				fname=fos->getFullFileName(i);
				GCPtr<video::ITexture> t=gTextureResourceManager.loadTexture2D(fname);
				if(t){
					GCPtr<GUI::GUIRotImage> img=m_guiMngr->addRotateImage(0,256);
					img->setTexture(t);
					m_imageController->addImage(img);
				}
			}
		}

		fos->changeDir(path.c_str());
	}


	m_backPanel=m_guiMngr->addDummy(math::vector2d(50,0));

	{
		IAboutItem*item=0;

		item= new CAboutText(L"Syrian University",this,math::vector2d(400,30),0);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,100,100,255));
		addItem(item);

		item= new CAboutText(L"4Th Year - AI",this,math::vector2d(400,30),0);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,100,100,255));
		addItem(item);

		item= new CAboutText(L"Virtual Reality Project",this,math::vector2d(400,30),0);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,100,100,255));
		addItem(item);


		item= new CAboutText(L"Programming",this,math::vector2d(400,40),1);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,0,0,255));
		item->setSpace(20);
		addItem(item);
		item= new CAboutText(L"Mohamad Yamen Saraiji",this,math::vector2d(400,40),0);
		addItem(item);

		item= new CAboutText(L"Modeling",this,math::vector2d(400,40),1);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,0,0,255));
		item->setSpace(20);
		addItem(item);
		item= new CAboutText(L"Mohamad Yamen Saraiji",this,math::vector2d(400,40),0);
		addItem(item);

		item= new CAboutText(L"Background Image",this,math::vector2d(400,40),1);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,0,0,255));
		item->setSpace(20);
		addItem(item);
		item= new CAboutText(L"Amjad Al-Asady",this,math::vector2d(400,40),0);
		addItem(item);

		item= new CAboutText(L"Sounds & Music",this,math::vector2d(400,40),0);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(255,0,0,255));
		item->setSpace(20);
		addItem(item);
		item= new CAboutText(L"Unreal Tournament",this,math::vector2d(400,40),0);
		addItem(item);
		item= new CAboutText(L"Blizzard",this,math::vector2d(400,40),0);
		addItem(item);



		item= new CAboutText(L"all Rights Reserved for",this,math::vector2d(400,40),0);
		item->getElem()->getFontAttributes()->fontColor=(video::SColor(250,0,0,255));
		item->setSpace(30);
		addItem(item);
		item= new CAboutPicture(gTextureResourceManager.loadTexture2D(mT("mrayLogo.png")),this,math::vector2d(400,200),0);
		addItem(item);

	}


	m_fadeOutImg=m_guiMngr->addImage(0,1);
	m_fadeOutImg->Dock=GUI::ED_FILL;
	m_fadeOutImg->Visible=false;
	m_fadeOutImg->Color=video::SColor(0,0,0,0);
}
AboutSenario::~AboutSenario(){
}

GCPtr<GUI::GUIManager> AboutSenario::getGUIManager(){
	return m_guiMngr;
}
void AboutSenario::onEnter(){
	m_oldGuiMngr=eagles::IEaglesManager::getInstance().getActiveGUIManager();
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_guiMngr);

	m_oldSndMngr=eagles::IEaglesManager::getInstance().getActiveSoundManager();
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_sndMngr);

	if(m_bgSound)
		m_bgSound->play();

	sort();

	m_backPanel->MainPos=math::vector2d(50,m_guiMngr->getDevice()->getViewportRect().getSize().y);

	m_imageController->shuffle();

	m_state=0;
	m_currentState=0;
	m_fadeOutImg->Visible=false;
	m_fadeOutImg->Color=video::SColor(0,0,0,0);
	m_bgAlpha=0;
	gCursorControl.setVisible(0);

}
void AboutSenario::onExit(){
	eagles::IEaglesManager::getInstance().setActiveGUIManager(m_oldGuiMngr);
	eagles::IEaglesManager::getInstance().setActiveSoundManager(m_oldSndMngr);
	m_oldGuiMngr=0;
	m_oldSndMngr=0;
	gCursorControl.setVisible(1);
	if(m_bgSound)
		m_bgSound->stop();
}

void AboutSenario::onEvent(GCPtr<Event> event){
	if(event->getType()==ET_Keyboard)
	{
		GCPtr<KeyboardEvent> e=event;
		if(e->key==KEY_ESCAPE){
			m_fastDisplay=e->press;
		}
	}
}

void AboutSenario::update(float dt){

	switch (m_currentState)
	{
	case 0:
		m_imageController->update(dt);

		if(m_backPanel->MainPos().y+m_maxY>0){
			m_backPanel->MainPos=(m_backPanel->MainPos()-math::vector2d(0,20*dt*(1+2*m_fastDisplay)));

			core::IteratorPair<ElementsList>p(m_elements);
			float mid=m_maxY*0.5;
			float grad=mid*0.7;
			for(;!p.done();p++){
				GCPtr<IAboutItem> item=*p;
				float y=item->getY();
				if(y>mid){
					y=2*mid-y;
				}
				float alpha;
				if(y>grad)
					alpha=255;
				else{
					alpha=(y/grad)*255.0f;
				}
				if(y<0)
					alpha=0;
				item->setAlpha(alpha);
			}

		}else{
			m_fadeOutImg->Visible=true;
			m_currentState=1;
		}break;
	case 1:
		if(m_bgAlpha<255){
			m_bgAlpha+=100*dt;
			if(m_bgAlpha>255)
				m_bgAlpha=255;
			m_fadeOutImg->Color().A=m_bgAlpha;
		}else
			m_currentState=2;
		break;
	case 2:
		m_state=SenarioGeneric_Done;
	}
}
int AboutSenario::isDone(){
	return m_state;
}
void AboutSenario::addItem(GCPtr<IAboutItem>item){
	if(!item->getElem())return;
	item->getElem()->SetManager(m_guiMngr);
	item->getElem()->SetDevice(m_guiMngr->getDevice());
	m_backPanel->addChild(item->getElem());
	m_elements.push_back(item);
	m_sorted=0;
}

void AboutSenario::sort(){
	if(!m_sorted){
		std::list<GCPtr<IAboutItem>>::iterator it=m_elements.begin();
		std::list<GCPtr<IAboutItem>>::iterator it2=m_elements.end();
		float y=0;
		if(it!=m_elements.end()){
			(*it)->getElem()->MainPos=(math::vector2d(0,y));
			it2=it+1;
		}
		for(;it2!=m_elements.end();it2++){
			y+=(*it)->getElem()->getSize().y + 2+(*it2)->getSpace();
			if((*it2)->isTitle())
				y+=30;
			(*it2)->getElem()->MainPos=(math::vector2d(0,y));
			it=it2;
		}

		m_maxY=y;
		if(m_elements.last()!=m_elements.end()){
			m_maxY+=(*m_elements.last())->getElem()->getSize().y;
		}
		m_sorted=1;
	}
}

}
}
