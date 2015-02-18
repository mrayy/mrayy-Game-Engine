


/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   11:26
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaSubmesh.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaSubmesh
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaSubmesh__
#define __MayaSubmesh__

#include "MayaVertex.h"

namespace mray
{
namespace scene
{
	class MeshBufferData;
}
namespace exporter
{
	class ExporterParams;
	class MayaMaterial;
	class MayaMesh;

class MayaSubmesh
{
private:
protected:
	MString m_name;

	std::vector<long> m_indices;
	std::vector<MayaVertex> m_vertices;
	std::vector<face> m_faces;
	std::vector<uvset> m_uvs;
	MBoundingBox m_boundingBox;

	MayaMaterial* m_material;
	bool m_hasVBA;

	bool m_32bitIndices;
	MDagPath m_dPath;

	MayaMesh* m_mesh;

public:
	MayaSubmesh(MayaMesh* creator,const MString &name="");
	virtual~MayaSubmesh();

	MayaMaterial* GetMaterial(){return m_material;}

	MStatus read(const MDagPath& dpath,
		std::vector<face>& faces,std::vector<vertexInfo>& vertInfos,MPointArray& points,
		MFloatVectorArray& normals,MStringArray& texcoordSets,
		ExporterParams& params,bool opposite);

	MStatus readMaterial(MObject& shader,MStringArray&uvsets,ExporterParams& params);

	void clear();

	void fillBuffer(scene::MeshBufferData*buffer,ExporterParams& params);
	xml::XMLElement* saveXML(xml::XMLElement* elem);
};

}
}

#endif
