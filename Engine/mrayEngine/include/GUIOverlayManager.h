
/********************************************************************
created:	2009/04/10
created:	10:4:2009   18:05
filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIOverlayManager.h
file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
file base:	GUIOverlayManager
file ext:	h
author:		Mohamad Yamen Saraiji

purpose:	
*********************************************************************/

#ifndef ___GUIOverlayManager___
#define ___GUIOverlayManager___

#include "ISingleton.h"



namespace mray{
namespace GUI{

	class GUIOverlay;

class MRAY_DLL GUIOverlayManager:public ISingleton<GUIOverlayManager>
{
private:
protected:
	typedef std::map<core::string,GUIOverlay*> OverlayMap;
	OverlayMap m_overlays;
public:
	GUIOverlayManager();
	virtual~GUIOverlayManager();

	GUIOverlay* LoadOverlay(const core::string&filename);
	GUIOverlay* GetOverlay(const core::string&overlay);

	void AddOverlay(GUIOverlay* o);

	void Clear();
};

}
}

#endif //___GUIOverlayManager___
