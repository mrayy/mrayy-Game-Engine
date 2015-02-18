#include "stdafx.h"

#include "SphereSoundEmitter.h"

#include "ISound.h"
#include "ISoundListener.h"

namespace mray
{
namespace sound
{

SphereSoundEmitter::SphereSoundEmitter():m_sound(0)
{
}
SphereSoundEmitter::~SphereSoundEmitter()
{
}

void SphereSoundEmitter::SetSphere(const math::Sphere&b)
{
	m_sphere=b;
}
const math::Sphere& SphereSoundEmitter::GetSphere()
{
	return m_sphere;
}

void SphereSoundEmitter::AttachSound(ISound* s)
{
	m_sound=s;
}
ISound* SphereSoundEmitter::GetAttachedSound()
{
	return m_sound;
}

void SphereSoundEmitter::Update(ISoundListener* l)
{
	if(m_sound)
	{
		math::vector3d p=m_sphere.GetClosestPoint(l->getPosition());
		m_sound->setPosition(p);
		if(!m_sphere.IsPointInside(p))
			m_sound->setDirection((l->getPosition()-p).Normalize());
	}
}

}
}


