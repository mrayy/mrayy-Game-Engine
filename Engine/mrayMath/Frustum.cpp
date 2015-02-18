


#include "Frustum.h"

#include "MathUtil.h"




namespace mray{
namespace math{

Frustum::Frustum()
{
}
Frustum::Frustum(const math::matrix4x4&projMat)
{
	set(projMat);
}
void Frustum::set(const math::matrix4x4&projMat)
{

	planes[FP_LEFT_PLANE].Normal.x	= (projMat[3][0]+projMat[0][0]);
	planes[FP_LEFT_PLANE].Normal.y	= (projMat[3][1]+projMat[0][1]);
	planes[FP_LEFT_PLANE].Normal.z	= (projMat[3][2]+projMat[0][2]);
	planes[FP_LEFT_PLANE].D			= (projMat[3][3]+projMat[0][3]);
	
	planes[FP_RIGHT_PLANE].Normal.x	= (projMat[3][0]-projMat[0][0]);
	planes[FP_RIGHT_PLANE].Normal.y	= (projMat[3][1]-projMat[0][1]);
	planes[FP_RIGHT_PLANE].Normal.z	= (projMat[3][2]-projMat[0][2]);
	planes[FP_RIGHT_PLANE].D		= (projMat[3][3]-projMat[0][3]);
	
	planes[FP_TOP_PLANE].Normal.x	= (projMat[3][0]-projMat[1][0]);
	planes[FP_TOP_PLANE].Normal.y	= (projMat[3][1]-projMat[1][1]);
	planes[FP_TOP_PLANE].Normal.z	= (projMat[3][2]-projMat[1][2]);
	planes[FP_TOP_PLANE].D			= (projMat[3][3]-projMat[1][3]);
	
	planes[FP_BOTTOM_PLANE].Normal.x= (projMat[3][0]+projMat[1][0]);
	planes[FP_BOTTOM_PLANE].Normal.y= (projMat[3][1]+projMat[1][1]);
	planes[FP_BOTTOM_PLANE].Normal.z= (projMat[3][2]+projMat[1][2]);
	planes[FP_BOTTOM_PLANE].D		= (projMat[3][3]+projMat[1][3]);
	
	planes[FP_NEAR_PLANE].Normal.x	= projMat[3][0]+projMat[2][0];
	planes[FP_NEAR_PLANE].Normal.y	= projMat[3][1]+projMat[2][1];
	planes[FP_NEAR_PLANE].Normal.z	= projMat[3][2]+projMat[2][2];
	planes[FP_NEAR_PLANE].D			= projMat[3][3]+projMat[2][3];
	
	planes[FP_FAR_PLANE].Normal.x	= (projMat[3][0]-projMat[2][0]);
	planes[FP_FAR_PLANE].Normal.y	= (projMat[3][1]-projMat[2][1]);
	planes[FP_FAR_PLANE].Normal.z	= (projMat[3][2]-projMat[2][2]);
	planes[FP_FAR_PLANE].D			= (projMat[3][3]-projMat[2][3]);
	// normalize normals

	for (int i=0; i < FP_PLANE_COUNT; ++i)
	{
		const float len =  1.0f/(float)planes[i].Normal.Length();
		planes[i].Normal *= len;
		planes[i].D *= len;
	}

	recalcBoundingBox();
}

math::vector3d Frustum::getFarLUp()const
{
	math::vector3d vec;
	planes[FP_FAR_PLANE].getIntersectionWithPlanes(
		planes[FP_TOP_PLANE],planes[FP_LEFT_PLANE],
		vec);
	return vec;
}
math::vector3d Frustum::getFarRUp()const
{
	math::vector3d vec;
	planes[FP_FAR_PLANE].getIntersectionWithPlanes(
		planes[FP_TOP_PLANE],planes[FP_RIGHT_PLANE],
		vec);
	return vec;
}
math::vector3d Frustum::getFarLDown()const
{
	math::vector3d vec;
	planes[FP_FAR_PLANE].getIntersectionWithPlanes(
		planes[FP_BOTTOM_PLANE],planes[FP_LEFT_PLANE],
		vec);
	return vec;
}
math::vector3d Frustum::getFarRDown()const
{
	math::vector3d vec;
	planes[FP_FAR_PLANE].getIntersectionWithPlanes(
		planes[FP_BOTTOM_PLANE],planes[FP_RIGHT_PLANE],
		vec);
	return vec;
}

//void Frustum::transform(math::matrix4x4 &mat)

bool Frustum::pointInFrustrum(const math::vector3d& p)const
{
	//return boundingBox.isPointInSide(p);
	for(int j=0;j<6;j++){
		if(planes[j].getVertLocation(p)<=0){
			return false;
		}
	}
	return true;
}
bool Frustum::boxInFrustrum(const math::vector3d& p1,const math::vector3d& p2)const
{
	return boxInFrustrum(math::box3d(p1,p2));
}
bool Frustum::boxInFrustrum(const math::box3d& b)const
{
	/**/
	math::vector3d edges[8];
	b.getEdges(edges);
	for(int i = 0; i < 6; i++ )
	{
		int out=0,in=0;
		for(int j=0;j<8;j++){
			if(planes[i].getVertLocation(edges[j])<=0){
				out++;
			}else in++;
		}

		if(!in)return false;
	}
	return true;
	math::vector3d center=b.getCenter();
	math::vector3d ext=b.getExtent();
	for(int j=0;j<6;j++){
		if(planes[j].getVertLocation(center,ext)<=0){
			return false;
		}
	}
	return true;
}
bool Frustum::sphereInFrustrum(const math::vector3d& p,float radius)const
{
	for(int i=0;i<6;++i){
		if(planes[i].getDistance(p)<=-radius)
			return 0;
	}
	return 1;
}

void Frustum::recalcBoundingBox()
{
	math::box3d box(viewPos);
	box.AddPoint(getFarLUp());
	box.AddPoint(getFarRUp());
	box.AddPoint(getFarLDown());
	box.AddPoint(getFarRDown());
	box.MaxP+=3;
	box.MinP-=3;
	boundingBox=box;
}

}
}


