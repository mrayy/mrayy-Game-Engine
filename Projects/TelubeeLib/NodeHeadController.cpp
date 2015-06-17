

#include "stdafx.h"
#include "NodeHeadController.h"
#include "IMovable.h"


namespace mray
{
namespace TBee
{
NodeHeadController::NodeHeadController()
{
	m_node = 0;
}
NodeHeadController::~NodeHeadController()
{
}

bool NodeHeadController::GetHeadOrientation(math::quaternion&v, bool abs)
{
	if (m_node){
		math::vector3d a;
		if (!abs)
		{
			(m_node->getAbsoluteOrintation()*m_initialOrientation.inverse()).toEulerAngles(a);
		}else
			m_node->getAbsoluteOrintation().toEulerAngles(a);

		 v= math::quaternion(a.x, -a.y, -a.z);
		 return true;
	}
	return false;
}
bool NodeHeadController::GetHeadPosition(math::vector3d&v, bool abs)
{
	if (m_node)
	{
		v = m_node->getAbsolutePosition();
		return true;
	}
	return false;
}
void NodeHeadController::Recalibrate()
{
	if (m_node)
		m_initialOrientation = m_node->getAbsoluteOrintation();
}

}
}

