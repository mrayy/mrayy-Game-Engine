


/********************************************************************
	created:	2011/01/21
	created:	21:1:2011   13:39
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayMath\ExtrudedPolygon.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayMath
	file base:	ExtrudedPolygon
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ExtrudedPolygon_h__
#define ExtrudedPolygon_h__

#include "Polygon2D.h"
#include "box3d.h"

namespace mray
{
namespace math
{

class ExtrudedPolygon:public Polygon2D
{
private:
protected:
	float m_height;
	float m_min;
	box3d m_boundingBox;
public:
	ExtrudedPolygon();
	virtual ~ExtrudedPolygon();

	void setHeight(float h);
	void setMin(float m);
	void addPoint(const math::vector3d &p);

	const math::box3d& getBoundingBox()const;

	bool isPointInside(const math::vector3d&p)const;
	
};

}
}


#endif // ExtrudedPolygon_h__
