

#ifndef ___BOX3D___
#define ___BOX3D___

#include "Point3d.h"
#include "line3d.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL box3d
{
public:
	Point3df MinP;
	Point3df MaxP;

	box3d();
	box3d(const box3d &b);
	box3d(const Point3df &p1,const Point3df  &p2);
	box3d(const Point3df &p);
	box3d(float mnX,float mnY,float mnZ,float mxX,float mxY,float mxZ);

	box3d operator += (const Point3df  &p);
	box3d operator -= (const Point3df  &p);
	
	box3d operator += (const box3d  &p);
	box3d operator -= (const box3d  &p);

	box3d operator + (const Point3df  &p)const;
	box3d operator - (const Point3df  &p)const;
	
	box3d operator + (const box3d  &p)const;
	box3d operator - (const box3d  &p)const;

	box3d operator = (const Point3df &p);

	bool operator == (const box3d  &b);
	bool operator != (const box3d  &b);

	void AddPoint(const Point3df &p);
	void AddPoint(float _x,float _y,float _z);

	void AddBox(const box3d &b);
	bool isPointTotalyInSide(const Point3df  &p)const;
	bool isPointInSide(const Point3df  &p)const;

	bool intersectsWithLine(const Point3df & linemiddle, 
				const Point3df & linevect,
				float halflength) const;
	bool intersectsWithLine(const line3d& line) const;
	bool intersectWithBox(const box3d  &b)const;
	bool isInsideBox(const box3d  &b)const;

	Point3df  getCenter() const;
	Point3df  getExtent() const;

	bool isEmpty() const;
	void repair();
	box3d  getInterploted(const box3d  &b,float i)const;
	void reset(float x,float y,float z);
	void reset(const Point3df &p);

	void getEdges(Point3df *edges) const;

	float getWidth() const ;
	float getDepth() const ;
	float getHeight()const ;

	static const box3d Empty;
};


};//math
};//mray

#endif