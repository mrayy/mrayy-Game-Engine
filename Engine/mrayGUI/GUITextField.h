

#ifndef ___GUITextField___
#define ___GUITextField___


#include "IGUIField.h"

namespace mray{
namespace GUI{


class GUITextField:public IGUIField
{
	int CursorPos;
	int MarkBegin;
	int MarkEnd;
	bool MouseMark;
	int startPos;
public:

	GUITextField();
	virtual ~GUITextField();
	virtual void Event(SEvent&event);
};

}
}



#endif


