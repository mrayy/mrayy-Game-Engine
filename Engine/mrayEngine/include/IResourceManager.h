
#ifndef ___IResourceManager___
#define ___IResourceManager___


#include <hash_map>
#include "GCPtr.h"
#include "IResource.h"
#include "mString.h"
#include "compileConfig.h"
#include "IStream.h"


namespace mray{
	namespace OS{

		class IMutex;
	}

class MRAY_DLL IResourceManager
{

public:
	typedef std::map<ResourceHandle,IResourcePtr> ResourceHandleMap;
	typedef std::map<core::string,IResourcePtr> ResourceMap;

protected:
	ResourceHandle m_lastHandle;

	ResourceHandleMap m_resourceHandleMap;
	ResourceMap m_resourceMap;

	ulong m_allowedMemory;
	ulong m_usedMemory;

	OS::IMutex* m_resMutex;

	core::string m_resourceType;

	ResourceHandle getNextHandle();

protected:

	virtual IResourcePtr createInternal(const core::string& name)=0;
	virtual void removeInternal(const IResourcePtr& resource);
	virtual bool addInternal(const IResourcePtr& resource,const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file)=0;

	virtual IResourcePtr create(const core::string& name);

	
	virtual core::string getDefaultGroup()=0;

	void _removeResource(const IResourcePtr& resource);
public:

	IResourceManager(const mchar*resourceType);
	virtual~IResourceManager();

	virtual IResourcePtr getOrCreate(const core::string& name);
	virtual IResourcePtr createFromFile(OS::IStreamPtr file);

	virtual void setAllowedMemory(ulong size);
	virtual ulong getAllowedMemory();
	virtual void checkMemory();
	virtual ulong getUsedMemory(bool recalc=false);
/**/
	virtual void unload(const core::string&name);
	virtual void unload(IResourcePtr resource);
	virtual void unload(ResourceHandle resource);

	virtual void unloadAll(bool unusedOnly=true);

	virtual void reload(const core::string&name);
	virtual void reload(IResourcePtr resource);
	virtual void reload(ResourceHandle resource);
	virtual void reloadAll();

	virtual void addResource(const IResourcePtr& resource,const core::string&name);
	
	virtual void remove(const core::string&name);
	virtual void remove(const IResourcePtr& resource);
	virtual void remove(ResourceHandle resource);
	virtual void removeAll();

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName){};
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName){};
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName){};

	virtual const IResourcePtr& getResource(const core::string&name);
	virtual const IResourcePtr& getResourceByHandle(ResourceHandle resource);

	virtual bool isResourceExists(const core::string&name);
	virtual bool isResourceExists(ResourceHandle resource);

	virtual void resourceLoaded(IResource*res);
	virtual void resourceUnloaded(IResource*res);

	virtual void _requestLoad(IResource*r);
	virtual void _requestUnload(IResource*r);

	virtual void listResources(std::vector<core::string>&outList);
	virtual void listResources(std::vector<IResourcePtr>&outList);

	const mchar*getResourceType();
};

}



#endif





