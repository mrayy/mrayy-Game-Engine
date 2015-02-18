
#ifndef ___GUIDUMMY___
#define ___GUIDUMMY___

//It's just a dummy
//used for parent things
#include "GUIElement.h"
#include "SColor.h"
#include "IFont.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIDummy:public GUIElement
{
public:
	DECLARE_RTTI;

	GUIDummy();
	GUIDummy(GUIManager*mngr,math::vector2d pos,GCPtr<GUIElement>  parent=0);
	virtual~GUIDummy();

	virtual GCPtr<GUIElement> getDuplicate();
};

}
}

#endif