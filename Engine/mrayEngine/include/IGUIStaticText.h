

/********************************************************************
	created:	2011/11/28
	created:	28:11:2011   10:34
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIStaticText.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIStaticText
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIStaticText__
#define __IGUIStaticText__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIStaticText:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	IGUIStaticText(IGUIManager* creator);
	virtual~IGUIStaticText();

	virtual const bool& GetBackground() =0;
	virtual bool SetBackground(const bool& val) =0;

	virtual const bool& GetBorder() =0;
	virtual bool SetBorder(const bool& val) =0;
};

}
}

#endif
