

#ifndef ___SoundLoader___
#define ___SoundLoader___




#include "GCPtr.h"

#include "ISoundLoader.h"

namespace mray{
namespace loaders{


class MRAY_DLL SoundLoader
{
	std::vector<GCPtr<ISoundLoader>> Loaders;
public:
	SoundLoader(){};
	virtual ~SoundLoader();
	void addLoader(GCPtr<ISoundLoader>loader);

	sound::ISoundStreamFrame *load(const  mchar*name);
	sound::ISoundStreamFrame *load(OS::IStream* file);
};

};//loaders
};//mray

#endif
