#include "stdafx.h"

#include "PointSoundEmitter.h"
#include "ISound.h"

namespace mray
{
namespace sound
{


PointSoundEmitter::PointSoundEmitter():m_sound(0)
{
}
PointSoundEmitter::~PointSoundEmitter()
{
}

void PointSoundEmitter::SetPoint(const math::vector3d&b)
{
	m_origin=b;
}
const math::vector3d& PointSoundEmitter::GetPoint()
{
	return m_origin;
}

void PointSoundEmitter::AttachSound(ISound* s)
{
	m_sound=s;
}
ISound* PointSoundEmitter::GetAttachedSound()
{
	return m_sound;
}

void PointSoundEmitter::Update(ISoundListener* l)
{
	if(m_sound)
		m_sound->setPosition(m_origin);
}


}
}

