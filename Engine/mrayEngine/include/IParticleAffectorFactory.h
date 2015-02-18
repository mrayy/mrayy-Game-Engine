


#ifndef ___IParticleAffectorFactory___
#define ___IParticleAffectorFactory___

#include "mstring.h"


namespace mray{
namespace scene{

class IParticleAffector;


class IParticleAffectorFactory
{
public:
	virtual core::string getType()=0;
	virtual bool canCreate(const core::string&type)=0;

	virtual IParticleAffector*createAffector()=0;
};

}
}






#endif

