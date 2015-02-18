
/********************************************************************
	created:	2009/02/01
	created:	1:2:2009   19:32
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\Polygon2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	Polygon2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___Polygon2D___
#define ___Polygon2D___

#include "Point2d.h"
#include "rect.h"
//#include "mArray.h"
#include <vector>

namespace mray{
namespace math{

class MRAY_MATH_DLL Polygon2D
{
protected:
	std::vector<math::vector2d> m_points;
	math::rectf m_boundingRect;
public:

	Polygon2D();
	virtual~Polygon2D();

	void addPoint(const math::vector2d &p);
	void removePoint(size_t i);
	void clear();

	const math::vector2d& getPoint(size_t i)const;
	size_t getPointsCount()const;

	const math::rectf& getBoundingRect()const;

	bool isPointInside(const math::vector2d&p)const;
};

}
}


#endif //___Polygon2D___
