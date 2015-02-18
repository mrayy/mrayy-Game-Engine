

/********************************************************************
	created:	2010/04/18
	created:	18:4:2010   7:50
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\LookAheadDOS.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	LookAheadDOS
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	look ahead for the nearest obstacle that might the agent collide with
*********************************************************************/

#ifndef LookAheadDOS_h__
#define LookAheadDOS_h__


#include "compileConfig.h"
#include "IDynamicObstacleSolver.h"

namespace mray
{
namespace AI
{

	class IActorQuery;

class MRAY_AI_DLL LookAheadDOS:public IDynamicObstacleSolver
{
	DECLARE_RTTI
private:
protected:
	AgentObstacle* m_agent;
	float m_detectionDistance;
	float m_brakingStrength;
	float m_avoidanceStrength;
	float m_extendRad;
	IActorQuery* m_query;


	DECLARE_PROPERTY_TYPE(DetectionDistance,float,MRAY_AI_DLL);
	DECLARE_PROPERTY_TYPE(AvoidanceStrength,float,MRAY_AI_DLL);
	DECLARE_PROPERTY_TYPE(BrakingStrength,float,MRAY_AI_DLL);
public:
	LookAheadDOS(AgentObstacle* agent,IActorQuery* query,float detectionDistance);
	virtual ~LookAheadDOS();

	const float& GetDetectionDistance()const{return m_detectionDistance;}
	bool SetDetectionDistance(float r){m_detectionDistance=r;return true;}

	const float& GetBrakingStrength()const{return m_brakingStrength;}
	bool SetBrakingStrength(float r){m_brakingStrength=r;return true;}

	const float& GetAvoidanceStrength()const{return m_avoidanceStrength;}
	bool SetAvoidanceStrength(float r){m_avoidanceStrength=r;return true;}

	virtual void CalculateAvoidanceForce();
	virtual void Visualize(IRenderDevice*dev);
};

}
}

#endif // LookAheadDOS_h__
