


/********************************************************************
	created:	2010/10/22
	created:	22:10:2010   15:04
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\CPUParticleBillboardRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	CPUParticleBillboardRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CPUParticleBillboardRenderer_h__
#define CPUParticleBillboardRenderer_h__

#include "IParticleRenderer.h"

#include "MeshBufferData.h"
#include "IHardwareIndexBuffer.h"


namespace mray
{
namespace scene
{

class MRAY_DLL CPUParticleBillboardRenderer:public IParticleRenderer
{
private:
protected:

	struct ParticleInfo
	{
		float distToCam;
		IParticle*part;

		ParticleInfo():distToCam(0),part(0)
		{
		}
		ParticleInfo(float d,IParticle*p):distToCam(d),part(p)
		{
		}

		bool operator<(const ParticleInfo&o)const
		{
			return distToCam<o.distToCam;
		}
	};

	std::vector<ParticleInfo> m_particles;

	IMeshBuffer* m_meshBuffer;
	video::IndexData* m_indecies;
	int m_maxParticles;
	int m_usedParticles;
	bool m_locked;

	math::vector3d* m_posPtr;
	video::SColor* m_clrPtr;

	IViewNode* m_vnode;

	math::vector3d m_ver;
	math::vector3d m_hor;

	bool m_rotatable;

public:
	CPUParticleBillboardRenderer(ushort maxParticles);
	virtual ~CPUParticleBillboardRenderer();

	void reset(IViewNode*vnode);
	void addParticle(IParticle* p);
	void addParticle(const math::vector3d&pos,float angle,const math::vector2d&scale,const video::SColor&clr);
	void done();

	void EnableRotate(bool e){m_rotatable=e;}
	bool IsRotateEnabled(){return m_rotatable;}

	IMeshBuffer* getMeshBuffer();
};

}
}

#endif // CPUParticleBillboardRenderer_h__
