
#include "GUIDummy.h"
#include "GUIManager.h"


namespace mray{
namespace GUI{

GUIDummy::GUIDummy():GUIElement(0)
{
	EnableClip=false;
	Selectable=false;
	setTypeStr(mT("Dummy"));
}
GUIDummy::GUIDummy(GUIManager* manager,math::vector2d pos,GCPtr<GUIElement>  parent)
:GUIElement(manager,parent,pos,math::vector2d(0,0),0,0)
{
	EnableClip=false;
	Selectable=false;
	setTypeStr(mT("Dummy"));
}
GUIDummy::~GUIDummy()
{
}




GCPtr<GUIElement> GUIDummy::getDuplicate()
{
	GUIDummy*dup=guiMngr->addDummy(getPos(),getParent());

	return dup;
}


}
}