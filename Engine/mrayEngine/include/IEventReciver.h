

#ifndef ___IEVENTRECIVER___
#define ___IEVENTRECIVER___

namespace mray{
	class Event;

class IEventReciver
{
public:
	IEventReciver(){};
	virtual ~IEventReciver(){};

	virtual void onEvent(Event* e)=0;
};

};

#endif