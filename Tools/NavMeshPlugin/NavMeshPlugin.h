
/********************************************************************
	created:	2010/03/04
	created:	4:3:2010   12:50
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\NavMeshPlugin\NavMeshPlugin.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\NavMeshPlugin
	file base:	NavMeshPlugin
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___NavMeshPlugin___
#define ___NavMeshPlugin___

#include <IEditorPlugin.h>
#include <IPlugin.h>

namespace mray{
namespace plugins{

class __declspec(dllexport) NavMeshPlugin:public IPlugin,public IEditorPlugin,public IEventReciver
{
protected:
	GCPtr<GUI::GUIFileBox> m_fileBox;
	core::string m_resultPath;
public:
	NavMeshPlugin();
	virtual~NavMeshPlugin();

	virtual const mchar*getName();

	virtual void onEvent(GCPtr<Event> e);

	virtual void install();
	virtual void uninstall();


	virtual void onActivate();
	virtual const mchar* getPluginBtnName();
};

}
}


#endif //___NavMeshPlugin___
