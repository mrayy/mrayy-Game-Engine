
/********************************************************************
	created:	2009/08/01
	created:	1:8:2009   19:35
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GPUParticleBillboardRenderer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GPUParticleBillboardRenderer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Batch Renderer for Particles , supports Billboard rendering
				in shader you have to take care of the following streams:
				Position 0 : contains quad positions for particles, centered at (0,0,0) with size of (1,1)
				Texcoord 0 : Texture Coordinates
				Texcoord 1 : [Trans X,Y,Z]
				Texcoord 2 : [Sin(a),Cos(a),ScaleX,ScaleY], where "a" is rotation angle for the particle
				Texcoord 3 : Color Value [RGBA]
*********************************************************************/

#ifndef ___GPUParticleBillboardRenderer___
#define ___GPUParticleBillboardRenderer___

#include "IParticleRenderer.h"

#include "MeshBufferData.h"
#include "IHardwareIndexBuffer.h"


namespace mray{
namespace scene{

	class IParticle;

class MRAY_DLL GPUParticleBillboardRenderer:public IParticleRenderer
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
	IViewNode* m_vnode;
	IMeshBuffer* m_meshBuffer;
	video::IndexData* m_indecies;
	bool m_locked;

	math::vector4d* m_tc1Ptr;
	math::vector4d* m_tc2Ptr;
	math::vector4d* m_tc3Ptr;
	
	int m_maxParticles;
	int m_usedParticles;
public:
	GPUParticleBillboardRenderer(ushort maxParticles);
	virtual~GPUParticleBillboardRenderer();

	void reset(IViewNode*vnode);
	void addParticle(IParticle* p);
	void addParticle(const math::vector3d&pos,float angle,const math::vector2d&scale,const video::SColor&clr);
	void done();


	IMeshBuffer* getMeshBuffer();
	video::IndexData* getIndecies();
};

}
}


#endif //___GPUParticleBillboardRenderer___
