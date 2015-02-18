
/********************************************************************
	created:	2010/03/17
	created:	17:3:2010   15:01
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\VisionSensor.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	VisionSensor
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Detect Surrounding AI Elements that's an agent can see
*********************************************************************/

#ifndef ___VisionSensor___
#define ___VisionSensor___

#include "IPerceptionSensor.h"
#include "compileConfig.h"
#include "AIActor.h"
#include "IActorQuery.h"

namespace mray{
namespace AI{
	class IRenderDevice;
	class ActorMemory;

class MRAY_AI_DLL VisionSensor:public IPerceptionSensor,public IActorQuery
{
	DECLARE_RTTI
private:
protected:
	AIActorList m_visibleActors;
	ObstacleList m_ActorObstacles;

	float m_fov;		//angle of view that's an agent can see
	float m_viewRange;	//maximum distance that's an agent can see
	float m_cosFov;	//cos(fov)

	AIActor* m_owner;
	ActorMemory* m_memory;


	DECLARE_PROPERTY_TYPE(FoV,float,MRAY_AI_DLL);
	DECLARE_PROPERTY_TYPE(ViewRange,float,MRAY_AI_DLL);
public:

/*	rwProperty<VisionSensor,float> FoV;
	rwProperty<VisionSensor,float> ViewRange;*/

	VisionSensor(AIActor*owner,ActorMemory*mem);
	virtual~VisionSensor();


	virtual void Update(float dt);

	IActorQuery* GetActorQuery();

	bool SetFOV(const float& f);
	const float& GetFOV();

	bool SetViewRange(const float& f);
	const float& GetViewRange();

	virtual AIActor* GetOwnerAgent();

	const AIActorList & GetActorsList();
	const ObstacleList & GetObstaclesList();

	virtual void Visualize(IRenderDevice*dev);
};

}
}


#endif //___VisionSensor___

