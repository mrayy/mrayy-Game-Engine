


#ifndef ___SoundManagerFactory___
#define ___SoundManagerFactory___

#include "ISingleton.h"
#include "ObjectFactoryManager.h"

namespace mray{
namespace sound{
	class ISoundManager;

class MRAY_DLL SoundManagerFactory:public ISingleton<SoundManagerFactory>,public ObjectFactoryManager<ISoundManager>
{
	core::string m_defaultManager;
public:

	SoundManagerFactory();
	virtual~SoundManagerFactory();

	
	void SetDefaultManager(const core::string& type);
	ISoundManager* CreateDefaultManager();

};

}
}




#endif


