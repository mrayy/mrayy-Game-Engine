
/********************************************************************
	created:	2009/04/10
	created:	10:4:2009   11:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIDefaultElements.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIDefaultElements
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GUIDefaultElements___
#define ___GUIDefaultElements___

#include "CompileConfig.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIDefaultElements
{
private:
protected:
public:
	GUIDefaultElements();
	virtual~GUIDefaultElements();

	static void installElements();
	static void removeElements();
};

}
}


#endif //___GUIDefaultElements___
