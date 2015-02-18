

#ifndef ___GUILogger___
#define ___GUILogger___

#include "GUIElement.h"
#include "SColor.h"


namespace mray{
namespace GUI{

class MRAY_DLL GUILogger:public GUIElement
{
	struct SItem{
		SItem(const  mchar *msg,video::SColor clr){
			this->msg=msg;
			this->clr=clr;
			msgTime=0;
			Alpha=clr.A;
		};
		float Alpha;
		core::string msg;
		video::SColor clr;
		float msgTime;
	};
	std::list<SItem*> items;
	float messageTime;
	FontAttributes m_FontAttributs;
public:
	DECLARE_RTTI;

	GUILogger(GUIManager* manager,float messageTime,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent);
	virtual ~GUILogger();

	void addMessage(const  mchar *msg,video::SColor clr);
	void clear();

	virtual void draw(float dt);
	virtual FontAttributes* getFontAttributes();

};

}
}


#endif




