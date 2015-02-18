#ifndef ___CTriangleSelector___
#define ___CTriangleSelector___




#include "SMesh.h"
#include "IMeshBuffer.h"

namespace mray{
namespace scene{

class MRAY_DLL CTriangleSelector
{

	std::vector<math::triangle3d> triangles;

	math::box3d m_boundingbox;
	bool m_autoTransform;

	void loadMeshBuffer(IMeshBuffer*mesh,const math::matrix4x4*transform);
public:
	CTriangleSelector();

	virtual ~CTriangleSelector();

	int getTrianglesCount();
	const math::box3d &getBoundingbox();

	void setTriangles(IMeshBuffer*mesh,const math::matrix4x4*transform);
	void setTriangles(scene::SMesh*mesh,const math::matrix4x4*transform);

	bool getTriangle(math::triangle3d&t,int index);

	int getTriangles(math::triangle3d*out,int arraySize,const math::matrix4x4*transform);

	int getTriangles(math::triangle3d*out,int arraySize,const math::line3d&ln,const math::matrix4x4*transform);
	int getTriangles(math::triangle3d*out,int arraySize,const math::box3d &box,const math::matrix4x4*transform);
};



};//scene
};//mray


#endif





