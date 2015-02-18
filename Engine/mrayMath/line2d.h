
#ifndef ___LINE2D___
#define ___LINE2D___

#include "compileConfig.h"
#include "Point2d.h"

namespace mray{
namespace math{

class MRAY_MATH_DLL line2d
{
public:
	Point2df pStart;
	Point2df pEnd;

	line2d();
	line2d(const Point2df &p1,const Point2df &p2);
	line2d(const line2d &l);
	line2d(float sX,float sY,float eX,float eY);

	line2d &operator+=(const Point2df&p);
	line2d &operator-=(const Point2df&p);

	line2d  operator+(const Point2df&p)const;
	line2d  operator-(const Point2df&p)const;

	bool operator ==(const line2d&l)const;
	bool operator !=(const line2d&l)const;

	double Length();
	double LengthSQ();

	Point2df getVector();

	bool IntersctWithLine(const line2d&l,Point2df &out)const;

	Point2df UnitVector();
	Point2df getMiddle();
};


};//math
};//mray


#endif