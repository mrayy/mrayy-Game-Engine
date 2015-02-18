#include "stdafx.h"



#include "SceneHelper.h"
#include "IVideoDevice.h"

#include "ISceneNode.h"
#include "CameraNode.h"

namespace mray{
namespace scene{


SceneHelper::SceneHelper()
{
	selector=new CTriangleSelector();
}
SceneHelper::~SceneHelper()
{
	delete selector;
}


inline bool SceneHelper::getLowestRoot(float a, float b, float c, float maxR, float* root)
{
	// check if solution exists
	float det = b*b - 4.0f*a*c;

	// if determinant is negative, no solution
	if (det < 0.0f) return false;

	// calculate two roots: (if det==0 then x1==x2
	// but lets disregard that slight optimization)
	// burningwater: sqrt( 0) is an illegal operation.... smth should be done...

	float sqrtD = (float)sqrt(det);

	float r1 = (-b - sqrtD) / (2*a);
	float r2 = (-b + sqrtD) / (2*a);

	// sort so x1 <= x2
	if (r1 > r2) 
		math::Swap<float>(r1,r2);

	// get lowest root
	if (r1 > 0 && r1 < maxR)
	{
		*root = r1;
		return true;
	}

	// its possible that we want x2, this can happen if x1 < 0
	if (r2 > 0 && r2 < maxR)
	{
		*root = r2;
		return true;
	}

	return false;
}


math::vector3d SceneHelper::getVectorFromScreen(const math::recti &viewPort,const math::vector2d& p,float depth,CameraNode*camera){

	if(!camera){
		return math::vector3d(-10000);
	}


	math::line3d line;//=getRayFromScreenCoord(p,camera);

	const math::Frustum *f=camera->getViewFrustrum();

	math::vector3d flu=f->getFarLUp();
	math::vector3d ltr=f->getFarRUp()-flu;
	math::vector3d utd=f->getFarLDown()-flu;

	
	math::Point2di screenSize(viewPort.getWidth(),viewPort.getHeight());

	float dx=p.x/(float)screenSize.x;
	float dy=p.y/(float)screenSize.y;

	line.pStart=camera->getAbsolutePosition();//+ (ltr * (dx-0.5f)) + (utd * (dy-0.5f));
	line.pEnd=flu+ltr*dx+utd*dy;

	return line.pStart + line.getVector().Normalize()*depth;
}
math::vector3d SceneHelper::getScreenCoordFromPos(const math::vector3d& p,scene::CameraNode*cam){
	math::vector3d pos(-10000);
	if(!cam)return pos;

	math::recti view=cam->getViewPort();
	math::vector2d dim(view.getWidth(),view.getHeight());
	dim/=2;

	math::matrix4x4 trans=cam->getProjectionMatrix();
	trans=trans*cam->getViewMatrix();

	float transformPos[4];
	transformPos[0]=p.x;
	transformPos[1]=p.y;
	transformPos[2]=p.z;
	transformPos[3]=1;

	trans.multWithVec4(transformPos);

	if(transformPos[3]<0)return pos;
	float zDiv=transformPos[3] == 0 ? 1 : 1/transformPos[3];
	pos.x=(dim.x * transformPos[0] *zDiv)+dim.x;
	pos.y=(dim.y-(dim.y * transformPos[1] *zDiv));
	pos.z=transformPos[3];

	return pos;
}

math::line3d SceneHelper::getRayFromScreenCoord(const math::vector2d& p,CameraNode*cam){
	math::line3d line(0,0);
	if(!cam)return line;

	const math::Frustum *f=cam->getViewFrustrum();

	math::vector3d flu=f->getFarLUp();
	math::vector3d ltr=f->getFarRUp()-flu;
	math::vector3d utd=f->getFarLDown()-flu;


	math::recti viewPort=cam->getViewPort();
	math::Point2di screenSize(viewPort.getWidth(),viewPort.getHeight());

	float dx=p.x/(float)screenSize.x;
	float dy=p.y/(float)screenSize.y;

	line.pStart=cam->getAbsolutePosition();
	line.pEnd=flu+ltr*dx+utd*dy;

	return line;
}

void SceneHelper::getPickedNode(const MovableNodeList& nodes,const math::line3d&ray,float &bestDist,ISceneNode* &bestNode,
									  uint idMask,uint nodeTypeMask){
	math::vector3d edges[8];
	MovableNodeList::const_iterator p=nodes.begin();
	MovableNodeList::const_iterator end=nodes.end();
	for(;p!=end;++p)
	{
		math::box3d box;
		ISceneNode* curr=dynamic_cast<ISceneNode*>((*p).pointer());
		if(curr && curr->isVisible() && (!idMask || curr->getID()&idMask) && (!nodeTypeMask || ((uint)curr->GetNodeType() & nodeTypeMask)) )
		{
			math::matrix4x4 mat;

			if(curr->getAbsoluteTransformation().getInverse(mat)){
				math::line3d ln(ray);

				ln.pStart=mat*(ln.pStart);
				ln.pEnd=mat*(ln.pEnd);
				box=curr->getBoundingBox();
				if(box.intersectsWithLine(ln))
				{
					box.getEdges(edges);
					float dist=0;
					for(int e=0;e<8;e++)
					{
						float t=edges[e].getDistSQ(ln.pStart);
						if(t>dist)
							dist=t;
					}
					if(dist<bestDist){
						bestDist=dist;
						bestNode=curr;
					}
				}
			}
		}
		getPickedNode((*p)->getChildren(),ray,bestDist,bestNode,idMask,nodeTypeMask);
	}
}

ISceneNode*  SceneHelper::getNodeFromRay(const MovableNodeList& nodes ,const math::line3d& ln,uint idMask,uint nodeTypeMask){
	ISceneNode* out=0;
	float dist=9999999999;
	getPickedNode(nodes,ln,dist,out,idMask,nodeTypeMask);
	return out;
}

ISceneNode*  SceneHelper::getNodeFromScreenCoord(const math::vector2d& p,uint idMask,ISceneNode* node){
	math::line3d line=getRayFromScreenCoord(p,0);
	return getNodeFromRay(node->getChildren(),line,idMask,0);
}


int SceneHelper::getCollisionTriangle(CTriangleSelector *tSelector,const math::line3d&  ray,math::triangle3d&outT,math::vector3d& outV){
	if(!tSelector)return -1;
	int totalT=tSelector->getTrianglesCount();/*
	triangles.resize(totalT);

	tSelector->getTriangles(triangles.pointer(),totalT,0);*/

	float dist=math::Infinity;
	math::vector3d lineVec=ray.getVector();
	lineVec.Normalize();
	math::vector3d p;
	float lineLen=ray.LengthSQ();
	bool found=0;
	math::triangle3d t;
	for(int i=0;i<totalT;++i)
	{
		tSelector->getTriangle(t,i);
		if(t.isIntersectWithLine(ray.pStart,lineVec,p)){
			float tmp=p.getDistSQ(ray.pStart);
			float tmp2=p.getDistSQ(ray.pEnd);
			if(tmp<lineLen && tmp2<lineLen && tmp<dist){
				dist=tmp;
				outT=t;
				outV=p;
				found=1;
			}
		}
	}
	if(found)return dist;
	return -1;
}
/*

ISceneNode*  SceneHelper::getCollisionTriangleBB(core::IteratorPair<SceneNodeList> nodes,const math::line3d& ray,uint idMask,math::triangle3d&outT,math::vector3d &outV)
{
	ISceneNode* node=getNodeFromRay(nodes,ray,idMask,0);
	if(!node || node->getMesh()==0)return 0;

	selector->setTriangles(node->getMesh(),&node->getAbsoluteTransformation());

	int dist=getCollisionTriangle(selector,ray,outT,outV);


	if(dist==-1)return 0;
	return node;

}
*/


math::vector3d SceneHelper::getCollisionPosition(const math::vector3d &pos,const math::vector3d &eRadius,const math::vector3d &vel,
	const math::vector3d&gravity,CTriangleSelector*world,float slidingSpeed,math::triangle3d &outTri,bool &outFall){

	//check for noncollision
	if(!world || eRadius.x==0.0f || eRadius.y==0.0f || eRadius.z==0.0f)
		return pos;

	return getEllispoidWorldCollision(pos,eRadius,vel,gravity,world,slidingSpeed,outTri,outFall);
}

// This code is based on the paper "Improved Collision detection and Response" by Kasper Fauerby
math::vector3d SceneHelper::getEllispoidWorldCollision(const math::vector3d &pos,const math::vector3d &eRadius,const math::vector3d &vel,
	const math::vector3d&gravity,CTriangleSelector*world,float slidingSpeed,math::triangle3d &outTri,bool &outFall)
{

	CollisionPack pack;
	pack.eRadius=eRadius;
	pack.Pos=pos;
	pack.Vel=vel;
	pack.nearestDist=math::Infinity;
	pack.world=world;
	pack.slidingSpeed=slidingSpeed;
	pack.triCountHit=0;


	//convert into ellipsoid space
	math::vector3d eSPos=pos / eRadius;
	math::vector3d eSVel=vel / eRadius;

	//iterate until we got final pos
	math::vector3d finalPos=collideWithWorld(pack,eSPos,eSVel,0);

	
	outFall = false;

	// add gravity

	if (gravity != 0)
	{
		pack.Pos = finalPos * pack.eRadius;
		pack.Vel = gravity;
		pack.triCountHit = 0;

		eSVel = gravity/pack.eRadius;

		finalPos = collideWithWorld(pack,
			finalPos, eSVel,0);

		outFall = (pack.triCountHit == 0);
	}

	if (pack.triCountHit)
	{
		outTri = pack.collisionTri;
		outTri.pA *= pack.eRadius;
		outTri.pB *= pack.eRadius;
		outTri.pC *= pack.eRadius;
	}

	finalPos *= pack.eRadius;
	return finalPos;
}



math::vector3d SceneHelper::collideWithWorld(CollisionPack&pack,const math::vector3d& pos,const math::vector3d &vel,int depth){
	
	if(depth>5)
		return pos;

	float veryCloseDistance = pack.slidingSpeed;

	pack.Vel=vel;
	pack.basePoint=pos;
	pack.normalizedVel=vel;
	pack.normalizedVel.Normalize();
	pack.foundCollision=0;
	pack.nearestDist=math::Infinity;

	
	// get only the triangles that we might collide with
	math::box3d box(pack.Pos);
	box.AddPoint(pack.Pos + pack.Vel);
	box.MinP -= pack.eRadius;
	box.MaxP += pack.eRadius;

	int totalTriangleCnt = pack.world->getTrianglesCount();
	triangles.resize(totalTriangleCnt);

	math::matrix4x4 scaleMat;
	scaleMat.setScale(
				math::vector3d(1.0f / pack.eRadius.x, 
						 1.0f / pack.eRadius.y,
						 1.0f / pack.eRadius.z)
					);

	int triangleCnt = pack.world->getTriangles(&triangles[0], totalTriangleCnt, box, &scaleMat);
	//int triangleCnt = pack.world->getTriangles(triangles.pointer(), totalTriangleCnt, &scaleMat);
	for (int i=0; i<triangleCnt; ++i)
		testTriangleIntersection(pack, triangles[i]);

	//---------------- end collide with world

	if (!pack.foundCollision)
		return pos + vel;

	// original destination point
	math::vector3d destinationPoint = pos + vel;
	math::vector3d newBasePoint = pos;

	// only update if we are not already very close
	// and if so only move very close to intersection, not to the
	// exact point
	if (pack.nearestDist >= veryCloseDistance)
	{
		math::vector3d v = vel;
		v.setLength( pack.nearestDist - veryCloseDistance );
		newBasePoint = pack.basePoint + v;

		v.Normalize();
		pack.collisionPoint -= (v * veryCloseDistance);
	}

	// calculate sliding plane

	math::vector3d slidePlaneOrigin = pack.collisionPoint;
	math::vector3d slidePlaneNormal = newBasePoint - pack.collisionPoint;
	slidePlaneNormal.Normalize();
	math::Plane slidingPlane(slidePlaneOrigin, slidePlaneNormal);

	math::vector3d newDestinationPoint =destinationPoint -
		(slidePlaneNormal * slidingPlane.getDistance(destinationPoint));

	// generate slide vector

	math::vector3d newVelocityVector = newDestinationPoint - pack.collisionPoint;

	if (newVelocityVector.Length() < veryCloseDistance)
		return newBasePoint;

	return collideWithWorld(pack,newBasePoint, newVelocityVector,depth+1);
}

void SceneHelper::testTriangleIntersection(CollisionPack&pack,const math::triangle3d&tri){

	math::Plane plane=tri.getPlane();

	//test if it's not onfront of the triangle
//	if(!(plane.getVertLocation(pack.normalizedVel)>0))
//		return;

	float t0,t1;
	bool embeddedInPlane = 0;

	// calculate signed distance from sphere position to triangle plane
	float signedDistToTrianglePlane = plane.getDistance(pack.basePoint);

	float NdV=plane.Normal.dotProduct(pack.Vel);

	// check if the sphere is parallel to the plane
	if ( math::isZero ( NdV) )
	{

		//check if it's far away from the plane
		if (fabs(signedDistToTrianglePlane) >= 1.0f)
			return; // no collision possible
		else
		{
			// sphere is embedded in plane
			embeddedInPlane = true;
			t0 = 0.0;
			t1 = 1.0;
		}
	}
	else
	{
		NdV = 1.0f / NdV;

		// N.V is not 0. Calculate intersection interval
		t0 = (-1.f - signedDistToTrianglePlane) * NdV;
		t1 = (1.f - signedDistToTrianglePlane) * NdV;

		// Swap so t0 < t1
		if (t0 > t1) 
			math::Swap<float>(t0,t1);

		// check if at least one value is within the range
		if (t0 > 1.0f || t1 < 0.0f)
			return; // both t values are outside 1 and 0, no collision possible

		// clamp to 0 and 1
		t0 = math::clamp<float> ( t0, 0.0f, 1.0f );
		t1 = math::clamp<float> ( t1, 0.0f, 1.0f );
	}

	// at this point we have t0 and t1, if there is any intersection, it
	// is between this interval
	math::vector3d collisionPoint;
	bool foundCollision = false;
	float t = 1.0f;

	// first check the easy case: Collision within the triangle;
	// if this happens, it must be at t0 and this is when the sphere
	// rests on the front side of the triangle plane. This can only happen
	// if the sphere is not embedded in the triangle plane.

	if (!embeddedInPlane)
	{
		math::vector3d planeIntersectionPoint =
			(pack.basePoint - plane.Normal)
			+ (pack.Vel * t0);

		if (tri.isPointInsideFast(planeIntersectionPoint))
		{
			foundCollision = true;
			t = t0;
			collisionPoint = planeIntersectionPoint;
		}
	}

	// if we havent found a collision already we will have to sweep
	// the sphere against points and edges of the triangle. Note: A
	// collision inside the triangle will always happen before a
	// vertex or edge collision.

	if (!foundCollision)
	{
		math::vector3d velocity = pack.Vel;
		math::vector3d base = pack.basePoint;

		float velocitySqaredLength = velocity.LengthSQ();
		float a,b,c;
		float newT;

		// for each edge or vertex a quadratic equation has to be solved:
		// a*t^2 + b*t + c = 0. We calculate a,b, and c for each test.

		// check against points
		a = velocitySqaredLength;

		// p1
		b = 2.0f * (velocity.dotProduct(base - tri.pA));
		c = (tri.pA-base).LengthSQ() - 1.0f;
		if (getLowestRoot(a,b,c,t, &newT))
		{
			t = newT;
			foundCollision = true;
			collisionPoint = tri.pA;
		}

		// p2
		if (!foundCollision)
		{
			b = 2.0f * (velocity.dotProduct(base - tri.pB));
			c = (tri.pB-base).LengthSQ() - 1.f;
			if (getLowestRoot(a,b,c,t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = tri.pB;
			}
		}

		// p3
		if (!foundCollision)
		{
			b = 2.0f * (velocity.dotProduct(base - tri.pC));
			c = (tri.pC-base).LengthSQ() - 1.f;
			if (getLowestRoot(a,b,c,t, &newT))
			{
				t = newT;
				foundCollision = true;
				collisionPoint = tri.pC;
			}
		}

		// check against edges:

		// p1 --- p2
		math::vector3d edge = tri.pB - tri.pA;
		math::vector3d baseToVertex = tri.pA - base;
		float edgeSqaredLength = edge.LengthSQ();
		float edgeDotVelocity = edge.dotProduct(velocity);
		float edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2.f *velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1.f -baseToVertex.LengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			float f = (edgeDotVelocity*newT - edgeDotBaseToVertex) / edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = tri.pA + (edge*f);
			}
		}

		// p2 --- p3
		edge = tri.pC-tri.pB;
		baseToVertex = tri.pB - base;
		edgeSqaredLength = edge.LengthSQ();
		edgeDotVelocity = edge.dotProduct(velocity);
		edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2*velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1-baseToVertex.LengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			float f = (edgeDotVelocity*newT-edgeDotBaseToVertex) /
				edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = tri.pB + (edge*f);
			}
		}


