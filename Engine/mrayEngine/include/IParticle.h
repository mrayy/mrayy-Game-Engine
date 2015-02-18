

#ifndef ___SPARTICAL___
#define ___SPARTICAL___




#include "SColor.h"
#include "Point4d.h"


namespace mray{
namespace scene{

class ParticleEmitter;
enum EParticleShape;

class MRAY_DLL IParticle
{
protected:
	ParticleEmitter* m_emitter;
	EParticleShape m_lastShape;
public:

	IParticle(ParticleEmitter*e);
	virtual ~IParticle();

	ParticleEmitter* GetEmitter();


	void setupParticleToPreRender(EParticleShape shape);

	virtual void render();
	virtual void update(float dt);

	math::box3d calculateBB()const;

public:

	math::vector3d lastPosition;
	math::vector3d position;
	float AngleZ;
	math::vector3d scale;


	bool bAlive;
	float mainLifeSpawn;
	float lifeSpawn;
	math::vector4d randVal;

	video::SColor color;

	math::vector3d force;
	math::vector3d acceleration;
	math::vector3d velocity;
};

}
}



#endif





