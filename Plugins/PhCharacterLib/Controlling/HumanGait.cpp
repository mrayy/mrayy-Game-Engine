#include "stdafx.h"
#include "HumanGait.h"
#include "../Character.h"
#include "../PhysicalModel/Segment.h"
#include <cmath>
#include "CharacterManager.h"

namespace mray
{
	namespace PhysicsBasedCharacter
	{
		HumanGait::HumanGait()
		{
			m_root = 0;
			m_leftHip = 0;
			m_rightHip = 0;
			m_leftAnkle = 0;
			m_rightAnkle = 0;

			m_balanceParams = 0;
			m_controllingParams = new GaitParams();
			m_controllingParams->facingDirection = 90;
			m_controllingParams->turningSpeed = 90;
			flag = true;

		}
		HumanGait::~HumanGait()
		{
			delete m_balanceParams;
			delete m_controllingParams;
		}
		void HumanGait::load( xml::XMLElement*elem, Character* character )
		{
			m_root = character->getSegment(elem->getAttribute(mT("root"))->value);
			m_leftHip = character->getSegment(elem->getAttribute(mT("leftHip"))->value);
			m_rightHip = character->getSegment(elem->getAttribute(mT("rightHip"))->value);
			m_leftAnkle = character->getSegment(elem->getAttribute(mT("leftAnkle"))->value);
			m_rightAnkle = character->getSegment(elem->getAttribute(mT("rightAnkle"))->value);
			setBalanceParams(createBalanceParams(elem->getSubElement(mT("Params"))));
			m_environement = character->GetCreator()->GetEnvironment();
		}

		BalanceParams* HumanGait::createBalanceParams( xml::XMLElement*elem )
		{
			if (elem == 0)
			{
				return 0;
			}
			HumanGait_BalanceParams* params = new HumanGait_BalanceParams();
			params->cd = core::StringConverter::toFloat(elem->getAttribute(mT("cd"))->value);
			params->cv = core::StringConverter::toFloat(elem->getAttribute(mT("cv"))->value);
			params->leftStance = core::StringConverter::toBool(elem->getAttribute(mT("leftStance"))->value);
			return params;
		}

		void HumanGait::setBalanceParams( BalanceParams* params )
		{
			m_balanceParams = (static_cast<HumanGait_BalanceParams*>(params));
		}

		void HumanGait::setControllingParams( ControllingParams* params )
		{
			m_controllingParams = (static_cast<GaitParams*>(params));
		}

		void HumanGait::modifyTargetPose(float dt)
		{

			math::quaternion q = m_root->getGlobalOrientation();
			math::vector3d dir = (q * math::vector3d.XAxis).invert();
			float currentDirection = math::toDeg(atan2(dir.z,dir.x));
			float diff = m_controllingParams->facingDirection - currentDirection;
			if (diff > 180)
			{
				diff -= 360;
			}
			if (diff < -180)
			{
				diff += 360;
			}

			float modif = m_controllingParams->turningSpeed * dt;

			if (!flag && abs(diff)>2*modif)
			{
				if (diff > 0)
				{
					currentDirection += modif;
				}
				else
				{
					currentDirection -= modif;
				}
			}

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
			math::quaternion refQ;
			refQ.fromMatrix(refTrans);

			Segment* swingHip;
			Segment* stanceHip;
			Segment* stanceAnkle;
			if (m_balanceParams->leftStance)
			{
				stanceAnkle = m_leftAnkle;
				swingHip = m_rightHip;
				stanceHip = m_leftHip;
			}
			else
			{
				stanceAnkle = m_rightAnkle;
				swingHip = m_leftHip;
				stanceHip = m_rightHip;
			}

			math::vector3d d = stanceAnkle->getGlobalTranslation() - m_root->getGlobalTranslation();
			math::vector3d v = m_root->getGlobalLinearVelocity();

			d = refQ.inverse() * d;
			v = refQ.inverse() * v;

			//d = m_root->getGlobalOrientation().inverse() * d;
			//v = m_root->getGlobalOrientation().inverse() * v;

			flag = abs(v.y)>m_balanceParams->cv||v.x>m_balanceParams->cv;

			math::vector3d groundNormal = refQ.inverse() * m_environement->getGroundNormal(m_root->getGlobalTranslation());
			float dy = math::vector3d(groundNormal.x,0,groundNormal.z).getAngleBetween(math::vector3d(0,0,-1));
			//if (groundNormal.x<0)
			{
				dy = -1 * dy;
			}
			float dx = math::vector3d(0,groundNormal.y,groundNormal.z).getAngleBetween(math::vector3d(0,0,-1));
			//if (groundNormal.y>0)
			//{
			//	dx = -1 * dx;
			//}

			math::vector3d delta;
			delta.set(math::toDeg((d.y * m_balanceParams->cd) - (v.y * (m_balanceParams->cv + 1*dy))),math::toDeg(-(d.x * m_balanceParams->cd) + (v.x * (m_balanceParams->cv + 1*dx))),0);

			swingHip->target  += delta;
			
			m_root->startOrient = refQ;

		}


	}
}