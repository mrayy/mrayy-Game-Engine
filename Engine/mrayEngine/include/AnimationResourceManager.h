
/********************************************************************
	created:	2009/06/23
	created:	23:6:2009   18:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\AnimationResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	AnimationResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AnimationResourceManager___
#define ___AnimationResourceManager___


#include "IResourceManager.h"
#include "ISingleton.h"
#include "IStream.h"
#include "TransformationAnimationMixer.h"
#include "IAnimLoader.h"

namespace mray{

class MRAY_DLL AnimationResourceManager:public IResourceManager,public ISingleton<AnimationResourceManager>
{
protected:
	std::list<loaders::IAnimLoaderPtr> m_Loaders;

	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	AnimationResourceManager();
	virtual~AnimationResourceManager();

	void addAnimLoader(const loaders::IAnimLoaderPtr& loader);

	GCPtr<animation::TransformationAnimationMixer> getAnimation(const core::string&name);
	void loadAnimationFolder(const  core::string& fname);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gAnimationResourceManager mray::AnimationResourceManager::getInstance()

}


#endif //___AnimationResourceManager___
