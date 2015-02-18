

/********************************************************************
	created:	2011/12/04
	created:	4:12:2011   10:06
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\IGUIPropertyGrid.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	IGUIPropertyGrid
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUIPropertyGrid__
#define __IGUIPropertyGrid__

#include "IGUIElement.h"
#include "CPropertieSet.h"

namespace mray
{
namespace GUI
{

class MRAY_DLL IGUIPropertyGrid:public IGUIElement
{
private:
protected:
	CPropertieSet* m_object;
public:
	static const GUID ElementType;

	IGUIPropertyGrid(IGUIManager*mngr);
	virtual~IGUIPropertyGrid();

	virtual void SetPropertyObject(CPropertieSet* obj){m_object=obj;}
	virtual CPropertieSet* GetPropertyObject(){return m_object;}
};


}
}

#endif

