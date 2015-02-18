
#ifndef ___EVENT___
#define ___EVENT___

#include "mstring.h"
#include "GCPtr.h"
#include "GUID.h"

namespace mray{

namespace video
{
	class RenderWindow;
}

extern MRAY_DLL const GUID ET_Unkown;
extern MRAY_DLL const GUID ET_Mouse;
extern MRAY_DLL const GUID ET_Keyboard;
extern MRAY_DLL const GUID ET_Joystick;
extern MRAY_DLL const GUID ET_GUIEvent;
extern MRAY_DLL const GUID ET_NetworkEvent;
extern MRAY_DLL const GUID ET_ResizeEvent;
extern MRAY_DLL const GUID ET_SystemEvent;
extern MRAY_DLL const GUID ET_ChangeModeEvent;


class MRAY_DLL Event
{
protected:
	bool recived;
	GUID m_type;
	video::RenderWindow* m_ownerWindow;
public:
	Event(const GUID& t);
	virtual~Event();

	void SetOwnerRenderWindow(video::RenderWindow*rw){m_ownerWindow=rw;}
	video::RenderWindow* GetOwnerRenderWindow(){return m_ownerWindow;}

	bool isRecived();
	void setRecived();
	
	virtual void reset();
	
	const GUID& getType();
	const core::stringc& getTypeStr();
	//ulong getMaskingValue();

	//static ulong getEventMaskingValue(EVENT_TYPE t);
};

MakeSharedPtrType(Event)

};//mray
#endif