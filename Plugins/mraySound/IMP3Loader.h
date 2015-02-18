


#ifndef ___IMP3Loader___
#define ___IMP3Loader___

#include <ISoundLoader.h>

#include "compileConfig.h"

namespace mray{
namespace loaders{
class MRAY_SOUND_DLL IMP3Loader:public ISoundLoader
{

public:

	virtual sound::ISoundStreamFrame*  load(const  mchar*filename);
	virtual sound::ISoundStreamFrame* load(OS::IStream* file);
	virtual bool canLoad(const  mchar*filename);

};


};//loaders
};//mray


#endif







