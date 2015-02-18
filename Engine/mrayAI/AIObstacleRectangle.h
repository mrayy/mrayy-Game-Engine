

/********************************************************************
	created:	2010/05/04
	created:	4:5:2010   20:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIObstacleRectangle.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIObstacleRectangle
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIObstacleRectangle_h__
#define AIObstacleRectangle_h__

#include "CompileConfig.h"
#include "IAIObstacleShape.h"

namespace mray
{
namespace AI
{

class MRAY_AI_DLL AIObstacleRectangle:public IAIObstacleShape
{
private:
protected:
	float m_width;
	float m_height;
public:
	AIObstacleRectangle(float w,float h);
	virtual ~AIObstacleRectangle();

	void SetWidth(float v);
	float GetWidth();

	void SetHeight(float v);
	float GetHeight();

	bool IntersectWithLine(const math::line2d& l,math::vector2d*interPoint);

	void GetShapeConvexPoints(std::vector<math::vector2d>& points);

	virtual void loadXMLSettings(xml::XMLElement* elem);
};

}
}

#endif // AIObstacleRectangle_h__

