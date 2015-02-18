

#include "GUIConsole.h"
#include "GUIEvent.h"
#include "FontResourceManager.h"
#include "CommandManager.h"
#include "GUIManager.h"


namespace mray{
namespace GUI{

GUIConsole::GUIConsole(GUIManager*mngr,float height)
:GUIElement(mngr,0,0,math::vector2d(0,height),0,0)
{
	m_cmdLine=new GUIEditBox(mngr,mT(""),0,math::vector2d(1,height/6),0,0);
	addChild(m_cmdLine);
	m_cmdLine->setDock(ED_BOTTOM);
	m_cmdLine->getFontAttributes()->fontSize=15;
	m_cmdLine->getFontAttributes()->fontColor=255;//.Set(0,0,0,255);
	m_cmdLine->getFontAttributes()->hasShadow=0;
	this->setDock(ED_TOP);

	m_font.fontSize=15;
	m_font.fontColor=255;
	m_font.hasShadow=0;
	m_font.fontAligment=EFA_TopLeft;

	Color=video::SColor(255,255,255,128);

	m_lastSelected=-1;


}
GUIConsole::~GUIConsole(){
}

void GUIConsole::draw(float dt){
	if(!Visible)return;
	math::rectf rc=getRect();

	if(getManager()->getFocus().pointer()==this){
		getManager()->setFocus(m_cmdLine);
	}

	getDevice()->draw2DRectangle(rc,this->Color);
	IFont*font=gFontResourceManager.getFontByName(m_font.fontName);

	if(font){
		math::rectf trc=rc;
		trc.ULPoint+=10;
		int cnt=(rc.getHeight()-m_cmdLine->getSize().y)/m_font.fontSize;
		cnt-=1;
		for (int i=math::Max<int>(0,m_lines.size()-cnt);i<m_lines.size();++i)
		{
			switch(m_lines[i].type){
			case ELT_CMD:
				m_font.fontColor.Set(0,0,0,255);
				break;
			case ELT_Normal:
				m_font.fontColor.Set(0,255,0,255);
				break;
			case ELT_Error:
				m_font.fontColor.Set(255,0,0,255);
				break;
			}
			//font->print(trc,&m_font,&rc,m_lines[i].line,guiMngr->GetRenderQueue());
			trc.ULPoint.y+=m_font.fontSize;
			trc.BRPoint.y+=m_font.fontSize;
		}
	}

	GUIElement::draw(dt);
}
void GUIConsole::onEvent(const EventPtr& event){
	GUIElement::onEvent(event);
	switch (event->getType())
	{
	case ET_GuiEvent:
		{
			GCPtr<GUIEvent> e=event;
			if(e->Caller==m_cmdLine && e->event==GET_KeyDown){
				if(e->key==KEY_RETURN){
					m_cmdHistory.push_back(m_cmdLine->Caption());
					m_lines.push_back(SHistoryLine(m_cmdLine->Caption(),ELT_CMD));

					core::UTFString res;
					bool ok=CommandManager::getInstance().execCommand(m_cmdLine->Caption());
					std::vector<core::UTFString> arr;
					res=CommandManager::getInstance().getLastMessage();
					res.split(res.c_str(),arr,mT("\n"));
					for(int i=0;i<arr.size();++i)
						m_lines.push_back(SHistoryLine(arr[i],ok ? ELT_Normal : ELT_Error));

					m_cmdLine->Caption=mT("");
					m_lastSelected=m_cmdHistory.size();
				}else if(e->key==KEY_UP){
					if(m_lastSelected>0){
						m_lastSelected--;
					}else{
						m_lastSelected=-1;
					}
					if(m_lastSelected>=0)
						m_cmdLine->Caption=m_cmdHistory[m_lastSelected];
				}else if(e->key==KEY_DOWN){
					if(m_lastSelected<(int)m_cmdHistory.size()-1){
						m_lastSelected++;
					}else{
						m_lastSelected=(int)m_cmdHistory.size();
					}
					if(m_lastSelected<(int)m_cmdHistory.size())
						m_cmdLine->Caption=m_cmdHistory[m_lastSelected];
					else
						m_cmdLine->Caption=mT("");
				}
			}
		}break;
	}
}

FontAttributes* GUIConsole::getFontAttributes(){
	return &m_font;
}

void GUIConsole::setTextColor(const video::SColor&c){
	m_font.fontColor=c;
	m_cmdLine->getFontAttributes()->fontColor=c;
}


}
}