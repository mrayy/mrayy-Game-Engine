


/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   12:52
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\IGUIButton.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	IGUIButton
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIButton__
#define __IGUIButton__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIButton:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	ObjectEvent OnKeyDown;//args=KeyCode
	ObjectEvent OnClick;//args=MouseButton

public:
	IGUIButton(IGUIManager* creator);
	virtual~IGUIButton();

};


}
}

#endif
