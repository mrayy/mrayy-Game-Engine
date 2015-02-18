#include "stdafx.h"
#include "CharacterGait.h"
#include "./Character.h"
#include "./Environment.h"
#include "./Controlling/GaitParams.h"
#include "./Controlling/PositioningParams.h"

#include "CharacterManager.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		CharacterGait::CharacterGait(xml::XMLElement*elem, Character* character)
		{
			params=0;
			m_character = character;

			m_actor =m_character->getSegment(mT("LeftFoot"))->getActor();
			character->GetCreator()->GetEnvironment()->registerActor(m_actor);
			isWalking = false;

			m_slot = core::StringConverter::toInt(elem->getAttribute(mT("slot_num"))->value);
			desiredHeight = core::StringConverter::toFloat(elem->getAttribute(mT("desired_height"))->value)*0.01;
			fallingThreshold = core::StringConverter::toFloat(elem->getAttribute(mT("falling_threshold"))->value)*0.01;
			maxLinearSpeed = core::StringConverter::toFloat(elem->getAttribute(mT("max_linear_speed"))->value);
			maxAngularSpeed = core::StringConverter::toFloat(elem->getAttribute(mT("max_angular_speed"))->value);

			positioningController = elem->getAttribute(mT("positioning"))->value;
			standingController = elem->getAttribute(mT("standing"))->value;
			slowController = elem->getAttribute(mT("slow"))->value;
			mediumController = elem->getAttribute(mT("medium"))->value;
			fastController = elem->getAttribute(mT("fast"))->value;
			gaitController = mediumController;
			isFalling = false;
			isPositioning = false;
			params = new GaitParams();
			params->facingDirection = -180;
			linearSpeed = maxLinearSpeed/2;
			params->turningSpeed = 0;
			 m_character->getRoot()->getAllChildren(segments);
		}

		CharacterGait::~CharacterGait()
		{
			m_character->GetCreator()->GetEnvironment()->unRegisterActor(m_actor);
			delete params;
		}

		float CharacterGait::GetMass()
		{
			float m =0;
			for (SegmentList::iterator it = segments.begin(); it!=segments.end();++it)
			{
				m += (*it)->getMass();
			}
			return m;
		}

		void CharacterGait::SetGlobalPosition( const math::vector3d&pos )
		{
			math::vector3d transform =  pos - m_character->getRoot()->getGlobalTranslation();
			for (SegmentList::iterator it = segments.begin(); it!=segments.end();++it)
			{
				(*it)->setGlobalTranslation((*it)->getGlobalTranslation() +transform);
// 				(*it)->moveGlobalTranslation((*it)->getGlobalTranslation() +transform);
				(*it)->setGlobalLinearVelocity(math::vector3d::Zero);
				(*it)->setGlobalAngularVelocity(math::vector3d::Zero);
			}
		}

		math::vector3d CharacterGait::GetGlobalPosition()
		{
			return m_character->getRoot()->getGlobalTranslation();
		}

		void CharacterGait::SetGlobalOrintation( const math::quaternion&or )
		{
			math::vector3d zAxis = (or * math::vector3d.YAxis).invert();
			math::vector3d xAxis = (or * math::vector3d.ZAxis).invert();
			math::vector3d yAxis = zAxis.dotProduct(xAxis);

			math::matrix4x4 resMat;
			resMat.setComponent(xAxis, yAxis, zAxis);

			math::matrix4x4 resTrans;
			resMat.getTransposeMatrix(resTrans);
			math::quaternion resQ;
			resQ.fromMatrix(resTrans);
			
			math::matrix4x4 newRootToWorld;
			or.toMatrix(newRootToWorld);
			newRootToWorld.setTranslation(m_character->getRoot()->getGlobalTranslation());

			math::matrix4x4 oldRootToWorld;
			m_character->getRoot()->getGlobalOrientation().toMatrix(oldRootToWorld);
			oldRootToWorld.setTranslation(m_character->getRoot()->getGlobalTranslation());
			math::matrix4x4 worldToOldRoot;
			oldRootToWorld.getInverse(worldToOldRoot);
				
			math::matrix4x4 oldBodyToWorld;
			math::matrix4x4 newBodyToWorld;

			for (SegmentList::iterator it = segments.begin(); it!=segments.end();++it)
			{
				oldBodyToWorld.makeIdentity();

				(*it)->getGlobalOrientation().toMatrix(oldBodyToWorld);
				oldBodyToWorld.setTranslation((*it)->getGlobalTranslation());

				newBodyToWorld =  newRootToWorld * worldToOldRoot * oldBodyToWorld;

				//(*it)->moveGlobalOrientation(math::quaternion(newBodyToWorld));
				//(*it)->moveGlobalTranslation(newBodyToWorld.getTranslation());

				(*it)->setGlobalOrientation(math::quaternion(newBodyToWorld));
				(*it)->setGlobalTranslation(newBodyToWorld.getTranslation());

				(*it)->setGlobalLinearVelocity(math::vector3d::Zero);
				(*it)->setGlobalAngularVelocity(math::vector3d::Zero);
			}
		}

		math::quaternion CharacterGait::GetGlobalOrintation()
		{
			math::quaternion q = m_character->getRoot()->getGlobalOrientation();
			math::vector3d yAxis = (q * math::vector3d.ZAxis).invert();
			math::vector3d zAxis = (q * math::vector3d.XAxis).invert();
			math::vector3d xAxis = yAxis.dotProduct(zAxis);

			math::matrix4x4 resMat;
			resMat.setComponent(xAxis, yAxis, zAxis);

			math::matrix4x4 resTrans;
			resMat.getTransposeMatrix(resTrans);
			math::quaternion resQ;
			resQ.fromMatrix(resTrans);
			return resQ;
		}

		float CharacterGait::GetMaxSpeed()
		{
			return maxLinearSpeed;
		}


		float CharacterGait::GetMaxAngularVelocity()
		{
			return maxAngularSpeed;
		}

		math::vector3d CharacterGait::GetLinearVelocity()
		{
			return math::vector3d(math::cosd(params->facingDirection),0,math::sind(params->facingDirection)).setLength(linearSpeed);
		}

		math::vector3d CharacterGait::GetAngularVelocity()
		{
			return math::vector3d(0,params->turningSpeed,0);
		}

		void CharacterGait::SetLinearVelocity( const math::vector3d&v )
		{
			linearSpeed = v.Length();			
			if (linearSpeed!=0)
			{
				params->facingDirection = math::toDeg(atan2(v.z,v.x));
			}
			//MessageBox(0,"PlayController","",0);
			gaitController = resolveSpeed(linearSpeed);
			m_character->setParams(gaitController, params);
			if (gaitController!=m_character->getControllerName(m_slot) && m_character->getControllerName(m_slot)!=positioningController)
			{
				m_character->playController(gaitController,m_slot);
			}
		}

		void CharacterGait::SetAngularVelocity( const math::vector3d& v )
		{
			params->turningSpeed = v.y;
			m_character->setParams(gaitController, params);
		}

		void CharacterGait::update( float dt )
		{
			Environment* env=m_character->GetCreator()->GetEnvironment();
			if (env->isContacted(m_actor))
			{
				int i=0;
				i=i;
			}
			math::vector3d pos = m_character->getRoot()->getGlobalTranslation();
			float height = env->getHeightToGround(pos);
			
			if (height>=desiredHeight*0.7f)
			{
				isPositioning = false;
			}

			if (isFalling)
			{
				if (height < fallingThreshold)
				{
					isFalling = false;
					PositioningParams* posParam = new PositioningParams();
					posParam->pos.y = pos.y - height + desiredHeight;
					posParam->pos.x = pos.x;
					posParam->pos.z = pos.z;

					math::quaternion q = m_character->getRoot()->getGlobalOrientation();
					math::vector3d dir = (q * math::vector3d.XAxis).invert();
					float currentDirection = math::toDeg(atan2(dir.z,dir.x));

					math::matrix4x4 refMat;
					math::vector3d xVec,yVec,zVec;

					zVec.set(0,-1,0);
					zVec.Normalize();

					xVec.x = - cos(math::toRad(currentDirection)) ;
					xVec.y = 0;
					xVec.z = - sin(math::toRad(currentDirection)) ;
					xVec.Normalize();

					yVec = zVec.crossProduct(xVec);

					refMat.setComponent(xVec, yVec, zVec);

					math::matrix4x4 refTrans;
					refMat.getTransposeMatrix(refTrans);

					posParam->orient.fromMatrix(refTrans);
					m_character->setParams(positioningController, posParam);
					m_character->playController(positioningController, m_slot);
					isPositioning = true;
				}
			}
			else
			{
				if (!isPositioning)
				{
					if (height < fallingThreshold)
					{
						isFalling = true;
						isWalking = false;
						m_character->forceStopController(m_slot);
					}
					else
					{
						if (!isWalking && env->isContacted(m_actor))
						{
							isWalking = true;
							m_character->setParams(gaitController, params);
							m_character->playController(gaitController, m_slot);
						}
					}
				}
				
			}
		}

		core::string CharacterGait::resolveSpeed( float speed )
		{
			if (speed>maxLinearSpeed*(70.0f/100.0f))
			{
				return fastController;
			}
			if (speed>maxLinearSpeed*(30.0f/100.0f))
			{
				return mediumController;
			}
			if (speed>0)
			{
				return slowController;
			}
			return standingController;
		}
	}
}