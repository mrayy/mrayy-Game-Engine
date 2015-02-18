


/********************************************************************
	created:	2010/05/04
	created:	4:5:2010   19:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IAIObstacleShape.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IAIObstacleShape
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IAIObstacleShape_h__
#define IAIObstacleShape_h__

#include "Point2d.h"
#include "line2d.h"
#include "marray.h"

namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace AI
{

class IAIObstacleShape
{
private:
protected:
public:
	IAIObstacleShape(){}
	virtual ~IAIObstacleShape(){}

	//line should be in local space 
	virtual bool IntersectWithLine(const math::line2d& l,math::vector2d*interPoint)=0;

	//return array contain surrounding convex shape of the obstacle
	virtual void GetShapeConvexPoints(std::vector<math::vector2d>& points)=0;

	virtual void loadXMLSettings(xml::XMLElement* elem)=0;
};

}
}

#endif // IAIObstacleShape_h__
