
/********************************************************************
	created:	2012/11/28
	created:	28:11:2012   14:14
	filename: 	D:\Development\mrayEngine\Engine\mrayGUI\GUIPaneElement.h
	file path:	D:\Development\mrayEngine\Engine\mrayGUI
	file base:	GUIPaneElement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _GUIPaneElement_
#define _GUIPaneElement_

#include <IGUIPanelElement.h>


namespace mray
{
namespace GUI
{

	enum EPanePosition
	{

	};

class GUIPaneElement:public IGUIPanelElement
{
	DECLARE_RTTI;
protected:
public:
	GUIPaneElement();
	virtual~GUIPaneElement();
};


}
}

#endif