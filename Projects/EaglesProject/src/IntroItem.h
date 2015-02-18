
/********************************************************************
	created:	2009/04/01
	created:	1:4:2009   0:35
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\IntroItem.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	IntroItem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IntroItem___
#define ___IntroItem___

#include <rect.h>

namespace mray{
namespace gameMod{

class IntroItem
{
private:
protected:
public:
	IntroItem(){}
	virtual~IntroItem(){}

	virtual void setRect(const math::rectf&rc)=0;

	virtual void draw()=0;
	virtual void update(float dt)=0;

	virtual bool isDone()=0;

	virtual void sendSkip()=0;
};

}
}

#endif //___IntroItem___
