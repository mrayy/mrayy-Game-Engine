#include "stdafx.h"
#include "Parental_PD_Actuator.h"
#include "..\Character.h"
#include "..\PhysicalModel\Segment.h"

//#define SGN(x) (((x)<0)?(-1):(1))

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		Parental_PD_Actuator::Parental_PD_Actuator()
		{
			m_segment=0;
			m_params=0;
		}
		Parental_PD_Actuator::~Parental_PD_Actuator()
		{
			m_segment->setAngularDriveProperties(0, 0, 0, false, false);
			delete m_params;
		}

		void Parental_PD_Actuator::load( xml::XMLElement*elem, Character* character )
		{
			m_name = elem->getAttribute(mT("name"))->value;
			m_segment = character->getSegment(elem->getAttribute(mT("segment"))->value);
			setParams(createParams(elem->getSubElement(mT("Params"))));
		}

		ActuatorParams* Parental_PD_Actuator::createParams( xml::XMLElement*elem )
		{
			if (elem==0)
			{
				return 0;
			}

			Parental_PD_Params* params = new Parental_PD_Params();

			params->ks = core::StringConverter::toFloat(elem->getAttribute(mT("ks"))->value);
			params->kd = core::StringConverter::toFloat(elem->getAttribute(mT("kd"))->value);

			return params;
		}

		void Parental_PD_Actuator::setParams( ActuatorParams* params )
		{
			m_params = static_cast<Parental_PD_Params*>(params);
		}

		void Parental_PD_Actuator::applyForces(float dt)
		{
			m_segment->setTargetOrientation(m_segment->target);
			m_segment->setAngularDriveProperties(m_params->ks, m_params->kd, math::Infinity, true, false);

		}

		const core::string& Parental_PD_Actuator::getName()
		{
			return m_name;
		}
	}
}