

/********************************************************************
	created:	2009/05/06
	created:	6:5:2009   13:18
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\JetParticleSystem.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	JetParticleSystem
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___JetParticleSystem___
#define ___JetParticleSystem___

#include <SParticleSystem.h>

namespace mray{
namespace gameMod{

class JetParticleSystem:public scene::SParticleSystem
{
private:
protected:
	virtual void setupParticlePosition(GCPtr<scene::IParticle>particle);
public:

	float JetRadius;

	JetParticleSystem();
	virtual~JetParticleSystem();

};

}
}


#endif //___JetParticleSystem___
