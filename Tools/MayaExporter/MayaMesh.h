


/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   11:24
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaMesh.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaMesh
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaMesh__
#define __MayaMesh__

#include "MayaVertex.h"
#include "MayaNode.h"

namespace mray
{
namespace exporter
{
	class ExporterParams;

	class MayaSubmesh;
	class MayaSkeleton;

	struct DagInfo
	{
		long offset;
		long numVertices;
		MDagPath dagPath;
		//BlendShape* pBlendShape;
	};
	struct SharedGeometry
	{
		std::vector<MayaVertex> vertices;
		std::vector<DagInfo> dagMap;
	};

	typedef stdext::hash_map<int,int> SubmeshPosRemapping;

	typedef stdext::hash_map<int,SubmeshPosRemapping> PosRemapping;

class MayaMesh:public MayaNode
{
private:
protected:
	std::vector<uvset> m_uvs;
	std::vector<MayaSubmesh*> m_submeshs;
	SharedGeometry m_sharedGeom;

	std::vector<vertexInfo> m_newVertices;
	std::vector<MFloatArray> m_newWeights;
	std::vector<MIntArray> m_newJointIds;
	MPointArray m_newPoints;
	MFloatVectorArray m_newNormals;
	MStringArray m_newUVSets;

	MFnSkinCluster* m_skinCluster;
	MObjectArray m_shaders;
	MIntArray m_shaderPolygonRemapping;
	std::vector<faceArray> m_polygonSets;
	bool m_opposite;
	PosRemapping m_posRemapping;

	MayaSkeleton* m_skeleton;

	MStatus _getVertices(const MDagPath& dpath,ExporterParams& params);
	MStatus _getFaces(const MDagPath& dpath,ExporterParams& params);
	MStatus _getSkinCluster(const MDagPath& dpath,ExporterParams& params);
	MStatus _getBlendShapeDeformer(const MDagPath& dpath,ExporterParams& params);
	MStatus _getShaders(const MDagPath& dpath);
	MStatus _getVertexBoneWeights(const MDagPath& dpath,ExporterParams& params);
	MStatus _getUVSets(const MDagPath& dpath);
	MStatus _buildSharedGeom(const MDagPath& dpath,ExporterParams& params);

	MStatus _createSubmeshes(const MDagPath& dpath,ExporterParams& params);

	virtual void _OnLoadComponent(const MDagPath& dpath,ExporterParams& params);
public:
	MayaMesh(const MString &name="");
	virtual~MayaMesh();

	void clear();

	MStatus read(const MDagPath& dpath,ExporterParams& params);


	void saveMesh(ExporterParams& params,const core::string& path);

	virtual xml::XMLElement* saveXML(xml::XMLElement* parent,ExporterParams& params);
};

}
}

#endif
