
/********************************************************************
	created:	2009/05/01
	created:	1:5:2009   22:44
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\AboutSenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	AboutSenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AboutSenario___
#define ___AboutSenario___

#include "IScenarioNode.h"
#include "ImageController.h"
#include <GUIManager.h>
#include <ISoundManager.h>

namespace mray{
namespace gameMod{

class IAboutItem{
protected:
	bool m_isTitle;
	GCPtr<GUI::GUIElement> m_elem;
	float m_space;
public:
	IAboutItem(bool isTitle)
	{
		m_space=0;
		m_elem=0;
		m_isTitle=isTitle;
	}

	GCPtr<GUI::GUIElement>getElem(){
		return m_elem;
	}

	void setSpace(float space){
		m_space=space;
	}
	float getSpace(){
		return m_space;
	}

	virtual void setAlpha(float a){
		m_elem->Alpha=a;
		m_elem->Color=video::SColor(m_elem->Color().R,m_elem->Color().G,m_elem->Color().B,a);
	}

	float getY(){
		return m_elem->getAbsPos().y;
	}

	bool isTitle(){
		return m_isTitle;
	}

	virtual void update(float dt)=0;
};

class AboutSenario:public IScenarioNode
{
private:
protected:
	typedef std::list<GCPtr<IAboutItem>> ElementsList;
	ElementsList m_elements;

	GCPtr<GUI::GUIManager> m_oldGuiMngr;
	GCPtr<GUI::GUIManager> m_guiMngr;

	GCPtr<sound::ISoundManager> m_sndMngr;
	GCPtr<sound::ISoundManager> m_oldSndMngr;

	GCPtr<sound::ISound> m_bgSound;

	int m_state;

	GCPtr<GUI::GUIElement> m_backPanel;

	GCPtr<ImageController> m_imageController;

	GCPtr<GUI::GUIImage> m_fadeOutImg;
	int m_currentState;
	float m_bgAlpha;

	bool m_sorted;
	int m_maxY;

	bool m_fastDisplay;

	void sort();

public:
	AboutSenario();
	virtual~AboutSenario();

	GCPtr<GUI::GUIManager> getGUIManager();

	virtual void onEnter();
	virtual void onExit();

	virtual void onEvent(GCPtr<Event> event);

	virtual void update(float dt);

	// return the exit code !=0
	virtual int isDone();

	void AboutSenario::addItem(GCPtr<IAboutItem>item);
};

}
}


#endif //___AboutSenario___
