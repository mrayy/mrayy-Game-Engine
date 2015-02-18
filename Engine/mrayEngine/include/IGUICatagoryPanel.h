

/********************************************************************
	created:	2011/12/14
	created:	14:12:2011   14:45
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUICatagoryPanel.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUICatagoryPanel
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUICatagoryPanel__
#define __IGUICatagoryPanel__


#include "IGUIPanelElement.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUICatagoryPanel:public IGUIPanelElement
{
private:
protected:
public:
	static const GUID ElementType;

	IGUICatagoryPanel(IGUIManager* creator);
	virtual~IGUICatagoryPanel();

};


}
}

#endif


