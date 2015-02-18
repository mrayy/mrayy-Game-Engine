


/********************************************************************
	created:	2011/11/20
	created:	20:11:2011   13:11
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUICheckBox.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUICheckBox
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUICheckBox__
#define __IGUICheckBox__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUICheckBox:public IGUIElement
{
private:
protected:
public:
	static const GUID ElementType;

	ObjectEvent OnChangeCheck;
public:
	IGUICheckBox(IGUIManager* creator);
	virtual~IGUICheckBox();

	virtual void ChangeState()=0;

	virtual const bool& IsChecked()=0;
	virtual bool SetChecked(const bool& val)=0;

	virtual const bool& IsBackgroundOpaque() =0;
	virtual bool SetBGOpaque(const bool& val)=0;

	virtual const bool& IsPushButton()=0;
	virtual bool SetPushButton(const bool& val)=0;


};

}
}

#endif
