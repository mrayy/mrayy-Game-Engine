

#ifndef ___SoundResourceManager___
#define ___SoundResourceManager___


#include "IResourceManager.h"
#include "ISingleton.h"
#include "ISound.h"
#include "SoundLoader.h"

namespace mray{

class MRAY_DLL SoundResourceManager:public IResourceManager,public ISingleton<SoundResourceManager>
{
	GCPtr<loaders::SoundLoader> m_soundLoader;

protected:
	virtual IResourcePtr createInternal(const core::string& name);
	
	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	void addSoundLoader(loaders::ISoundLoader*loader);

	virtual core::string getDefaultGroup();
public:
	SoundResourceManager();
	virtual~SoundResourceManager();

	virtual IResourcePtr getOrCreate(const core::string& name);

	sound::ISoundPtr  loadSound(const core::string& name);
	
	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gSoundResourceManager mray::SoundResourceManager::getInstance()

}


#endif


