
#ifndef ___MeshResourceManager___
#define ___MeshResourceManager___



#include "compileconfig.h"
#include "IResourceManager.h"
#include "ISingleton.h"
#include "MeshLoader.h"
#include "SMesh.h"

namespace mray{


class MRAY_DLL MeshResourceManager:public IResourceManager,public ISingleton<MeshResourceManager>
{

	GCPtr<loaders::MeshLoader> m_meshLoader;

protected:
	virtual IResourcePtr createInternal(const core::string& name);
	
	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);


	virtual core::string getDefaultGroup();
public:
	MeshResourceManager();
	virtual~MeshResourceManager();

	void addMeshLoader(loaders::IMeshLoader*loader);

	GCPtr<scene::SMesh> loadMesh(const core::string&name,bool instance);

	
	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gMeshResourceManager mray::MeshResourceManager::getInstance()

}




#endif


