
/********************************************************************
	created:	2008/11/01
	created:	1:11:2008   23:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\MeshGenerator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	MeshGenerator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Generating Mesh
*********************************************************************/

#ifndef ___MeshGenerator___
#define ___MeshGenerator___

#include "ISingleton.h"
#include "SMesh.h"
#include "ITexture.h"

#include "IViewNode.h"

namespace mray{
namespace scene{

class MRAY_DLL MeshGenerator:public ISingleton<MeshGenerator>
{
	enum EGenTypes
	{
		EGT_Box,
		EGT_Sphere,
		EGT_Capsule,
		EGT_Torus,
		EGT_Cylinder,
		EGT_Plane,
		EGT_Skydome,
		EGT_SkyPlane,
		EGT_Skybox,
		EGT_Terrain,
		EGT_Billboard,
		EGT_BillboardBeam,
		EGT_Nodes_Count
	};

	int m_genId[EGT_Nodes_Count];

	void setMeshName(SMesh*mesh,EGenTypes type);

	std::vector<uint> m_tmpIndiciesUINT;
	std::vector<ushort> m_tmpIndiciesUSHORT;

public:

	MeshGenerator();
	virtual~MeshGenerator();

	void generateBox(const math::vector3d&  extent, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generateSphere(float radius, int uSlices, int vSlices, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generateCapsule(float radius, float height, int segments, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generateTorus(float radius1, float radius2, int segments, int sides, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generateCylinder(float radius, float height, int segments, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generatePlane(int widthSegment, int lengthSegment, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);

	void generateSkyDome(video::ITexture*tex,int uSegments,int vSegments,
		double texPer,double spherePer,float radius,SMesh*mesh);
	void generateSkyPlane(video::ITexture*tex,int uSegments,int vSegments,
		double texPercentage,float hight,float size,SMesh*mesh);
	void generateSkyBox(SMesh*mesh);

	void generateTerrain(video::ITexture*heightMap,int patchSize,SMesh*mesh);

	void generateBillboard(IViewNode*cam, const math::vector3d& pos, const math::vector2d& size, float angle, bool rotHor, bool rotVer, math::vector3d*verts, int startIndex = 0, int startVertex = 0);
	void generateBillboard(IViewNode*cam, const math::vector3d& pos, const math::vector2d& size, float angle, bool rotHor, bool rotVer, IMeshBuffer*mesh, int startIndex = 0, int startVertex = 0);
	void generateBillboardBeam(IViewNode*cam, float width, const math::vector3d&  start, const math::vector3d&  end, IMeshBuffer*mesh, const math::matrix4x4* worldMat, int startIndex = 0, int startVertex = 0);
	void generateBillboardChain(IViewNode*cam, float startW, float endW, const math::vector3d* points, int count , IMeshBuffer*mesh, const math::matrix4x4* worldMat, int startIndex = 0, int startVertex = 0);
};

}
}


#endif //___MeshGenerator___


