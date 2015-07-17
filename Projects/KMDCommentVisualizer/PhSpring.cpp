

#include "stdafx.h"
#include "PhSpring.h"
#include "PhNode.h"




namespace mray
{
namespace physics
{

PhSpring::PhSpring(PhNode* a, PhNode* b, float strength,float length)
{
	m_a = a;
	m_b = b;
	m_strength = strength;
	m_length = length;
	m_damp = 0.5;
}

PhSpring::~PhSpring()
{

}

void PhSpring::Update(float dt)
{
	if (m_a->IsFixed() && m_b->IsFixed())
		return;

	math::vector2d a = m_a->GetPosition();
	math::vector2d b = m_b->GetPosition();
	math::vector2d dir = (a - b);
	math::vector2d dv = m_a->GetVelocity() - m_b->GetVelocity();
	float l = dir.Length();
	float d = l - m_length;
	if (l == 0)
		return;
	dir /= l;
	math::vector2d f = -(m_strength*d /*+ m_damp*dv.dotProduct(dir)*/)*dir - m_damp*dv;

	if (!m_a->IsFixed())
		m_a->AddForce(f);
	if (!m_b->IsFixed())
		m_b->AddForce(f.invert());

}

}
}
