
#ifndef ___IOGGLoader___
#define ___IOGGLoader___

#include "ISoundLoader.h"

#include "compileConfig.h"

namespace mray
{

	namespace sound
	{
		class ISoundStreamFrame;
	}
namespace loaders
{

class MRAY_SOUND_DLL IOGGLoader:public ISoundLoader
{
public:

	virtual sound::ISoundStreamFrame* load(const mchar*filename);
	virtual bool canLoad(const mchar*filename);

};


};//loaders
};//mray

#endif




