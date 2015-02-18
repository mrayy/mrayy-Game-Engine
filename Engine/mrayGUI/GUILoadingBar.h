


#ifndef ___GUILoadingBar___
#define ___GUILoadingBar___


#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUILoadingBar:public GUIElement
{
	GCPtr<video::TextureUnit>  bkTex;
	GCPtr<video::TextureUnit>  crTex;
	GCPtr<video::TextureUnit>  layerTex;

	float stepSpeed;
	float stepsCount;
	bool repeat;

	float current;

	video::SColor crColor;

	bool isAuto;
	bool useLayer;
	bool drawOutline;
	bool drawPercent;

	FontAttributes m_FontAttributs;
public:
	DECLARE_RTTI;

	GUILoadingBar(GUIManager* manager,float speed,int stepsCnt,math::vector2d pos,math::vector2d size,GCPtr<GUIElement>  parent=0,video::SColor color=240,int id=0);
	~GUILoadingBar();
	virtual void draw(float dt);
	void setBackTexture(const  mchar*texName);
	void setLoadingTexture(const  mchar*texName);
	void setLayerTexture(const  mchar*texName);
	virtual void loadTextures();

	void drawOutlineEnable(bool d){drawOutline=d;}
	void drawPercentEnable(bool d){drawPercent=d;}

	void enableLayer(bool e){useLayer=e;}
	void setAuto(bool a){isAuto=a;}
	
	void setLoadingColor(const video::SColor &clr){crColor=clr;};

	void setSpeed(float s){stepSpeed = s;}
	void setRepeat(bool r){repeat =r;}

	void setStepCount(float steps){stepsCount=steps;}
	void addSteps(float step=1);

	void reset(){current=0;}

	bool isDone(){return current==stepsCount;}
	
	virtual GCPtr<GUIElement> getDuplicate();
	virtual FontAttributes* getFontAttributes();
};

}
}

#endif




