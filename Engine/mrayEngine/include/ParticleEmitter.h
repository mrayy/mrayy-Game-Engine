

/********************************************************************
	created:	2010/10/21
	created:	21:10:2010   15:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ParticleEmitter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ParticleEmitter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ParticleEmitter_h__
#define ParticleEmitter_h__

#include "TypedProperty.h"

#include "RenderMaterial.h"
#include "CKeyableAttr.h"

#include "IParticleRenderer.h"
#include "IParticleEmitterFactory.h"
#include "IJobPiece.h"
#include "PoolMemoryAllocator.h"

namespace mray
{
	class IJobPiece;
	class ThreadJobManager;

namespace scene
{

	enum EParticleShape
	{
		EPS_Point,
		EPS_Line,
		EPS_Billboard
	};

	enum EParticleGenerateMode
	{
		EPGM_Infinite,
		EPGM_Cycle
	};


	class IParticleRenderer;
	class IParticle;
	class SParticleSystem;
	class IParticleAffector;
	class IRenderPass;
	class ICustomParticleEmitter;

class MRAY_DLL ParticleEmitter:public IObject,public IJobPiece::Listener
{
	DECLARE_RTTI
public:
	//typedef std::list<IParticle* > ParticleList;
	typedef std::list<IParticle*> ParticleList;/*,PoolMemoryAllocator<IParticle*>*/
	typedef std::list<IParticleAffector*> ParticleAffectorList;
protected:
	ParticleList m_particles;
	ParticleList m_deadParticles;
	ParticleAffectorList m_particleAffectors;

	std::list<IJobPiece*> m_jobPieces;
	int m_totalJobsRequested;

	video::RenderMaterialPtr m_material;

	bool m_stopEmitting;

	float m_currRate;
	int m_particlesGen;

	float m_currLifeSpawn;

	float m_time;

	float m_cycleLength;

	EParticleShape m_particleShapeType;

	EParticleGenerateMode m_generateMode;

	virtual void setupParticle(IParticle* particle);

	virtual IParticle*  createParticle();

	math::box3d m_boundingBox;

	math::vector3d m_acceleration;
	math::vector3d m_velocity;

	core::string m_name;

	SParticleSystem* m_system;

	bool m_visible;

	IParticleRenderer* m_renderer;


	ICustomParticleEmitter* m_emitter;

	virtual void _ApplyAffectors(float dt);
	virtual void _SpawnParticles(float dt);

public:

	ParticleEmitter();
	virtual ~ParticleEmitter();

	void SetSystem(SParticleSystem* s){m_system=s;}
	SParticleSystem* GetSystem(){return m_system;}

	void ClearParticles();
	void ClearEffecters();

	bool IsEmpty()const;

	void SetRenderer(IParticleRenderer*r);
	IParticleRenderer* GetRenderer();

	void setName(const core::string&name);
	const core::string& getName();

	void setGenerateMode(EParticleGenerateMode mode);
	EParticleGenerateMode getGenerateMode();

	void setCycleLength(float length);
	float getCycleLength();

	void addAffector(IParticleAffector*affector);
	void removeAffector(IParticleAffector*affector);

	void setParticleShapeType(EParticleShape type);
	EParticleShape getParticleShapeType();

	void setParticleAcceleration(const math::vector3d& acc);
	void setParticleVelocity(const math::vector3d& vel);

	int getTotalGeneratedParticles();
	int getCurrentParticlesCount();

	void stopEmitting();
	void resumeEmitting();
	bool isStopEmitting();

	void addDeadParticle(IParticle* part);
	virtual void reSpawn(IParticle* part);

	void reset();

	virtual void setMaterial(const video::RenderMaterialPtr&mtrl);
	virtual const video::RenderMaterialPtr& getMaterial();

	void setTexture(video::ITexturePtr  tex);

	virtual void preRender(IRenderPass*pass);
	virtual void render(IRenderPass*pass);
	virtual void update(float dt);
	void AddUpdateJobs(ThreadJobManager* mngr,float dt);

	const math::box3d& GetBoundingBox()const{return m_boundingBox;}

	ParticleList& getParticles();

	bool LoadFromXML(xml::XMLElement*elem);

	void _UpdateParticleMT(IParticle* particle,float dt);

	virtual void SignalJobDone(IJobPiece*);
public:
	core::CKeyableAttr<float> particlesRate;
	core::CKeyableAttr<int> particlesCount;
	core::CKeyableAttr<float> particlesLifeSpawn;
};

}
}

#endif // ParticleEmitter_h__
