#include "stdafx.h"

#include "GUIOverlayManager.h"
#include "XMLOverlay.h"
#include "IFileSystem.h"
#include "GUIOverlay.h"



namespace mray{
namespace GUI{

GUIOverlayManager::GUIOverlayManager(){
}
GUIOverlayManager::~GUIOverlayManager(){
	Clear();
}

GUIOverlay* GUIOverlayManager::LoadOverlay(const core::string&filename)
{
	XMLOverlay o;
	return o.loadXML(gFileSystem.openFile(filename));
}
GUIOverlay* GUIOverlayManager::GetOverlay(const core::string&overlay)
{
	OverlayMap::iterator it= m_overlays.find(overlay);
	if(it==m_overlays.end())
		return 0;
	return it->second;
}

void GUIOverlayManager::AddOverlay(GUIOverlay*o){
	m_overlays[o->GetName()]=o;
}
void GUIOverlayManager::Clear(){
	OverlayMap::iterator it= m_overlays.begin();
	for(;it!=m_overlays.end();++it)
	{
		delete it->second;
	}
	m_overlays.clear();
}

}
}