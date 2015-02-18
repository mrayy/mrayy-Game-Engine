
#ifndef ___GUIParticle___
#define ___GUIParticle___





#include "GUIElement.h"

namespace mray{
namespace GUI{

class GUIParticleSystem;

class MRAY_DLL GUIParticle:public GUIElement
{
	GUIParticleSystem* pSystem;
	float m_lastRot;
public:

	DECLARE_RTTI;

	float R,G,B,A;
	bool bAlive;
	float mainLifeSpawn;
	float lifeSpawn;

	video::SColor color;
	math::vector3d colorDT;
	float AlphaDT;

	math::vector2d sizeDT;
	float	rotSpeed;

	math::vector2d force;
	math::vector2d acceleration;
	math::vector2d velocity;

	GUIParticle(GUIParticleSystem*system);
	virtual ~GUIParticle();


	virtual void draw(float dt);
};

}
}






#endif


