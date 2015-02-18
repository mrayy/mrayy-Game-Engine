/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   0:37
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\VideoIntroItem.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	VideoIntroItem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___VideoIntroItem___
#define ___VideoIntroItem___

#include "IntroItem.h"
#include <IVideoTexture.h>

namespace mray{
namespace gameMod{

class VideoIntroItem:public IntroItem
{
private:
protected:
	GCPtr<video::IVideoTexture> m_video;
	bool m_toExit;
	float m_fadeOut;
	math::rectf m_viewArea;
public:
	VideoIntroItem(GCPtr<video::IVideoTexture> v);
	virtual~VideoIntroItem();

	virtual void setRect(const math::rectf&rc);

	void setVideo(GCPtr<video::IVideoTexture> v);
	GCPtr<video::IVideoTexture>  getVideo();

	void reset();

	virtual void draw();
	virtual void update(float dt);

	virtual bool isDone();

	virtual void sendSkip();
};

}
}


#endif //___VideoIntroItem___