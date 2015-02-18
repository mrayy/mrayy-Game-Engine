

/********************************************************************
	created:	2009/01/31
	created:	31:1:2009   21:38
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\SteerController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	SteerController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SteerController___
#define ___SteerController___

#include "compileConfig.h"
#include "Point3d.h"
#include "AIActor.h"
#include "IAIComponent.h"

namespace mray{
namespace AI{


enum ESteeringTypes
{
	EST_Seek,
	EST_Flee,
	EST_Arrive,
	EST_Pursuit,
	EST_Evade,
	EST_Hide,
	EST_Wander,
	EST_Seperation,
	EST_Align,
	EST_Cohesion,
	EST_Count
};


class MRAY_AI_DLL SteerController:public IAIComponent
{
DECLARE_RTTI
protected:
/*
	struct SteeringFlag{
		SteeringFlag(){
			data=0;
			inUse=0;
		}
		template<class T>
		T&d(){
			return *static_cast<T*>(data);
		}
		template<class T>
		T*ptr(){
			return static_cast<T*>(data);
		}
		void*data;
		bool inUse;
	};*/

	IAIPhysics* m_physics;

	math::vector3d m_targetV;
	AIActor* m_targetActor;

	math::vector3d m_steeringForce;

	float m_weights[EST_Count];
	bool m_steeringFlags[EST_Count];

	float m_wanderJitter;
	float m_wanderRadius;
	float m_wanderDistance;
	math::vector3d m_wanderTarget;



	float turnAroundTime(const math::vector3d&p);

	math::vector3d seek(const math::vector3d&target);
	math::vector3d flee(const math::vector3d&target);
	math::vector3d arrive(const math::vector3d&target,float deceleraion=1);
	math::quaternion align(AIActorList& target);
	math::vector3d cohesion(AIActorList& target);
	math::vector3d pursuit(AIActor*o);
	math::vector3d evade(AIActor*o);
	math::vector3d wander();

	// returns a steering force that moves a vehicle to 
	// the midpoint of the imaginary line connecting two other agents 
	math::vector3d interpose(AIActor*a,AIActor*b);

	// givent center and radius of the sphere , get the best place to hide from target
	math::vector3d getHidePosition(const math::vector3d&center,float radius,
		const math::vector3d&target);

	math::vector3d hide(AIActor*target,ObstacleList&obstacles);

	math::vector3d followPath();

	math::vector3d offsetPursuit(AIActor*target,const math::vector3d& offset);


	bool accumulateForce(math::vector3d&target,const math::vector3d&force);
public:

	SteerController(IAIPhysics* ph);
	virtual~SteerController();

	IAIPhysics* getPhysics(){return m_physics;}

	bool isSteering(ESteeringTypes t);

	void seekOn(AIActor*agent);
	void seekOn(const math::vector3d&pos);
	void seekOff();

	void fleeOn(AIActor*agent);
	void fleeOn(const math::vector3d&pos);
	void fleeOff();

	void pursuitOn(AIActor*agent);
	void pursuitOn(const math::vector3d&pos);
	void pursuitOff();

	void evadeOn(AIActor *agent);
	void evadeOn(const math::vector3d&pos);
	void evadeOff();

	void hideOn(AIActor *agent);
	void hideOn(const math::vector3d&pos);
	void hideOff();

	void arriveOn(const math::vector3d&pos);
	void arriveOff();

	void wanderOn();
	void wanderOff();

	void seperationOn();
	void seperationOff();

	void alignOn();
	void alignOff();

	void cohesionOn();
	void cohesionOff();

	math::vector3d GetTargetPos();

	math::vector3d calculate();

	math::vector3d headComponent();
	//math::vector3d sideComponent();
	math::vector3d upComponent();

	void Visualize(IRenderDevice*dev);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}


#endif //___SteerController___
