


#ifndef ___IConvexHull___
#define ___IConvexHull___

#include "SMesh.h"


namespace mray{
namespace scene{

class IConvexHull{

public:
	struct SFace{
		SFace(){}
		SFace(math::vector3d&v1,math::vector3d&v2,math::vector3d&v3,int i1,int i2,int i3){
			setTri(v1,v2,v3,i1,i2,i3);
		}
		~SFace(){
			vtx.clear();
		}
		void setTri(math::vector3d&v1,math::vector3d&v2,math::vector3d&v3,int i1,int i2,int i3){
			tri.x=i1;
			tri.y=i2;
			tri.z=i3;
			plane=math::triangle3d(v1,v2,v3).getPlane();
		}
		math::Plane plane;
		math::Point3d<int>tri;
		std::vector<int>vtx;
	};
protected:
	std::list<SFace> convexFaces;
	GCPtr<IMeshBuffer>convexMesh;
	
	void copyMesh(GCPtr<scene::SMesh>mesh);

public:
	IConvexHull():convexMesh(0){
	}
	virtual ~IConvexHull(){
		convexFaces.clear();
	}

	virtual void createConvexHull(GCPtr<scene::SMesh>mesh)=0;
	GCPtr<IMeshBuffer>getConvexMesh(){
		return convexMesh;
	}

	
};


}
}


#endif



