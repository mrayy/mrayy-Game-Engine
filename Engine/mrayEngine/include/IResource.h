
#ifndef ___IResource___
#define ___IResource___

#include <time.h>
#include "mTypes.h"
#include "mString.h"
#include "IObject.h"
#include "compileConfig.h"

#include "GUID.h"


namespace mray{

class IResourceManager;
class IResource;
class IAsyncLoadRequest;

typedef ulong ResourceHandle;
class ResourceListner
{
public:
	virtual void loadComplete(IResource*)=0;
	virtual void unloadComplete(IResource*)=0;
};

class MRAY_DLL IResource:public IObject
{
	DECLARE_RTTI
public:


	enum EPriorityLevel{
		EPL_LowLevel=0,
		EPL_MidLevel,
		EPL_HighLevel
	};
	enum ELoadingState{
		LoadState_Unloaded,
		LoadState_Loading,
		LoadState_Loaded,
		LoadState_Unloading
	};
protected:

	IResourceManager*m_manager;
	IAsyncLoadRequest*m_loadRequest;


	EPriorityLevel m_priorityLevel;
	time_t m_lastAccess;

	GUID m_handle;
	core::string m_name;
	core::string m_group;

	ELoadingState m_loadState;

	std::vector<ResourceListner*> m_listners;

	uint m_size;
	
	//this number determines number of objects who need this resource,when it reaches 0,it'll call unload method to free up memory
	//if it raised to 1 again,it'll reload the resource
	uint m_usedCounter;

	virtual uint calcSizeInternal()=0;

	virtual void preLoadInternal(bool async);
	virtual void loadInternal();
	virtual void postLoadInternal();


	virtual void preUnloadInternal();
	virtual void unloadInternal();
	virtual void postUnloadInternal();
	

	void fillProperties();

	void notifyResourceListners(bool load);

	void setResourceManager(IResourceManager*manager);

	friend class IResourceManager;

	void _onLoaded();//must be called after it is loaded 
public:
	IResource();


	virtual~IResource();

	virtual void _onResourceUsed();

	virtual void useResource();
	virtual void unuseResource();

	virtual bool load(bool async=true);
	
	virtual void unload();
	virtual bool reload();
	
	virtual uint recalcSize(){
		m_size=calcSizeInternal();
		return m_size;
	}

	ELoadingState getResourceState()const;

	virtual uint getResourceSize()const{return m_size;}

	inline void setResourcePriority(EPriorityLevel level){ m_priorityLevel=level;	}
	inline EPriorityLevel getResourcePriority()const{ return m_priorityLevel;	}

	inline void setLastResourceAccess(time_t lastAccess){m_lastAccess=lastAccess; }
	inline time_t getLastResourceAccess()const{return m_lastAccess; }

	void setResourceHandle(const GUID& handler);
	void setResourceName(const  core::string&name);
	void setResourceGroup(const core::string&group);

	inline const GUID& getResourceHandle()const
	{
		return m_handle;
	}

	inline const core::string&getResourceName()const
	{
		return m_name;
	}
	
	inline const core::string&getResourceGroup()const
	{
		return m_group;
	}

	void addResourceListner(ResourceListner*listner);

	virtual bool operator <(const IResource&o)const;

	
	
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement*elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement*elem);
};

MakeSharedPtrType(IResource)

}


#endif


