

#ifndef ___SParticleSystemFactory___
#define ___SParticleSystemFactory___


#include "GCPtr.h"
#include "ISingleton.h"


namespace mray{
namespace scene{
	class ICustomParticleEmitter;
	class IParticleAffector;
	class SParticleSystem;
	class IParticleEmitterFactory;
	class IParticleAffectorFactory;


class MRAY_DLL SParticleSystemFactory:public ISingleton<SParticleSystemFactory>
{
	typedef std::map<core::string,IParticleEmitterFactory*> ParticleSystemF;
	typedef std::map<core::string,IParticleAffectorFactory*> ParticleAffectorF;

	ParticleSystemF m_PSfactories;
	ParticleAffectorF m_Affectorfactories;
public:
	SParticleSystemFactory();
	virtual~SParticleSystemFactory();

	void addEmitterFactory(IParticleEmitterFactory *f);
	IParticleEmitterFactory *getEmitterFactory(const core::string&type);

	void addAffectorFactory(IParticleAffectorFactory *f);
	IParticleAffectorFactory* getAffectorFactory(const core::string&type);

	ICustomParticleEmitter *createParticleEmitter(const core::string&type);
	
	IParticleAffector*createParticleAffector(const core::string&type);
	
	void Clear();

	void removeParticleEmitter(const core::string&type);
	void removeParticleAffector(const core::string&type);

	void listParticleEmitters(std::vector<core::string> &out);
	void listParticleAffector(std::vector<core::string> &out);
};

}
}





#endif



