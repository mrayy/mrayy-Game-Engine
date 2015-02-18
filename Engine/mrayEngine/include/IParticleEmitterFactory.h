

#ifndef ___IParticleEmitterFactory___
#define ___IParticleEmitterFactory___

#include "mTypes.h"
#include "mString.h"

namespace mray{
namespace scene{

class ICustomParticleEmitter;


class IParticleEmitterFactory
{
public:
	virtual const core::string& getType()=0;
	virtual bool canCreate(const core::string& type)=0;

	virtual ICustomParticleEmitter*createParticleEmitter()=0;
};

}
}






#endif




