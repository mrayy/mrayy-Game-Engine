

#ifndef ___MeshParticleEmitter___
#define ___MeshParticleEmitter___


#include <SParticleSystem.h>
#include <IParticleEmitterFactory.h>

#include "compileConfig.h"

namespace mray{
namespace scene{

#ifdef __TODO__
class IMeshBuffer;
class MRAY_PS_DLL MeshParticleEmitter:public ICustomParticleEmitter
{
	DECLARE_RTTI;
protected:
	core::array<GCPtr<SMeshBuffer>>m_meshs;
	ISceneNode*m_node;

	int m_lastMesh;
	int m_lastVertex;
	
	virtual void setupParticlePosition(GCPtr<IParticle>particle);
public:
	MeshParticleEmitter();
	virtual~MeshParticleEmitter();

	void setNode(ISceneNode*node);

	void addMesh(GCPtr<SMeshBuffer> mesh);
	void addMesh(GCPtr<SMesh> mesh);

	int getMeshCount();
	GCPtr<SMeshBuffer> getMesh(int index);

};


class MRAY_PS_DLL SMeshPSFactory:public IParticleFactory
{
public:
	static const mchar* static_getType();
	virtual const mchar* getType();
	virtual bool canCreate(const mchar*type);

	virtual SParticleSystem*createParticleSystem();
};
#endif
}
}



#endif

