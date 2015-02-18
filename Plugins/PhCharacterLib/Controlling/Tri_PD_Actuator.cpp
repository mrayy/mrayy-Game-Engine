#include "stdafx.h"
#include "Tri_PD_Actuator.h"
#include "..\Character.h"
#include "..\PhysicalModel\Segment.h"
//#include <NxPhysics.h>

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		Tri_PD_Actuator::Tri_PD_Actuator()
		{
			 m_parent=0;
			 m_child1=0;
			 m_child2=0;
			m_params = 0;
		}

		Tri_PD_Actuator::~Tri_PD_Actuator()
		{
			delete m_params;
		}

		void Tri_PD_Actuator::load( xml::XMLElement*elem, Character* character )
		{
			m_name = elem->getAttribute(mT("name"))->value;
			m_parent = character->getSegment(elem->getAttribute(mT("parent"))->value);
			m_child1 = character->getSegment(elem->getAttribute(mT("child1"))->value);
			m_child2 = character->getSegment(elem->getAttribute(mT("child2"))->value);
			setParams(createParams(elem->getSubElement(mT("Params"))));

		}

		ActuatorParams* Tri_PD_Actuator::createParams( xml::XMLElement*elem )
		{
			if (elem==0)
			{
				return 0;
			}
			
			Tri_PD_Params* params = new Tri_PD_Params();

			if (m_params!=0)
			{			
				params->parent_ks = m_params->parent_ks;
				params->parent_kd = m_params->parent_kd;
				params->child1_ks = m_params->child1_ks;
				params->child1_kd = m_params->child1_kd;
				params->child2_ks = m_params->child2_ks;
				params->child2_kd = m_params->child2_kd;
				params->freeChild = m_params->freeChild;
			}


			xml::xmlAttributesMapIT it= elem->getAttributesBegin();
			xml::xmlAttributesMapIT end= elem->getAttributesEnd();

			core::stringc attrName;


			for(;it!=end;++it)
			{
				xml::XMLAttribute&attr= it->second;
				core::string_to_char(attr.name,attrName);
				attrName.make_lower();		
				if(attrName=="parent_ks")
				{
					params->parent_ks = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="parent_kd")
				{
					params->parent_kd = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="child1_ks")
				{
					params->child1_ks = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="child1_kd")
				{
					params->child1_kd = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="child2_ks")
				{
					params->child2_ks = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="child2_kd")
				{
					params->child2_kd = core::StringConverter::toFloat(attr.value);
				}
				else if(attrName=="freechild")
				{
					params->freeChild = core::StringConverter::toInt(attr.value);
				}
			}

			return params;
		}

		void Tri_PD_Actuator::setParams( ActuatorParams* params )
		{
			m_params = static_cast<Tri_PD_Params*>(params);
		}

		void Tri_PD_Actuator::applyForces(float dt)
		{
			Segment* swingSegment;
			Segment* stanceSegment;
			float swingKs;
			float swingKd;
			if (m_params->freeChild == 1)
			{
				stanceSegment = m_child1;
				swingSegment = m_child2;
				swingKs = m_params->child2_ks;
				swingKd = m_params->child2_kd;
			}else
			if (m_params->freeChild == 2)
			{
				stanceSegment = m_child2;
				swingSegment = m_child1;
				swingKs = m_params->child1_ks;
				swingKd = m_params->child1_kd;
			}

			math::vector3d torsoTorque,stanceTorque,swingTorque;
			torsoTorque = m_parent->getPD_Torque(m_parent->startOrient, m_params->parent_ks, m_params->parent_kd);
			swingTorque =  swingSegment->getPD_Torque(m_parent->startOrient, swingKs, swingKd);
			stanceTorque = -torsoTorque- swingTorque;


			m_parent->applyRelativeTorque(m_parent->startOrient, torsoTorque);
			swingSegment->applyRelativeTorque(m_parent->startOrient, swingTorque);
			stanceSegment->applyRelativeTorque(m_parent->startOrient, stanceTorque);

			//swingSegment->setCrap(700,70);
			//swingSegment->setTargetOrientation(math::vector3d(0,0,0));


			//m_parent->setCrap(1000,100);
			//m_child1->setCrap(1000,100);
			//m_child2->setCrap(1000,100);

			//m_parent->setTargetOrientation(math::vector3d(0,0,0));
			//m_child1->setTargetOrientation(math::vector3d(0,0,0));
			//m_child2->setTargetOrientation(math::vector3d(0,0,0));
		}

		const core::string& Tri_PD_Actuator::getName()
		{
			return m_name;
		}


	}
}