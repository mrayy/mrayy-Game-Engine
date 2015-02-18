

/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   13:07
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUIEditBox.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUIEditBox
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIEditBox__
#define __IGUIEditBox__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIEditBox:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	ObjectEvent OnKeyDown;//args=KeyboardEvent
	ObjectEvent OnTextChange;

	DECLARE_PROPERTY_TYPE(RegularExpression,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Background,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Border,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(HideText,bool,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(HideChar,core::string,MRAY_DLL);

public:
	IGUIEditBox(IGUIManager*mngr);
	virtual~IGUIEditBox();

	virtual bool CheckIsValideText(const core::UTFString&t) =0;

	virtual core::string getSelectedText() =0;

	virtual const bool& getBackground() =0;
	virtual bool setBackground(const bool &val)  =0;

	virtual const bool& getBorder()  =0;
	virtual bool setBorder(const bool &val) =0;

	virtual const core::string& getHideChar() =0;
	virtual bool setHideChar(const core::string &val) =0;

	virtual const bool& getHideText()  =0;
	virtual bool setHideText(const bool &val)  =0;

	virtual const core::string& getRegex() =0;
	virtual bool setRegex(const core::string&val) =0;
};

}
}

#endif

