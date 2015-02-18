
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   18:55
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\IEnvironemtEvent.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	IEnvironemtEvent
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this interface is used to define environment events like rain,snow,etc..
*********************************************************************/

#ifndef ___IEnvironemtEvent___
#define ___IEnvironemtEvent___

#include <mstring.h>

namespace mray{
namespace gameMod{

class IEnvironemtEvent
{
private:
protected:
public:
	IEnvironemtEvent(){}
	virtual~IEnvironemtEvent(){}

	virtual float getEventStart()=0;
	virtual float getEventEnd()=0;

	virtual void enableEvent()=0;
	virtual void update(float dt)=0;
	virtual void disableEvent()=0;

	virtual const core::string& getEventName()=0;
};

}
}


#endif //___IEnvironemtEvent___
