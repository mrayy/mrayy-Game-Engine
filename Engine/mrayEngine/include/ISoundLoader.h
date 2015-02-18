

#ifndef ___ISoundLoader___
#define ___ISoundLoader___


#include "mString.h"


namespace mray{
	namespace OS{
		class IStream;
	}
	namespace sound{
		class ISoundStreamFrame;
	}
namespace loaders{
class ISoundLoader
{
public:

	virtual~ISoundLoader(){};

	virtual sound::ISoundStreamFrame*  load(const  mchar*fileName)=0;
	virtual sound::ISoundStreamFrame*  load(OS::IStream* file)=0;
	virtual bool canLoad(const  mchar*fileName)=0;
};
};//loaders
};//mray


#endif



