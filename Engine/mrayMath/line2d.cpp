
#include <math.h>
#include "line2d.h"


namespace mray{
namespace math{

line2d::line2d():pStart(0,0),pEnd(0,0){};
line2d::line2d(const Point2df &p1,const Point2df &p2):pStart(p1),pEnd(p2){};
line2d::line2d(const line2d &l):pStart(l.pStart),pEnd(l.pEnd){};
line2d::line2d(float sX,float sY,float eX,float eY):pStart(sX,sY),pEnd(eX,eY){};

line2d &line2d::operator+=(const Point2df&p){pStart+=p;pEnd+=p;return *this;}
line2d &line2d::operator-=(const Point2df&p){pStart-=p;pEnd-=p;return *this;}

line2d  line2d::operator+(const Point2df&p)const{return line2d(pStart+p,pEnd+p);}
line2d  line2d::operator-(const Point2df&p)const{return line2d(pStart-p,pEnd-p);}

bool line2d::operator ==(const line2d&l)const{return pStart==l.pStart && pEnd==l.pEnd;}
bool line2d::operator !=(const line2d&l)const{return pStart!=l.pStart || pEnd!=l.pEnd;}

double line2d::Length()
{
	return pStart.getDist(pEnd);
}
double line2d::LengthSQ()
{
	return pStart.getDistSQ(pEnd);
}

Point2df line2d::getVector()
{
	return (pEnd-pStart);
}

bool line2d::IntersctWithLine(const line2d&l,Point2df &out)const
{

	float Ay_minus_Cy = pStart.y - l.pStart.y;	
	float Dx_minus_Cx = l.pEnd.x - l.pStart.x;	
	float Ax_minus_Cx = pStart.x - l.pStart.x;	
	float Dy_minus_Cy = l.pEnd.y - l.pStart.y;	
	float Bx_minus_Ax = pEnd.x - pStart.x;	
	float By_minus_Ay = pEnd.y - pStart.y;	

	float Numerator = (Ay_minus_Cy * Dx_minus_Cx) - (Ax_minus_Cx * Dy_minus_Cy);
	float Denominator = (Bx_minus_Ax * Dy_minus_Cy) - (By_minus_Ay * Dx_minus_Cx);

	// if lines do not intersect, return now
	if (!Denominator)
	{
		if (!Numerator)
		{
			return false;
		}

		return false;
	}

	float FactorAB = Numerator / Denominator;
	float FactorCD = ((Ay_minus_Cy * Bx_minus_Ax) - (Ax_minus_Cx * By_minus_Ay)) / Denominator;

	// posting (hitting a vertex exactly) is not allowed, shift the results
	// if they are within a minute range of the end vertecies
	/*	if (fabs(FactorCD) < 1.0e-6f)
	{
	FactorCD = 1.0e-6f;
	}
	if (fabs(FactorCD - 1.0f) < 1.0e-6f)
	{
	FactorCD = 1.0f - 1.0e-6f;
	}
	*/

	// if an interection point was provided, fill it in now
	
	out.x = (pStart.x + (FactorAB * Bx_minus_Ax));
	out.y = (pStart.y + (FactorAB * By_minus_Ay));

	// now determine the type of intersection
	if ((FactorAB >= 0.0f) && (FactorAB <= 1.0f) && (FactorCD >= 0.0f) && (FactorCD <= 1.0f))
	{
		// SEGMENTS_INTERSECT;
		return true;
	}
	return false;
	/*
	else if ((FactorCD >= 0.0f) && (FactorCD <= 1.0f))
	{
		 //(A_BISECTS_B);
		return true;
	}
	else if ((FactorAB >= 0.0f) && (FactorAB <= 1.0f))
	{
		//(B_BISECTS_A);
		return true;
	}
	return false;
*/
	/*
	float a1,a2,m1,m2;
	if(pStart.x==pEnd.x)
		m1=(float)1e+10;
	else
		m1=(pEnd.y-pStart.y)/(pEnd.x-pStart.x);

	if(l.pStart.x==l.pEnd.x)
		m2=(float)1e+10;
	else
		m2=(l.pEnd.y-l.pStart.y)/(l.pEnd.x-l.pStart.x);

	a1=pStart.y - m1*pStart.x;
	a2=l.pStart.y - m2*l.pStart.x;

	out.x=-(a1-a2)/(m1-m2);
	out.y=a1+ m1*out.x;
	if( (pStart.x  -out.x)*(out.x-pEnd.x  )>=0 &&
		(l.pStart.x-out.x)*(out.x-l.pEnd.x)>=0 &&
		(pStart.y  -out.y)*(out.y-pEnd.y  )>=0 &&
		(l.pStart.y-out.y)*(out.y-l.pEnd.y)>=0 )
	{
		return 1;
	}
	return 0;*/
}

Point2df line2d::UnitVector()
{
	return (pEnd-pStart).Normalize();
}

Point2df line2d::getMiddle()
{
	return (pStart + pEnd)* 0.5f;
}


};//math
};//mray

