

/********************************************************************
	created:	2013/01/28
	created:	28:1:2013   20:25
	filename: 	C:\Development\mrayEngine\Plugins\mrayBerkelium\GUIPageManager.h
	file path:	C:\Development\mrayEngine\Plugins\mrayBerkelium
	file base:	GUIPageManager
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIPageManager__
#define __GUIPageManager__


#include "GUIWebPageControl.h"

namespace mray
{
namespace GUI
{
	class GUIPageManagerImpl;
class GUIPageManager
{
protected:
	GUIPageManagerImpl* m_impl;
public:
	GUIPageManager();
	virtual~GUIPageManager();

	GCPtr<GUIWebPageControl> CreateWebPage(IGUIManager* creator);

	void RemovePage(GUIWebPageControl*p);
	void ClearPages();

	void Update();
};

}
}


#endif
