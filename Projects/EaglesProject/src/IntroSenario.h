
/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   0:33
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\IntroSenario.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	IntroSenario
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IntroSenario___
#define ___IntroSenario___

#include "IScenarioNode.h"
#include "IntroItem.h"
#include <C_List.h>
#include <ITexture.h>
#include <IVideoTexture.h>

namespace mray{
namespace gameMod{

class IntroSenario:public IScenarioNode
{
private:
protected:
	typedef std::list<GCPtr<IntroItem>> IntroItemsList;
	IntroItemsList m_introItems;
	IntroItemsList::iterator m_currItem;
public:
	IntroSenario();
	virtual~IntroSenario();

	void createVideoItem(GCPtr<video::IVideoTexture> v);
	void createImageItem(GCPtr<video::ITexture> t,float fadeTime,float freezeTime);

	void addIntroItem(GCPtr<IntroItem> item);

	virtual void update(float dt);

	virtual void onEvent(GCPtr<Event> event);

	virtual void onEnter();
	virtual void onExit();

	virtual int isDone();
};

}
}


#endif //___IntroSenario___
