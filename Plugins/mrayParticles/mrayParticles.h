
#ifndef ___mrayParticleEmittersPlugin___
#define ___mrayParticleEmittersPlugin___

#include <SParticleSystemFactory.h>
#include <IPlugin.h>

#include "BoxParticleEmitter.h"
#include "MeshParticleEmitter.h"
#include "RingParticleEmitter.h"
#include "SphereParticleEmitter.h"


#include "IPABlocker.h"
#include "PAColorInterpolater.h"
#include "PAScaleInterpolater.h"
#include "PAVelocityInterpolater.h"
#include "PARotationInterpolater.h"


namespace mray{
namespace plugins{

class MRAY_PS_DLL mrayParticleEmittersPlugin:public IPlugin
{
	GCPtr<scene::SBoxPEFactory> m_boxPS;
	//GCPtr<scene::SMeshPSFactory> m_meshPS;
	GCPtr<scene::SRingPSFactory> m_ringPS;
	GCPtr<scene::SSpherePSFactory> m_spherePS;

	GCPtr<scene::PlaneBlockerFactory> m_planeBPA;
	GCPtr<scene::BoxBlockerFactory> m_boxBPA;
	GCPtr<scene::SphereBlockerFactory> m_sphereBPA;
	GCPtr<scene::ColorInterpolaterFactory> m_colorInterPA;
	GCPtr<scene::ScaleInterpolaterFactory> m_scaleInterPA;
	GCPtr<scene::VelocityInterpolaterFactory> m_velocityInterPA;
	GCPtr<scene::RotationInterpolaterFactory> m_rotationInterPA;
public:
	mrayParticleEmittersPlugin();
	virtual~mrayParticleEmittersPlugin();

	virtual core::string getName();

	virtual void install();
	virtual void uninstall();
};


}
}

#endif