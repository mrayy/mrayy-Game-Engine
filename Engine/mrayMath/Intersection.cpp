
#include "Intersection.h"



namespace mray{
namespace math{

EInterResult Intersection::intersect(const box3d&b1,const box3d&b2){
	if( (b2.MinP.x<b1.MinP.x) && (b2.MaxP.x>b1.MaxP.x) &&
		(b2.MinP.y<b1.MinP.y) && (b2.MaxP.y>b1.MaxP.y) &&
		(b2.MinP.z<b1.MinP.z) && (b2.MaxP.z>b1.MaxP.z))
		return EIR_Contained;

	if( (b1.MinP.x<b2.MinP.x) && (b1.MaxP.x>b2.MaxP.x) &&
		(b1.MinP.y<b2.MinP.y) && (b1.MaxP.y>b2.MaxP.y) &&
		(b1.MinP.z<b2.MinP.z) && (b1.MaxP.z>b2.MaxP.z))
		return EIR_Contains;

	if( (b1.MinP.x>b2.MaxP.x) || (b2.MinP.x>b1.MaxP.x) ||
		(b1.MinP.y>b2.MaxP.y) || (b2.MinP.y>b1.MaxP.y) ||
		(b1.MinP.z>b2.MaxP.z) || (b2.MinP.z>b1.MaxP.z))
		return EIR_Partial;

	return EIR_None;
}

bool Intersection::intersect(const Sphere&s,const box3d&b){
	if(s.center.x<b.MinP.x && (b.MinP.x-s.center.x) > s.radius)
		return false;
	if(s.center.y<b.MinP.y && (b.MinP.y-s.center.y) > s.radius)
		return false;
	if(s.center.z<b.MinP.z && (b.MinP.z-s.center.z) > s.radius)
		return false;

	if(s.center.x>b.MaxP.x && (b.MaxP.x-s.center.x) > s.radius)
		return false;
	if(s.center.y>b.MaxP.y && (b.MaxP.y-s.center.y) > s.radius)
		return false;
	if(s.center.z>b.MaxP.z && (b.MaxP.z-s.center.z) > s.radius)
		return false;

	return true;
}

InterRayResult Intersection::intersect(const Ray3d&r,const box3d&b){
	InterRayResult res(0);

	if((r.Start>=b.MinP) && (r.Start<=b.MaxP)){
		res.hit=true;
		return res;
	}

	float n;
	vector3d v;

	if(r.Start.x < b.MinP.x && r.Dir.x>0){
		n= ( b.MinP.x - r.Start.x)/r.Dir.x;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.y>=b.MinP.y && v.y<=b.MinP.y && v.z>=b.MinP.z && v.z<=b.MinP.z){
				res.distance=n;
				res.hit=true;
			}
		}
	}

	if(r.Start.x > b.MaxP.x && r.Dir.x>0){
		n= ( b.MaxP.x - r.Start.x)/r.Dir.x;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.y>=b.MinP.y && v.y<=b.MinP.y && v.z>=b.MinP.z && v.z<=b.MinP.z && (!res.hit || res.distance>n)){
				res.distance=n;
				res.hit=true;
			}
		}
	}

	if(r.Start.y < b.MinP.y && r.Dir.y>0){
		n= ( b.MinP.y - r.Start.y)/r.Dir.y;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.x>=b.MinP.x && v.x<=b.MinP.x && v.z>=b.MinP.z && v.z<=b.MinP.z && (!res.hit || res.distance>n)){
				res.distance=n;
				res.hit=true;
			}
		}
	}
	if(r.Start.y > b.MaxP.y && r.Dir.y>0){
		n= ( b.MaxP.y - r.Start.y)/r.Dir.y;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.x>=b.MinP.x && v.x<=b.MinP.x && v.z>=b.MinP.z && v.z<=b.MinP.z && (!res.hit || res.distance>n)){
				res.distance=n;
				res.hit=true;
			}
		}
	}

	if(r.Start.z < b.MinP.z && r.Dir.z>0){
		n= ( b.MinP.z - r.Start.z)/r.Dir.z;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.x>=b.MinP.x && v.x<=b.MinP.x && v.y>=b.MinP.y && v.y<=b.MinP.y && (!res.hit || res.distance>n)){
				res.distance=n;
				res.hit=true;
			}
		}
	}
	if(r.Start.z > b.MaxP.z && r.Dir.z>0){
		n= ( b.MaxP.z - r.Start.z)/r.Dir.z;
		if(n>0)
		{
			v=r.Start + r.Dir*n;
			if(v.x>=b.MinP.x && v.x<=b.MinP.x && v.y>=b.MinP.y && v.y<=b.MinP.y && (!res.hit || res.distance>n)){
				res.distance=n;
				res.hit=true;
			}
		}
	}
	return res;
}

bool Intersection::intersect(const Plane&p,const box3d&b){
	vector3d cor[8];
	b.getEdges(cor);
	
	int loc= p.getVertLocation(cor[0]);
	for(int i=1;i<8;i++){
		if(p.getVertLocation(cor[i])!=loc){
			return true;
		}
	}
	return false;
}


InterRayResult Intersection::intersect(const Ray3d&r,const Sphere&s){
	
	vector3d d=r.Start - s.center;
	if(d.LengthSQ() <= s.radius*s.radius){
		return InterRayResult(true,false);
	}
	float n2=r.Dir.dotProduct(r.Dir);
	float n3=d.dotProduct(r.Dir)*2.0f;
	float n4=d.dotProduct(d) - s.radius*s.radius;

	float delta=(n3*n3) - 4*n2*n4;
	if(delta<0)
		return InterRayResult(0);

	float sd=sqrt(delta);

	float dist=(-n3 - sd)/2*n2;
	if(dist<0){
		dist=(-n3+ sd)/2*n2;
	}
	return InterRayResult(true,dist);

}

bool Intersection::intersect(const Plane&p,const Sphere&s){
	return (p.getDistance(s.center)) <=s.radius;
}


InterRayResult Intersection::intersect(const Ray3d&r,const Plane&p){

	float n2 = (p.Normal.x * r.Dir.x) + (p.Normal.y * r.Dir.y) + (p.Normal.z * r.Dir.z);
	if (abs(n2) < math::Epsilon)
	{
		return InterRayResult(0);
	}
	float n3 = (p.Normal.x * r.Start.x) + (p.Normal.y * r.Start.y) + (p.Normal.z * r.Start.z);
	float n = (-p.D - n3) / n2;
	if (n < 0)
	{
		if (n > -math::Epsilon)
			n=0;
	}
	if(n>=0){
		return InterRayResult(1,n);
	}
	return InterRayResult(0);

}



}
}

