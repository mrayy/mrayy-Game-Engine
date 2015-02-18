

#ifndef ___GUIParticleSystem___
#define ___GUIParticleSystem___

#include "GUIElement.h"
#include "GUIParticle.h"
#include "GUIRotImage.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIParticleSystem:public GUIElement
{
	std::list<GCPtr<GUIParticle>> particles;
	std::list<GCPtr<GUIParticle>> deadParticles;

	GCPtr<GUIRotImage> m_particleShape;
	
	float particlesRate;
	float currRate;
	int particlesCount;
	int particlesGen;

	//! this function used to setup particle position when they born
	virtual void setupParticlePosition(GCPtr<GUIParticle>particle){
	};
	void reSpawn(GCPtr<GUIParticle>part);
public:
	DECLARE_RTTI;

	GUIParticleSystem(GUIManager*manager);
	virtual ~GUIParticleSystem();

	void setParticlesRate(float rate);
	void setParticlesCount(int count);
	void setParticlesLifeSpawn(float minLife,float maxLife);

	void setParticleAcceleration(math::vector2d acc);
	void setParticleVelocity(math::vector2d vel);

	void setEmitColor(video::SColor minClr,video::SColor maxClr);
	void setDieColor(video::SColor minClr,video::SColor maxClr);
	void setEmitSize(math::vector2d minSize,math::vector2d maxSize);
	void setDieSize(math::vector2d minSize,math::vector2d maxSize);
	void setMinMaxRotSpeed(float min,float max);

	
	void addDeadParticle(GCPtr<GUIParticle>part);

	void reset();

	//void setShape(GCPtr<GUIElment>shape);
	GCPtr<GUIRotImage> getShapeNode();


	virtual void draw(float dt);

public:

	
	float minLifeSpawn;
	float maxLifeSpawn;
	
	video::SColor minEmitColor;
	video::SColor maxEmitColor;
	
	video::SColor minDieColor;
	video::SColor maxDieColor;

	math::vector2d minEmitSize;
	math::vector2d maxEmitSize;
	
	math::vector2d minDieSize;
	math::vector2d maxDieSize;

	math::vector2d acceleration;
	math::vector2d velocity;

	float minRotSpeed;
	float maxRotSpeed;

};

}
}






#endif


