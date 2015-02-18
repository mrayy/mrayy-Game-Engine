

#ifndef ___GUIAnimatedSprite___
#define ___GUIAnimatedSprite___

#include "GUICellSprite.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIAnimatedSprite:public GUICellSprite
{
public:
	struct SCycle{
		SCycle(const mchar*name,int start,int end,float speed,bool loop){
			this->name=name;
			this->start=start;
			this->end=end;
			this->speed=speed;
			this->loop=loop;
		}
		SCycle(){
			start=0;
			end=0;
		}
		core::string name;
		int start;
		int end;
		float speed;
		bool loop;
	};
protected:

	float m_frame;
	float speed;
	bool autoAnimate;

	std::vector<SCycle> cycles;
	int currCycle;
public:
	DECLARE_RTTI;

	GUIAnimatedSprite(GUIManager* manager,video::ITexturePtr  tex,int rowCnt,int colCnt,int framesCnt,float animSpeed,
		math::Point2df pos,math::Point2df size,IEventReciver* event=0,GCPtr<GUIElement> parent=0,int id=0);
	virtual ~GUIAnimatedSprite();

	virtual void draw(float dt);

	void setFrame(float f);
	float getFrame();

	int addCycle(SCycle&c);
	void setCycle(int c);
	void setCycle(const mchar*name);
	int getCurrentCycle();

	void setAutoAnimate(bool a);
	bool getAutoAnimate();

	void setSpeed(float s);
	float getSpeed();

};

}
}

#endif





