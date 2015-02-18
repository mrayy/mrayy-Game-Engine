

#include "Stdafx.h"
#include "EngineScriptExporter.h"

namespace mray{
namespace script{

	void OpenMath(lua_State*L);
	void OpenVideo(lua_State*L);
	void OpenScene(lua_State*L);
	void OpenCore(lua_State*L);


	void EngineScriptExporter::openLibs(lua_State*L)
	{
		OpenCore(L);
		OpenMath(L);
		OpenVideo(L);
		OpenScene(L);
	}




}
}