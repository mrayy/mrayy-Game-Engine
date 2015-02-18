
/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   0:37
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\ImageIntroItem.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	ImageIntroItem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ImageIntroItem___
#define ___ImageIntroItem___

#include "IntroItem.h"
#include <ITexture.h>

namespace mray{
namespace gameMod{

class ImageIntroItem:public IntroItem
{
private:
protected:
	GCPtr<video::ITexture> m_image;
	int m_mode;
	float m_time;
	float m_fadeTime;
	float m_appearTime;
	math::rectf m_viewArea;
public:
	ImageIntroItem(GCPtr<video::ITexture> t,float fadeTime,float freezeTime);
	virtual~ImageIntroItem();

	virtual void setRect(const math::rectf&rc);

	void setImage(GCPtr<video::ITexture>v);
	GCPtr<video::ITexture> getImage();

	virtual void draw();
	virtual void update(float dt);

	virtual bool isDone();

	virtual void sendSkip();
};

}
}


#endif //___ImageIntroItem___
