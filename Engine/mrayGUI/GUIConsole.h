
/********************************************************************
	created:	2009/04/14
	created:	14:4:2009   23:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIConsole.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIConsole
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIConsole___
#define ___GUIConsole___

#include "GUIElement.h"
#include "GUIEditBox.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIConsole:public GUIElement,public IEventReciver
{
private:
	DECLARE_RTTI;
protected:
	GCPtr<GUIEditBox> m_cmdLine;

	enum ELineType{
		ELT_CMD,
		ELT_Normal,
		ELT_Error
	};
	struct SHistoryLine
	{
		SHistoryLine(){
			type=ELT_Normal;
		}
		SHistoryLine(const core::UTFString &l,ELineType t){
			line=l;
			type=t;
		}
		core::UTFString line;
		ELineType type;
	};
	std::vector<core::UTFString> m_cmdHistory;
	std::vector<SHistoryLine> m_lines;
	int m_lastSelected;

	FontAttributes m_font;

public:
	GUIConsole(GUIManager*mngr,float height);
	virtual~GUIConsole();

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr& event);

	virtual FontAttributes* getFontAttributes();

	void setTextColor(const video::SColor&c);
};

}
}


#endif //___GUIConsole___
