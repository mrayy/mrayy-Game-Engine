

/********************************************************************
	created:	2013/03/08
	created:	8:3:2013   10:11
	filename: 	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\SprintTouchSensorImpl.h
	file path:	D:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	SprintTouchSensorImpl
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/
#ifndef _SprintTouchSensorImpl_
#define _SprintTouchSensorImpl_

#include "IFeedbackControl.h"
#include "GUISpringTouchElement.h"


namespace mray
{
	namespace game
	{
		class PhysicsComponent;
	}
namespace VT
{
	class IVTDebugObject;
	class GGComponent;

class SpringTouchSensorComponentImpl
{
public:
	SpringTouchSensorComponentImpl()
	{
		channel=0;
	}
	struct SpringContactPoint
	{
		SpringContactPoint()
		{
			lastContact=0;
		}
		bool erased;
		math::vector3d position;
		math::vector3d normal;
		float affect;// this contains the dot product between sensor's normal and contact normal
		float force;
		float time;
		physics::IPhysicalNode* lastContact;
	};
	game::GameEntityManager* mngr;
	physics::IPhysicUserContactReport* reporter;

	core::string targetNodeName;
	core::string targetShapeName;
	core::string modulatorPath;

	game::PhysicsComponent* targetNode;
	physics::IPhysicalShape* shape;

	int channel;
	FeedbackResultF resForce;
	FeedbackResultF modulatedForce;

	GGComponent* ggComponent;
	core::string targetGG;

	SensorCurve* modulator;
	GCPtr<GUI::GUISpringTouchElement> guiElement;

	std::vector<SpringContactPoint> contactPoints;

	IVTDebugObject* debugObj;


	bool AddPoint(const SpringContactPoint& pt);

};

}
}

#endif
