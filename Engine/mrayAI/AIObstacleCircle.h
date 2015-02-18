

/********************************************************************
	created:	2010/05/04
	created:	4:5:2010   19:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIObstacleCircle.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIObstacleCircle
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIObstacleCircle_h__
#define AIObstacleCircle_h__

#include "CompileConfig.h"
#include "IAIObstacleShape.h"

namespace mray
{
namespace AI
{


class MRAY_AI_DLL AIObstacleCircle:public IAIObstacleShape
{
private:
protected:
	float m_radius;
public:
	AIObstacleCircle(float R);
	virtual ~AIObstacleCircle();

	void SetRadius(float r);
	float GetRadius();

	bool IntersectWithLine(const math::line2d& l,math::vector2d*interPoint);

	void GetShapeConvexPoints(std::vector<math::vector2d>& points);

	virtual void loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // AIObstacleCircle_h__
