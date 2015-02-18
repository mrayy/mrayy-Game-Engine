

#ifndef ___SMeshManipulator___
#define ___SMeshManipulator___


#include "ISingleton.h"
#include "IHardwareIndexBuffer.h"





namespace mray{
namespace scene{

	class SMesh;
	class IMeshBuffer;

class MRAY_DLL SMeshManipulator:public ISingleton<SMeshManipulator>
{
	std::vector<uint> m_tmpIndiciesUINT;

public:
	SMeshManipulator();
	~SMeshManipulator();

	void recalcNormalsFlat  (math::vector3d*vtx,math::vector3d*norm,void*ind,int iCnt,
		video::IHardwareIndexBuffer::EIndexType indexType);

	void recalcNormalsSmooth(math::vector3d*vtx,math::vector3d*norm,int vCnt,void*ind,int iCnt,
		video::IHardwareIndexBuffer::EIndexType indexType);

	void makePlanarMappingT(math::vector3d*vtx,int vCnt,void*ind,int iCnt,
		video::IHardwareIndexBuffer::EIndexType indexType,math::vector2d *coord,float resolution);

	void recalcNormalsFlat  (scene::SMesh*mesh);
	void recalcNormalsSmooth(scene::SMesh*mesh);
	void makePlanarTextureMapping(scene::SMesh*mesh,float resolution=0.1f);

	void flipMeshSurface(scene::SMesh*mesh);

	void createTangentMesh(scene::SMesh*mesh,bool onlyCurrFrame=1);

	void calcTangent(math::vector3d &normal,math::vector3d &tangent,math::vector3d &binormal,
		const math::vector3d &v1,const math::vector3d &v2,const math::vector3d &v3,
		const math::vector2d &t1,const math::vector2d &t2,const math::vector2d &t3);

	int getPolyCount(scene::SMesh*mesh);

	void setTextureTilt(IMeshBuffer*mesh,float u,float v);
	void setTextureTilt(scene::SMesh*mesh,float u,float v);

	void translateMesh(IMeshBuffer*mesh,math::vector3d t);
	void rotateMesh(IMeshBuffer*mesh,math::quaternion&q);
	void scaleMesh(IMeshBuffer*mesh,math::vector3d s,math::vector3d center=0);
	void applyTransformationOnMesh(IMeshBuffer*mesh,math::matrix4x4&r);

	void calculateBoundingSphere(IMeshBuffer*mesh,math::vector3d&center,float &radius);

	static bool ConvertTrisToTriStrips(const ushort *ind,int iCnt,ushort** retIndicies,ushort& retCount);
	static bool ConvertTrisToTriStrips(IMeshBuffer*mesh);

};

}
}


#endif