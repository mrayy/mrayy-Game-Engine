
#include "Box3D.h"


namespace mray{
namespace math{

	const box3d box3d::Empty(0,0);

box3d::box3d():MinP(-1,-1,-1),MaxP(1,1,1){}
box3d::box3d(const box3d &b):MinP(b.MinP),MaxP(b.MaxP){}
box3d::box3d(const Point3df &p1,const Point3df  &p2):MinP(p1),MaxP(p2){}
box3d::box3d(const Point3df &p):MinP(p),MaxP(p){}
box3d::box3d(float mnX,float mnY,float mnZ,float mxX,float mxY,float mxZ):MinP(mnX,mnY,mnZ),MaxP(mxX,mxY,mxZ){}

box3d box3d::operator += (const Point3df  &p){MinP+=p;MaxP+=p;return *this;}
box3d box3d::operator -= (const Point3df  &p){MinP-=p;MaxP-=p;return *this;}

box3d box3d::operator += (const box3d  &p){MinP+=p.MinP;MaxP+=p.MaxP;return *this;}
box3d box3d::operator -= (const box3d  &p){MinP-=p.MinP;MaxP-=p.MaxP;return *this;}

box3d box3d::operator + (const Point3df  &p)const{return box3d (MinP+p,MaxP+p);}
box3d box3d::operator - (const Point3df  &p)const{return box3d (MinP-p,MaxP-p);}

box3d box3d::operator + (const box3d  &p)const{return box3d (MinP+p.MinP,MaxP+p.MaxP);}
box3d box3d::operator - (const box3d  &p)const{return box3d (MinP-p.MinP,MaxP-p.MaxP);}

box3d box3d::operator = (const Point3df &p){MinP=MaxP=p;return *this;}

bool box3d::operator == (const box3d  &b){return (MinP==b.MinP && MaxP==b.MaxP);}
bool box3d::operator != (const box3d  &b){return (MinP!=b.MinP || MaxP!=b.MaxP);}

void box3d::AddPoint(const Point3df &p)
{
	AddPoint(p.x,p.y,p.z);
}
void box3d::AddPoint(float _x,float _y,float _z)
{
	if(_x>MaxP.x)MaxP.x=_x;
	if(_y>MaxP.y)MaxP.y=_y;
	if(_z>MaxP.z)MaxP.z=_z;

	if(_x<MinP.x)MinP.x=_x;
	if(_y<MinP.y)MinP.y=_y;
	if(_z<MinP.z)MinP.z=_z;
}

void box3d::AddBox(const box3d &b)
{
	AddPoint(b.MaxP);
	AddPoint(b.MinP);
}
bool box3d::isPointTotalyInSide(const Point3df  &p)const
{
	return(p.x<MaxP.x && p.x>MinP.x)&&
		(p.y<MaxP.y && p.y>MinP.y)&&
		(p.z<MaxP.z && p.z>MinP.z);
}
bool box3d::isPointInSide(const Point3df  &p)const
{
	return(p.x<=MaxP.x && p.x>=MinP.x)&&
		(p.y<=MaxP.y && p.y>=MinP.y)&&
		(p.z<=MaxP.z && p.z>=MinP.z);
}

bool box3d::intersectsWithLine(const Point3df & linemiddle, 
	const Point3df & linevect,
	float halflength) const
{
	const Point3df  e = getExtent() ;
	const Point3df  t = getCenter() - linemiddle;
	float r;

	if ((fabs(t.x) > e.x + halflength * fabs(linevect.x)) || 
		(fabs(t.y) > e.y + halflength * fabs(linevect.y)) ||
		(fabs(t.z) > e.z + halflength * fabs(linevect.z)) )
		return false;

	r = e.y * fabs(linevect.z) + e.z * fabs(linevect.y);
	if (fabs(t.y*linevect.z - t.z*linevect.y) > r )
		return false;

	r = e.x * fabs(linevect.z) + e.z * fabs(linevect.x);
	if (fabs(t.z*linevect.x - t.x*linevect.z) > r )
		return false;

	r = e.x * fabs(linevect.y) + e.y * fabs(linevect.x);
	if (fabs(t.x*linevect.y - t.y*linevect.x) > r)
		return false;
	return true;
}
bool box3d::intersectsWithLine(const line3d& line) const
{/*
	Point3df  v=line.getVector();
	float len=(float)line.Length();
	return intersectsWithLine(line.getMiddle(), v/len, 
		(len * 0.5f));*/


	if (isEmpty())
		return false;


	const vector3d& min = MinP;
	const vector3d& max = MaxP;
	const vector3d& rayorig = line.pStart;

	vector3d raydir(line.getVector());
	vector3d absDir;
	absDir.x = fabs(raydir.x);
	absDir.y = fabs(raydir.y);
	absDir.z = fabs(raydir.z);

	// Sort the axis, ensure check minimise floating error axis first
	int imax = 0, imid = 1, imin = 2;
	if (absDir[0] < absDir[2])
	{
		imax = 2;
		imin = 0;
	}
	if (absDir[1] < absDir[imin])
	{
		imid = imin;
		imin = 1;
	}
	else if (absDir[1] > absDir[imax])
	{
		imid = imax;
		imax = 1;
	}

	float start = 0, end = math::Infinity;

#define _CALC_AXIS(i)                                       \
	do {                                                    \
		float denom = 1 / raydir[i];                         \
		float newstart = (min[i] - rayorig[i]) * denom;      \
		float newend = (max[i] - rayorig[i]) * denom;        \
		if (newstart > newend) Swap(newstart, newend); \
		if (newstart > end || newend < start) return false; \
		if (newstart > start) start = newstart;             \
		if (newend < end) end = newend;                     \
	} while(0)

	// Check each axis in turn

	_CALC_AXIS(imax);

	if (absDir[imid] < math::Epsilon)
	{
		// Parallel with middle and minimise axis, check bounds only
		if (rayorig[imid] < min[imid] || rayorig[imid] > max[imid] ||
			rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
			return false;
	}
	else
	{
		_CALC_AXIS(imid);

		if (absDir[imin] < math::Epsilon)
		{
			// Parallel with minimise axis, check bounds only
			if (rayorig[imin] < min[imin] || rayorig[imin] > max[imin])
				return false;
		}
		else
		{
			_CALC_AXIS(imin);
		}
	}
#undef _CALC_AXIS


// 	if (d1) *d1 = start;
// 	if (d2) *d2 = end;

	return true;
}
bool box3d::intersectWithBox(const box3d  &b)const
{
	return (b.MinP<=MaxP && b.MaxP>=MinP);
}
bool box3d::isInsideBox(const box3d  &b)const
{
	return (MinP>=b.MinP && MaxP<=b.MaxP);
}

Point3df  box3d::getCenter() const
{
	return (MaxP+MinP)*0.5f;
}
Point3df  box3d::getExtent() const
{
	return (MaxP-MinP)*0.5f;
}

bool box3d::isEmpty() const
{
	Point3df  d=(MaxP-MinP);
	if(d.x<0)d.x=-d.x;
	if(d.y<0)d.y=-d.y;
	if(d.z<0)d.z=-d.z;

	return (d.x<math::RoundingValue&&
		d.y<math::RoundingValue&&
		d.z<math::RoundingValue);
}
void box3d::repair()
{
	if(MaxP.x<MinP.x)math::Swap<float> (MaxP.x,MinP.x);
	if(MaxP.y<MinP.y)math::Swap<float> (MaxP.y,MinP.y);
	if(MaxP.z<MinP.z)math::Swap<float> (MaxP.z,MinP.z);
}
box3d  box3d::getInterploted(const box3d  &b,float i)const
{
	float inv=1.0f-i;
	return box3d (b.MinP*inv + MinP*i,b.MaxP*inv + MaxP*i);
}
void box3d::reset(float x,float y,float z)
{
	MaxP.set(x,y,z);
	MinP=MaxP;
}
void box3d::reset(const vector3d&  p)
{
	MaxP=p;
	MinP=MaxP;
}

void box3d::getEdges(Point3df *edges) const
{
	vector3d middle=getCenter();
	vector3d diag=middle-MaxP;
	edges[0].set(middle.x+diag.x,middle.y+diag.y,middle.z+diag.z);
	edges[1].set(middle.x+diag.x,middle.y-diag.y,middle.z+diag.z);
	edges[2].set(middle.x-diag.x,middle.y-diag.y,middle.z+diag.z);
	edges[3].set(middle.x-diag.x,middle.y+diag.y,middle.z+diag.z);
	edges[4].set(middle.x+diag.x,middle.y+diag.y,middle.z-diag.z);
	edges[5].set(middle.x+diag.x,middle.y-diag.y,middle.z-diag.z);
	edges[6].set(middle.x-diag.x,middle.y-diag.y,middle.z-diag.z);
	edges[7].set(middle.x-diag.x,middle.y+diag.y,middle.z-diag.z);
}

float box3d::getWidth() const {return MaxP.x-MinP.x;} 
float box3d::getDepth() const {return MaxP.z-MinP.z;}
float box3d::getHeight()const {return MaxP.y-MinP.y;}



};//math
};//mray
