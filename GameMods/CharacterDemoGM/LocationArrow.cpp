

#include "stdafx.h"
#include "LocationArrow.h"



namespace mray
{
namespace GameMod
{

LocationArrow::LocationArrow()
{
	m_node=0;
	m_timeSinceShown=0;
	m_lifeDuration=5;
}
LocationArrow::~LocationArrow()
{
}

void LocationArrow::SetShape(scene::ISceneNode*shape)
{
	m_node=shape;
}
scene::ISceneNode* LocationArrow::GetShape()
{
	return m_node;
}

void LocationArrow::SetLocation(const math::vector3d&pos)
{
	m_position=pos;
}

void LocationArrow::Show()
{
	if(m_node)
	{
		m_node->Visible=true;
		m_node->setOrintation(math::quaternion::Identity);
	}
	m_timeSinceShown=0;
}
void LocationArrow::Hide()
{
	if(m_node)
	{
		m_node->Visible=false;
	}
}

void LocationArrow::Update(float dt)
{
	if(!m_node)
		return;
	if(m_node->Visible==false)
		return;
	m_timeSinceShown+=dt;

	if(m_timeSinceShown>=m_lifeDuration)
	{
		Hide();
		return;
	}

	math::quaternion ori=m_node->getOrintation();
	ori*=math::quaternion(360*dt,math::vector3d::YAxis);
	m_node->setOrintation(ori);

	math::vector3d pos=m_position;
	pos.y+=abs(math::sind(4*360*m_timeSinceShown/m_lifeDuration));
	m_node->setPosition(pos);
}


}
}
