
#ifndef ___SceneHelper___
#define ___SceneHelper___





#include "CTriangleSelector.h"

#include "ISceneNode.h"

namespace mray{
namespace scene{

	class ISceneNode;
	class CameraNode;

class MRAY_DLL SceneHelper:public ISingleton<SceneHelper>
{
	struct CollisionPack{
		math::vector3d eRadius;
		math::vector3d Pos,Vel;
		math::vector3d basePoint;
		math::vector3d normalizedVel;
		math::vector3d collisionPoint;
		math::triangle3d collisionTri;
		int triCountHit;
		float nearestDist;
		bool foundCollision;
		float slidingSpeed;
		GCPtr<CTriangleSelector>world;
	};

	std::vector<math::triangle3d> triangles;

	CTriangleSelector *selector;
	math::vector3d collideWithWorld(CollisionPack&pack,const math::vector3d& pos,const math::vector3d& vel,int depth);
	void testTriangleIntersection(CollisionPack&pack,const math::triangle3d&tri);
	math::vector3d getEllispoidWorldCollision(const math::vector3d &pos,const math::vector3d &eRadius,const math::vector3d &vel,
			const math::vector3d&gravity,CTriangleSelector*world,float slidingSpeed,math::triangle3d &outTri,bool &outFall);
	

	inline bool getLowestRoot(float a, float b, float c, float maxR, float* root);
public:
	SceneHelper();
	virtual ~SceneHelper();

	math::vector3d getVectorFromScreen(const math::recti &viewPort,const math::vector2d& p,float depth,CameraNode*camera=0);
	math::vector3d getScreenCoordFromPos(const math::vector3d& p,scene::CameraNode*cam);

	math::line3d getRayFromScreenCoord(const math::vector2d& p,CameraNode*camera);

	void getPickedNode(const MovableNodeList& nodes,const math::line3d&ray,float &bestDist,ISceneNode* &bestNode,uint idMask=1,uint nodeTypeMask=1);

	ISceneNode* getNodeFromRay(const MovableNodeList& nodes,const math::line3d& ln,uint idMask=1,uint nodeTypeMask=1);

	ISceneNode*  getNodeFromScreenCoord(const math::vector2d& p,uint idMask=1,ISceneNode* node=0);

	int getCollisionTriangle(CTriangleSelector *tSelector,const math::line3d& ray,math::triangle3d&outT,math::vector3d &outV);

	//fast , not Accurate , depend on the Bounding Box
	//ISceneNode*  getCollisionTriangleBB(core::IteratorPair<SceneNodeList> nodes,const math::line3d& ray,uint idMask,math::triangle3d&outT,math::vector3d &outV);

	math::vector3d getCollisionPosition(const math::vector3d &pos,const math::vector3d &eRadius,const math::vector3d &vel,
		const math::vector3d&gravity,CTriangleSelector*world,float slidingSpeed,math::triangle3d &outTri,bool &outFall);
	
	//slow , Accurate , depend on the Mesh
//	ISceneNode*  getCollisionTriangleMesh(ISceneNode* node,math::line3d ray,math::triangle3d&outT,math::vector3d &outV);
};

}
}



#endif