		// p3 --- p1
		edge = tri.pA-tri.pC;
		baseToVertex = tri.pC - base;
		edgeSqaredLength = edge.LengthSQ();
		edgeDotVelocity = edge.dotProduct(velocity);
		edgeDotBaseToVertex = edge.dotProduct(baseToVertex);

		// calculate parameters for equation
		a = edgeSqaredLength* -velocitySqaredLength +
			edgeDotVelocity*edgeDotVelocity;
		b = edgeSqaredLength* (2*velocity.dotProduct(baseToVertex)) -
			2.0f*edgeDotVelocity*edgeDotBaseToVertex;
		c = edgeSqaredLength* (1-baseToVertex.LengthSQ()) +
			edgeDotBaseToVertex*edgeDotBaseToVertex;

		// does the swept sphere collide against infinite edge?
		if (getLowestRoot(a,b,c,t,&newT))
		{
			float f = (edgeDotVelocity*newT-edgeDotBaseToVertex) /
				edgeSqaredLength;
			if (f >=0.0f && f <= 1.0f)
			{
				// intersection took place within segment
				t = newT;
				foundCollision = true;
				collisionPoint = tri.pC + (edge*f);
			}
		}
	}// end no collision found

	// set result:
	if (foundCollision)
	{
		// distance to collision is t
		float distToCollision = t*pack.Vel.Length();

		// does this triangle qualify for closest hit?
		if (!pack.foundCollision ||	distToCollision	< pack.nearestDist)
		{
			pack.nearestDist = distToCollision;
			pack.collisionPoint = collisionPoint;
			pack.foundCollision = true;
			pack.collisionTri = tri;
			++pack.triCountHit;
		}

	}// end found collision
}
/*
ISceneNode*  SceneHelper::getCollisionTriangleMesh(ISceneNode* node,math::line3d ray,math::triangle3d&outT,math::vector3d &outV){
	C_List<ISceneNode* >::iterator it=node->getChildren()->begin();
	int dist=99999999999;

	for(;it!=node->getChildren()->end();++it)
	{
		ISceneNode* curr=*it;
		if(curr->Visible && !curr->IsPassable() && (!nodeTypeMask || ((uint)curr->getNodeType() & nodeTypeMask)) 
			&& curr->getMesh())
		{
			selector->setTriangles(curr,curr->getMesh());
			int d=getCollisionTriangle(selector,ray,outT,outV);
			if(d!=-1 && d<dist){
				dist=d;
			}
		}
	}

}


*/
}

}