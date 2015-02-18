

/********************************************************************
	created:	2008/10/19
	created:	19:10:2008   20:05
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\SkeletonResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	SkeletonResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	The main resource manager for creating skeletons
*********************************************************************/


#ifndef ___SkeletonResourceManager___
#define ___SkeletonResourceManager___

#include "IResourceManager.h"
#include "ISingleton.h"
#include "Skeleton.h"
#include "ISkeletonLoader.h"
#include "ISkeletonExporter.h"

namespace mray{

class MRAY_DLL SkeletonResourceManager:public IResourceManager,public ISingleton<SkeletonResourceManager>
{
protected:
	std::list<GCPtr<loaders::ISkeletonLoader>> m_Loaders;
	std::list<GCPtr<loaders::ISkeletonExporter>> m_exporters;

	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	SkeletonResourceManager();
	virtual~SkeletonResourceManager();

	void addSkeletonLoader(GCPtr<loaders::ISkeletonLoader> loader);
	void addSkeletonWriter(GCPtr<loaders::ISkeletonExporter> e);

	GCPtr<scene::Skeleton> loadSkeleton(const core::string& name);

	GCPtr<scene::Skeleton>createSkeleton(const core::string& name);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);
};

}


#endif //___SkeletonResourceManager___

