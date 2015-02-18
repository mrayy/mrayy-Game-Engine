#include "stdafx.h"

#include "LineSoundEmitter.h"

#include "ISound.h"
#include "ISoundListener.h"

namespace mray
{
namespace sound
{

LineSoundEmitter::LineSoundEmitter():m_sound(0)
{
}
LineSoundEmitter::~LineSoundEmitter()
{
}

void LineSoundEmitter::SetLine(const math::line3d&b)
{
	m_line=b;
}
const math::line3d& LineSoundEmitter::GetLine()
{
	return m_line;
}

void LineSoundEmitter::AttachSound(ISound* s)
{
	m_sound=s;
}
ISound* LineSoundEmitter::GetAttachedSound()
{
	return m_sound;
}

void LineSoundEmitter::Update(ISoundListener* l)
{
	if(m_sound)
	{
		math::vector3d p=m_line.ClosestPoint(l->getPosition());
		m_sound->setPosition(p);
		m_sound->setDirection((l->getPosition()-p).Normalize());
	}
}

}
}

