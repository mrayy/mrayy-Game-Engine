#include "stdafx.h"

#include "BoxSoundEmitter.h"

#include "ISound.h"
#include "ISoundListener.h"

namespace mray
{
namespace sound
{

BoxSoundEmitter::BoxSoundEmitter():m_sound(0)
{
}
BoxSoundEmitter::~BoxSoundEmitter()
{
}

void BoxSoundEmitter::SetBox(const math::box3d&b)
{
	m_box=b;
}
const math::box3d& BoxSoundEmitter::GetBox()
{
	return m_box;
}

void BoxSoundEmitter::AttachSound(ISound* s)
{
	m_sound=s;
}
ISound* BoxSoundEmitter::GetAttachedSound()
{
	return m_sound;
}

void BoxSoundEmitter::Update(ISoundListener* l)
{
	if(m_sound)
	{
		math::vector3d p,lp(l->getPosition());
		p.x=math::clamp(lp.x,m_box.MinP.x,m_box.MaxP.x);
		p.y=math::clamp(lp.y,m_box.MinP.y,m_box.MaxP.y);
		p.z=math::clamp(lp.z,m_box.MinP.z,m_box.MaxP.z);
		m_sound->setPosition(p);
		if(!m_box.isPointInSide(p))
			m_sound->setDirection((l->getPosition()-p).Normalize());
	}
}

}
}




