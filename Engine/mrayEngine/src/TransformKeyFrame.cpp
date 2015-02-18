#include "stdafx.h"

#include "TransformKeyFrame.h"



namespace mray{
namespace animation{
	


TransformKeyFrame::TransformKeyFrame(){
}
TransformKeyFrame::TransformKeyFrame(float time,const math::vector3d& pos,const math::vector3d& scale,const math::quaternion& orintation)
:IKeyFrame(time),m_pos(pos),m_scale(scale),m_orintation(orintation)
{
}

const math::vector3d& TransformKeyFrame::getPos()const
{
	return m_pos;
}
void TransformKeyFrame::setPos(const math::vector3d& v)
{
	m_pos=v;
}

const math::vector3d& TransformKeyFrame::getScale()const
{
	return m_scale;
}
void TransformKeyFrame::setScale(const math::vector3d& v)
{
	m_scale=v;
}

const math::quaternion& TransformKeyFrame::getOrintation()const
{
	return m_orintation;
}
void TransformKeyFrame::setOrintation(const math::quaternion& v)
{
	m_orintation=v;
}


IKeyFrame*TransformKeyFrame::duplicate()const
{
	return new TransformKeyFrame(getTime(),m_pos,m_scale,m_orintation);
}


}
}



