

/********************************************************************
	created:	2010/06/11
	created:	11:6:2010   16:53
	filename: 	i:\Programing\GameEngine\mrayEngine\Tools\Editor\EditorGlobals.h
	file path:	i:\Programing\GameEngine\mrayEngine\Tools\Editor
	file base:	EditorGlobals
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef EditorGlobals_h__
#define EditorGlobals_h__



#include "IToolBox.h"

namespace mray
{

class EditorGlobals
{
private:
protected:
public:

	static bool IsEngineInitiated;

	static std::list<IToolBox*> Toolboxes;

	static void RegisterToolBox(IToolBox*toolBox);
	static void UnregisterToolBox(IToolBox*toolBox);

};

}
#endif // EditorGlobals_h__
