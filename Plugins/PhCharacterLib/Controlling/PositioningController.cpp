#include "stdafx.h"
#include "PositioningController.h"
//#include <NxPhysics.h>
//#include "physXCommon.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		PositioningController::PositioningController()
		{
			m_elapsed = 0;
			m_params=0;
			m_root=0;
		}
		PositioningController::~PositioningController()
		{
			delete m_params;
		}

		void PositioningController::load( const core::string& descFile, Character* character )
		{
			m_root = character->getRoot();
			m_root->getAllChildren(m_children);

			xml::XMLTree xmlTree;
			GCPtr<OS::IStream> stream=gFileSystem.openFile(descFile,OS::TXT_READ);
			if(!xmlTree.load(stream)){
				stream->close();
				gLogManager.log(mT("Couldn't parse XML Font File :\"")+descFile+mT("\""),ELL_WARNING);
				return;
			}
			stream->close();
			xml::XMLElement*e=xmlTree.getSubElement(mT("Controller"));

			if(!e)
			{
				gLogManager.log(mT("cann't find Controller tag :\"")+descFile+mT("\""),ELL_WARNING);
				return;
			}

			speed = core::StringConverter::toFloat(e->getAttribute(mT("speed"))->value);
			m_target = core::StringConverter::toFloat(e->getAttribute(mT("time"))->value);
			m_ks = core::StringConverter::toFloat(e->getAttribute(mT("ks"))->value);
			m_kd = core::StringConverter::toFloat(e->getAttribute(mT("kd"))->value);
			m_name = e->getAttribute(mT("name"))->value;
		}

		void PositioningController::setControllingParams( ControllingParams* params )
		{
			m_params =  static_cast<PositioningParams*>(params);
		}

		void PositioningController::reset()
		{
			m_elapsed = 0;
		}

		void PositioningController::stop()
		{

		}

		bool PositioningController::isFinished()
		{
			if (m_root->getGlobalTranslation().y >= m_params->pos.y)
			{
				m_root->getActor()->SetBodyFlag(physics::EBF_DisableGravity,false);
				return true;
			}
			return false;
		}

		void PositioningController::update( float dt )
		{
			if (m_elapsed>=m_target)
			{
				if (m_root->getGlobalTranslation().y < m_params->pos.y)
				{
					m_root->getActor()->SetBodyFlag(physics::EBF_DisableGravity,true);
					m_root->getActor()->setLinearVelocity(math::vector3d(0,speed,0));

					SegmentList::iterator it = m_children.begin();
					for (;it!=m_children.end();++it)
					{
						if ((*it)==m_root)
							(*it)->setTargetOrientation(m_params->orient);
						else
							(*it)->setTargetOrientation(math::vector3d(0,0,0));

						(*it)->setAngularDriveProperties(m_ks,m_kd,math::Infinity,true,false);
					}
				}				
			}
			else
				m_elapsed += dt;
			
		}

		const core::string& PositioningController::getName()
		{
			return m_name;
		}
	}
}