


#include "GUIMessage.h"
#include "GUIManager.h"
#include "FontResourceManager.h"

#include "MouseEvent.h"
#include "GUIEvent.h"
#include "KeyboardEvent.h"
#include "EventMemoryManager.h"



namespace mray{
namespace GUI{


class GUIMessage::COkBtnEvent:public IEventReciver{
	GCPtr<GUIMessage>m_msg;
public:
	COkBtnEvent(GCPtr<GUIMessage>msg){
		m_msg=msg;
	}
	virtual void onEvent(const EventPtr& e){
		if(e->getType()==GET_Mouse_UP){
			GCPtr<GUIEvent>event=e;
			GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
			event1->Caller=m_msg;
			event1->Event=GET_DIALOG_CLOSE;
			m_msg->getManager()->onEvent(event1);
			if(m_msg->getParent())
				m_msg->getParent()->setActiveElem(0);

			m_msg->setVisible(0);
		}
	}
};

GUIMessage::GUIMessage(GUIManager* mngr,math::vector2d pos):
	GUIWindow(mngr,mT(""),pos,math::vector2d(200,100)){

	m_message = mngr->addStaticText(mT(""),math::vector2d(20,20),math::vector2d(100,20),NULL,this,240,0);
	m_message->Background=0;
	m_message->addEffector(guiMngr->addAnchorEffector(GUI::EPAnchor_CENTER,math::vector2d(0,-20)));

	okBtnEvent=new COkBtnEvent(this);
		
	m_ok	=guiMngr->addButton(mT("OK"),math::vector2d(280,330),math::vector2d(80,40),okBtnEvent,this,240,0);
	m_ok->addEffector(guiMngr->addAnchorEffector(GUI::EPAnchor_B,math::vector2d(0,-10)));

	setVisible(0);
}
GUIMessage::~GUIMessage()
{
}

void GUIMessage::showMessage(const  mchar*message,const  mchar*Caption,video::SColor msgColor){
	m_message->Caption=(message);
	m_message->getFontAttributes()->fontColor=msgColor;
	setCaption(Caption);

	IFont*font=gFontResourceManager.getFontByName(getFontAttributes()->fontName);
	if(font){
		math::vector2d dim;//=font->getTextDimension(message,getFontAttributes(),0);
		dim.x=math::Max(m_ok->getSize().x+50,dim.x);
		m_message->MainSize=(dim);
		this->MainSize=(dim+math::vector2d(50,80));

	}

	setVisible(1);
	if(getParent()){
		getParent()->setActiveElem(this);
		getParent()->bringToFront(this);
	}
}
GCPtr<GUIStaticText>GUIMessage::getMessageText(){
	return m_message;
}

void GUIMessage::draw(float dt){
	GUIWindow::draw(dt);
}

void GUIMessage::onEvent(const EventPtr& e)
{
	if(!Visible)return;
	GUIWindow::onEvent(e);
	switch(e->getType())
	{
	case ET_Keyboard:
	{
		GCPtr<KeyboardEvent>event=e;
		if(event->key==KEY_ESCAPE &&
			event->press==1){
			GCPtr<GUIEvent> event1=EventMemoryManager::getInstance().createEvent(ET_GuiEvent);
			event1->Caller=m_ok;
			event1->Event=GET_DIALOG_CLOSE;
			if(getParent())
				getParent()->setActiveElem(0);
		
			getManager()->onEvent(event1);
			setVisible(0);
		}
		break;
	}
	}
}
FontAttributes* GUIMessage::getFontAttributes(){
	return m_message->getFontAttributes();
}

}
}



