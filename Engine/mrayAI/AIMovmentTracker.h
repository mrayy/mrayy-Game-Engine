


/********************************************************************
	created:	2010/05/05
	created:	5:5:2010   8:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIMovmentTracker.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIMovmentTracker
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Draws agent movements
*********************************************************************/

#ifndef AIMovmentTracker_h__
#define AIMovmentTracker_h__

#include "IAIComponent.h"

namespace mray
{
namespace AI
{

	class IAIPhysics;

class MRAY_AI_DLL AIMovmentTracker:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
	IAIPhysics* m_physics;

	int m_maxSamples;
	std::list<math::vector3d> m_samples;
public:
	AIMovmentTracker(IAIPhysics*ph);
	virtual ~AIMovmentTracker();

	virtual void Update(float dt);


	virtual void Visualize(IRenderDevice*dev);
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // AIMovmentTracker_h__
