
#ifndef ___MESH_LOADER___
#define ___MESH_LOADER___


#include "MString.h"

namespace mray
{
	namespace scene
	{
		class SMesh;
	}
	namespace OS
	{
		class IStream;
	}
namespace loaders
{
	class IMeshLoader;

class MRAY_DLL MeshLoader
{
	std::map<core::string,IMeshLoader*> m_Loaders;
public:
	MeshLoader(){}
	virtual ~MeshLoader();
	void addLoader(IMeshLoader* loader);
	scene::SMesh* Load(const  core::string&name);
	scene::SMesh* Load(OS::IStream* file);

	IMeshLoader* GetLoader(const core::string& ext);
};

};//loaders
};//mray

#endif




