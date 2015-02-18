// NavMeshPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "NavMeshPlugin.h"


#ifdef _MANAGED
#pragma managed(push, off)
#endif


namespace mray{
namespace plugins{

NavMeshPlugin::NavMeshPlugin()
{
}
NavMeshPlugin::~NavMeshPlugin()
{
}

const mchar*NavMeshPlugin::getName()
{
	return mT("Navigation Mesh Generator");
}

void NavMeshPlugin::onEvent(GCPtr<Event> event)
{
	if(event->getType()==ET_GuiEvent){
		GCPtr<GUIEvent> e=event;
		if(e->Event==GET_DIALOG_CLOSE && e->Caller==m_fileBox)
		{
			if(m_fileBox->getCloseStatus()==GUI::EBC_OK)
			{
			}
		}
	}
}

void NavMeshPlugin::install()
{
	GCPtr<EditorApplication> app;
	app=EditorGlobals::getInstance().GetApplication();
	app->addPlugin(this);

	m_fileBox=app->getGUIManager()->addFileBox(math::vector2d(100,100));
	m_fileBox->getFontAttributes()->fontSize=12;
	m_fileBox->addFilter(mT(".mdl"),mT("Model files"));
	m_fileBox->setReciver(this);
}
void NavMeshPlugin::uninstall()
{
}


void NavMeshPlugin::onActivate()
{
	m_fileBox->openDialog(GUI::EB_OPEN,&m_resultPath);
}
const mchar* NavMeshPlugin::getPluginBtnName()
{
	return mT("NavMesh");
}

extern "C" __declspec(dllexport) IPlugin* dllCreatePlugin(){
	return new NavMeshPlugin();

} 

}
}
/*
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

*/
