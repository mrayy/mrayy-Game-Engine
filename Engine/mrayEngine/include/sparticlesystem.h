

#ifndef ___SPARTICAL_SYSTEM___
#define ___SPARTICAL_SYSTEM___


#include "IRenderable.h"



namespace mray{
namespace scene{

	class ParticleEmitter;

class MRAY_DLL SParticleSystem:public IRenderable
{
protected:
	std::vector<ParticleEmitter*> m_emitters;
public:

	SParticleSystem();
	virtual~SParticleSystem();

	virtual bool isCustomRender(){return true;}

	ParticleEmitter* CreateEmitter();
	void AddEmitter(ParticleEmitter*e);
	int GetEmittersCount();
	ParticleEmitter* GetEmitter(int i);
	void RemoveEmitter(int i);

	void ClearEmitters();

	virtual bool preRender(IRenderPass*pass);
	bool render(IRenderPass*pass);
	void update(float dt);

	void AddUpdateJobs(ThreadJobManager* mngr,float dt);

	virtual void setMaterial(const video::RenderMaterialPtr& mtrl,int i=0);
	virtual const video::RenderMaterialPtr& getMaterial(int i=0);
	virtual int getMaterialCount();

	virtual void setShader(int mesh,const video::GPUShaderPtr& shader);
	virtual void setShaderCallback(int mesh,const video::IShaderConstantsCallbackPtr& callback);

	virtual const video::GPUShaderPtr& getShader(int i);

	virtual math::box3d getBoundingBox();

	ParticleEmitter* LoadEmitter(xml::XMLElement*elem);
	ParticleEmitter* LoadEmitter(const core::string&path);
	void LoadSystem(const core::string&path);
};

}
}


#endif





