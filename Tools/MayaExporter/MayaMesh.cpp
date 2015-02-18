

#include "stdafx.h"
#include "MayaMesh.h"
#include "ExporterParams.h"
#include "MayaSubmesh.h"
#include "MayaSkeleton.h"


#include "IFileSystem.h"
#include "StreamWriter.h"
#include "SMesh.h"

#include "MdlWriter.h"
#include "XMLWriter.h"
#include "MayaMaterial.h"

namespace mray
{
namespace exporter
{


MayaMesh::MayaMesh(const MString &name)
{
	m_name=name;
	m_skeleton=0;
}

MayaMesh::~MayaMesh()
{
	clear();
}


void MayaMesh::clear()
{
	for(int i=0;i<m_submeshs.size();++i)
		delete m_submeshs[i];
	m_submeshs.clear();

	m_sharedGeom.vertices.clear();
	m_sharedGeom.dagMap.clear();

	m_uvs.clear();
	m_newVertices.clear();
	m_newWeights.clear();
	m_newJointIds.clear();
	m_polygonSets.clear();
	m_posRemapping.clear();

	m_shaders.clear();

	m_skinCluster=0;
	if(m_skeleton)
		delete m_skeleton;
	m_skeleton=0;
}

MStatus MayaMesh::read(const MDagPath& dpath,ExporterParams& params)
{
	MStatus stat;
	if(!dpath.hasFn(MFn::kMesh))
		return MS::kFailure;


	MFnTransform* nodeTransform = NULL;
	MFnMesh mesh(dpath);
	// get camera transform
	for (int i=0; i<mesh.parentCount(); i++)
	{
		if (mesh.parent(i).hasFn(MFn::kTransform))
		{
			nodeTransform = new MFnTransform(mesh.parent(i));
			continue;
		}
	}

	if(nodeTransform)
		MayaNode::read(*nodeTransform,params);

	m_sharedGeom.vertices.clear();
	m_sharedGeom.dagMap.clear();

	m_uvs.clear();
	m_newVertices.clear();
	m_newWeights.clear();
	m_newJointIds.clear();
	m_polygonSets.clear();
	m_posRemapping.clear();

	m_shaders.clear();

	m_skinCluster=0;
	m_skeleton=0;

	m_opposite=false;

	stat=_getUVSets(dpath);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving UVSets of mesh");
	}

	stat=_getSkinCluster(dpath,params);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving Skincluster of mesh");
	}
	stat=_getBlendShapeDeformer(dpath,params);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving blendshape of mesh");
	}
	stat=_getShaders(dpath);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving shaders of mesh");
	}

	stat=_getVertices(dpath,params);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving vertex data of mesh");
	}

	if(m_skinCluster)
	{
		stat=_getVertexBoneWeights(dpath,params);
		if(stat!=MS::kSuccess)
		{
			MGlobal::displayInfo("Error Retrieving vertex bone weights of mesh");
		}
	}

	stat=_getFaces(dpath,params);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Retrieving faces data of mesh");
	}

	if(params.useSharedGeom)
	{
		stat=_buildSharedGeom(dpath,params);
		if(stat!=MS::kSuccess)
		{
			MGlobal::displayInfo("Error building shared geom data of mesh");
		}
	}
	stat=_createSubmeshes(dpath,params);
	if(stat!=MS::kSuccess)
	{
		MGlobal::displayInfo("Error Creating submeshes of mesh");
	}

	return MS::kSuccess;
}

MStatus MayaMesh::_getVertices(const MDagPath& dpath,ExporterParams& params)
{
	MFnMesh mesh(dpath);
	m_newVertices.resize(mesh.numVertices());
	m_newWeights.resize(mesh.numVertices());
	m_newJointIds.resize(mesh.numVertices());
	for (int i=0; i<m_newVertices.size(); i++)
	{
		m_newVertices[i].pointId = -1;
		m_newVertices[i].normalId = -1;
		m_newVertices[i].next = -2;
	}
	//get vertex positions from mesh
	if (params.exportWorldCoords || (m_skinCluster && params.exportSkeletons))
		mesh.getPoints(m_newPoints,MSpace::kWorld);
	else
		mesh.getPoints(m_newPoints,MSpace::kTransform);
	//get list of normals from mesh data
	if (params.exportWorldCoords)
		mesh.getNormals(m_newNormals,MSpace::kWorld);
	else
		mesh.getNormals(m_newNormals,MSpace::kTransform);
	//check the "opposite" attribute to see if we have to flip normals
	mesh.findPlug("opposite",true).getValue(m_opposite);
	return MS::kSuccess;
}

MStatus MayaMesh::_getShaders(const MDagPath& meshDag)
{
	MStatus stat;
	MFnMesh mesh(meshDag);
	stat = mesh.getConnectedShaders(0,m_shaders,m_shaderPolygonRemapping);
	std::cout.flush();
	if (MS::kSuccess != stat)
	{
		std::cout << "Error getting connected shaders\n";
		std::cout.flush();
		return MS::kFailure;
	}
	std::cout << "Found " << m_shaders.length() << " connected shaders\n";
	std::cout.flush();
	if (m_shaders.length() <= 0)
	{
		std::cout << "No connected shaders, skipping mesh\n";
		std::cout.flush();
		return MS::kFailure;
	}
	// create a series of arrays of faces for each different submesh
	m_polygonSets.clear();
	m_polygonSets.resize(m_shaders.length());
	return MS::kSuccess;
}
MStatus MayaMesh::_getVertexBoneWeights(const MDagPath& meshDag,ExporterParams& params)
{
	unsigned int numWeights;
	MStatus stat;
	std::cout << "Get vbas\n";
	std::cout.flush();
	MItGeometry iterGeom(meshDag);
	for (int i=0; !iterGeom.isDone(); iterGeom.next(), i++)
	{
		MObject component = iterGeom.component();
		MFloatArray vertexWeights;
		stat=m_skinCluster->getWeights(meshDag,component,vertexWeights,numWeights);
		// save the normalized weights
		m_newWeights[i]=vertexWeights;
		if (MS::kSuccess != stat)
		{
			std::cout << "Error retrieving vertex weights\n";
			std::cout.flush();
		}
		
		// get ids for the joints
		if (m_skeleton)
		{
			MDagPathArray influenceObjs;
			m_skinCluster->influenceObjects(influenceObjs,&stat);
			if (MS::kSuccess != stat)
			{
				std::cout << "Error retrieving influence objects for given skin cluster\n";
				std::cout.flush();
			}
			m_newJointIds[i].setLength(m_newWeights[i].length());
			for (int j=0; j<influenceObjs.length(); j++)
			{
				bool foundJoint = false;
				MString partialPathName = influenceObjs[j].partialPathName(); 
				for (int k=0; k<m_skeleton->GetBones().size() && !foundJoint; k++)
				{
					if (partialPathName == m_skeleton->GetBones()[k]->name)
					{
						foundJoint=true;
						m_newJointIds[i][j] = m_skeleton->GetBones()[k]->id;
					}
				}
			}
		}
	}
	return MS::kSuccess;
}

MStatus MayaMesh::_getBlendShapeDeformer(const MDagPath& meshDag,ExporterParams& params)
{
	/*
	MStatus stat;
	MFnMesh mesh(meshDag);
	if (params.exportBlendShapes)
	{
		// get connected blend shape deformer (if present)
		MItDependencyNodes kDepNodeIt( MFn::kBlendShape );            
		for( ;!kDepNodeIt.isDone() && !pBlendShape; kDepNodeIt.next()) 
		{   
			MObject kObject = kDepNodeIt.item();
			MItDependencyGraph itGraph(kObject,MFn::kMesh,MItDependencyGraph::kDownstream,MItDependencyGraph::kDepthFirst);
			for (;!itGraph.isDone() && !pBlendShape; itGraph.next())
			{
				MFnMesh connectedMesh(itGraph.thisNode());
				if (connectedMesh.fullPathName() == mesh.fullPathName())
				{
					pBlendShape = new BlendShape();
					pBlendShape->load(kObject);
					std::cout << "Found blend shape deformer " << pBlendShape->getName().asChar() << " for mesh " 
						<< mesh.name().asChar() << "\n"; 
				}	
			}
		}
	}*/
	return MS::kSuccess;
}
MStatus MayaMesh::_getSkinCluster(const MDagPath& meshDag,ExporterParams& params)
{
	MStatus stat;
	MFnMesh mesh(meshDag);
	m_skinCluster = NULL;
	if (params.exportVBA || params.exportSkeletons)
	{
		// get connected skin clusters (if present)
		MItDependencyNodes kDepNodeIt( MFn::kSkinClusterFilter );            
		for( ;!kDepNodeIt.isDone() && !m_skinCluster; kDepNodeIt.next()) 
		{            
			MObject kObject = kDepNodeIt.item();
			m_skinCluster = new MFnSkinCluster(kObject);
			unsigned int uiNumGeometries = m_skinCluster->numOutputConnections();
			for(uint uiGeometry = 0; uiGeometry < uiNumGeometries; ++uiGeometry ) 
			{
				unsigned int uiIndex = m_skinCluster->indexForOutputConnection(uiGeometry);
				MObject kOutputObject = m_skinCluster->outputShapeAtIndex(uiIndex);
				if(kOutputObject != mesh.object()) 
				{
					delete m_skinCluster;
					m_skinCluster = NULL;
				}
			}
		}
		if (m_skinCluster)
		{
			// load the skeleton
			std::cout << "Loading skeleton data...\n";
			std::cout.flush();
			
			if (!m_skeleton)
				m_skeleton = new MayaSkeleton();
			stat = m_skeleton->read(m_skinCluster,params);
			if (MS::kSuccess != stat)
			{
				std::cout << "Error loading skeleton data\n";
				std::cout.flush();
			}
			else
			{
				std::cout << "OK\n";
				std::cout.flush();
			}
		}
	}
	return MS::kSuccess;
}
MStatus MayaMesh::_getUVSets(const MDagPath& dpath)
{
	MFnMesh mesh(dpath);
	MStatus stat;
	if(mesh.numUVSets()>0)
	{
		stat=mesh.getUVSetNames(m_newUVSets);
		if(stat!=MS::kSuccess)
		{
			MGlobal::displayInfo("Error retrieving uvsets names");
			return MS::kFailure;
		}
	}
	for(int i=m_uvs.size();i<m_newUVSets.length();++i)
	{
		uvset uv;
		uv.size=2;
		m_uvs.push_back(uv);
	}
	return MS::kSuccess;
}

MStatus MayaMesh::_getFaces(const MDagPath& dpath,ExporterParams& params)
{
	MFnMesh mesh(dpath);
	MStatus stat;

	if(mesh.numPolygons()>0)
	{
		MItMeshPolygon faceIt(mesh.object(),&stat);
		if(stat!=MS::kSuccess)
		{
			MGlobal::displayInfo("Error accessing mesh polygons");
			return MS::kFailure;
		}
		for(;!faceIt.isDone();faceIt.next())
		{
			int numTris=0;
			bool different;
			int vtxId;
			int nrmId;
			
			faceIt.numTriangles(numTris);
			for(int iTri=0;iTri<numTris;++iTri)
			{
				MPointArray triPoints;
				MIntArray tmpTriVertId,triVertId;
				int idx;
				face newFace;
				faceIt.getTriangle(iTri,triPoints,tmpTriVertId);
				MIntArray polyInd;
				faceIt.getVertices(polyInd);
				// convert indices to face-relative indices
				for(uint iObj=0;iObj<tmpTriVertId.length();++iObj)
				{
					for(uint iPoly=0;iPoly<polyInd.length();++iPoly)
					{
						if(tmpTriVertId[iObj]==polyInd[iPoly])
						{
							triVertId.append(iPoly);
							break;
						}
					}
				}
				// iterate over triangle's vertices
				for(int i=0;i<3;++i)
				{
					different=true;
					vtxId=faceIt.vertexIndex(triVertId[i],&stat);
					//if(stat!=MS::kSuccess)
					//{
					//}
					nrmId=faceIt.normalIndex(triVertId[i],&stat);

					MColor color;
					if(faceIt.hasColor(triVertId[i]))
					{
						stat=faceIt.getColor(color,triVertId[i]);
						if(stat!=MS::kSuccess)
							color=MColor(1,1,1,1);
						if(color.r>1)color.r=1;
						else if(color.r<PRECISION)color.r=0;

						if(color.g>1)color.g=1;
						else if(color.g<PRECISION)color.g=0;

						if(color.b>1)color.b=1;
						else if(color.b<PRECISION)color.b=0;

						if(color.a>1)color.a=1;
						else if(color.a<PRECISION)color.a=0;
					}else
						color=MColor(1,1,1,1);

					if(m_newVertices[vtxId].next==-2)
					{
						m_newPoints[vtxId].cartesianize();
						m_newVertices[vtxId].pointId=vtxId;
						m_newVertices[vtxId].normalId=nrmId;
						m_newVertices[vtxId].r=color.r;
						m_newVertices[vtxId].g=color.g;
						m_newVertices[vtxId].b=color.b;
						m_newVertices[vtxId].a=color.a;

						m_newVertices[vtxId].u.resize(m_newUVSets.length());
						m_newVertices[vtxId].v.resize(m_newUVSets.length());

						m_newVertices[vtxId].vba.resize(m_newWeights[vtxId].length());
						for(int j=0;j<m_newWeights[vtxId].length();++j)
						{
							m_newVertices[vtxId].vba[j]=m_newWeights[vtxId][j];
						}

						m_newVertices[vtxId].boneId.resize(m_newJointIds[vtxId].length());
						for(int j=0;j<m_newJointIds[vtxId].length();++j)
						{
							m_newVertices[vtxId].boneId[j]=m_newJointIds[vtxId][j];
						}

						for(int j=0;j<m_newUVSets.length();++j)
						{
							float2 uv;
							stat=faceIt.getUV(triVertId[i],uv,&m_newUVSets[j]);
							if(stat!=MS::kSuccess)
							{
								uv[0]=0;
								uv[1]=0;
							}
							m_newVertices[vtxId].u[j]=uv[0];
							m_newVertices[vtxId].v[j]=uv[1];
						}
						newFace.v[i]=m_sharedGeom.vertices.size()+vtxId;
						m_newVertices[vtxId].next=-1;
					}else
					{
						for(int k=vtxId;k!=-1 && different;k=m_newVertices[k].next)
						{
							different=false;
							if(params.exportVertNorms)
							{
								MFloatVector n1=m_newNormals[m_newVertices[k].normalId];
								MFloatVector n2=m_newNormals[nrmId];
								if (n1.x!=n2.x || n1.y!=n2.y || n1.z!=n2.z)
								{
									different = true;
								}
							}
							if(params.exportVertColor
								&&(m_newVertices[k].r!=color.r || m_newVertices[k].g!=color.g ||
								m_newVertices[k].b!=color.b ||m_newVertices[k].a!=color.a))
							{
								different = true;
							}
							if(params.exportTexcoords)
							{
								for(int j=0;j<m_newUVSets.length();++j)
								{
									float2 uv;
									stat = faceIt.getUV(triVertId[i],uv,&m_newUVSets[j]);
									if (stat!=MS::kSuccess)
									{
										uv[0] = 0;
										uv[1] = 0;
									}
									//uv[1] = (1-uv[1]);
									if (m_newVertices[k].u[j]!=uv[0] || m_newVertices[k].v[j]!=uv[1])
									{
										different = true;
									}
								}
							}
							idx=k;
						}
						if(different)
						{
							vertexInfo vtx;
							vtx.pointId=vtxId;
							vtx.normalId=nrmId;
							vtx.r = color.r;
							vtx.g = color.g;
							vtx.b = color.b;
							vtx.a = color.a;
							vtx.vba.resize(m_newWeights[vtxId].length());
							for (int j=0; j<m_newWeights[vtxId].length(); j++)
							{
								vtx.vba[j] = (m_newWeights[vtxId])[j];
							}
							// save joint ids
							vtx.boneId.resize(m_newJointIds[vtxId].length());
							for (int j=0; j<m_newJointIds[vtxId].length(); j++)
							{
								vtx.boneId[j] = (m_newJointIds[vtxId])[j];
							}
							// save vertex texture coordinates
							vtx.u.resize(m_newUVSets.length());
							vtx.v.resize(m_newUVSets.length());
							for (int j=0; j<m_newUVSets.length(); j++)
							{
								float2 uv;
								stat = faceIt.getUV(triVertId[i],uv,&m_newUVSets[j]);
								if (MS::kSuccess != stat)
								{
									uv[0] = 0;
									uv[1] = 0;
								}
								if (fabs(uv[0]) < PRECISION)
									uv[0] = 0;
								if (fabs(uv[1]) < PRECISION)
									uv[1] = 0;
								vtx.u[j] = uv[0];
								vtx.v[j] = uv[1];//(1-uv[1]);
							}
							vtx.next = -1;
							m_newVertices.push_back(vtx);
							// save vertex index in face info
							newFace.v[i] = m_sharedGeom.vertices.size() + m_newVertices.size()-1;
							m_newVertices[idx].next = m_newVertices.size()-1;
						}else
						{
							newFace.v[i]=m_sharedGeom.vertices.size()+idx;
						}
					}
				}

				if(m_shaderPolygonRemapping[faceIt.index()]>=0)
					m_polygonSets[m_shaderPolygonRemapping[faceIt.index()]].push_back(newFace);
			}
		}
	}
	return MS::kSuccess;

}
// Build shared geometry
MStatus MayaMesh::_buildSharedGeom(const MDagPath &meshDag,ExporterParams& params)
{
	// save a new entry in the shared geometry map: we associate the index of the first 
	// vertex we're loading with the dag path from which it has been read
	DagInfo di;
	di.offset = m_sharedGeom.vertices.size();
	di.dagPath = meshDag;
//	di.pBlendShape = pBlendShape;

	// load shared vertices
	for (int i=0; i<m_newVertices.size(); i++)
	{
		MayaVertex v;
		vertexInfo vInfo = m_newVertices[i];
		// save vertex coordinates (rescale to desired length unit)
		MPoint point = m_newPoints[vInfo.pointId] * params.scaler;
	/*	if (fabs(point.x) < PRECISION)
			point.x = 0;
		if (fabs(point.y) < PRECISION)
			point.y = 0;
		if (fabs(point.z) < PRECISION)
			point.z = 0;
			*/
		v.x = point.x;
		v.y = point.y;
		v.z = point.z;
		// save vertex normal
		MFloatVector normal = m_newNormals[vInfo.normalId];
		/*if (fabs(normal.x) < PRECISION)
			normal.x = 0;
		if (fabs(normal.y) < PRECISION)
			normal.y = 0;
		if (fabs(normal.z) < PRECISION)
			normal.z = 0;*/
		if (m_opposite)
			normal=-normal;

		v.normal.x = normal.x;
		v.normal.y = normal.y;
		v.normal.z = normal.z;
	
		v.normal.normalize();
		// save vertex color
		v.r = vInfo.r;
		v.g = vInfo.g;
		v.b = vInfo.b;
		v.a = vInfo.a;
		// save vertex bone assignements
		for (int k=0; k<vInfo.vba.size(); k++)
		{
			vba newVba;
			newVba.boneId = vInfo.boneId[k];
			newVba.weight = vInfo.vba[k];
			v.vbas.push_back(newVba);
		}
		// save texture coordinates
		for (int k=0; k<vInfo.u.size(); k++)
		{
			texcoord newTexCoords;
			newTexCoords.u = vInfo.u[k];
			newTexCoords.v = vInfo.v[k];
			newTexCoords.w = 0;
			v.texcoords.push_back(newTexCoords);
		}
		// save vertex index in maya mesh, to retrieve future positions of the same vertex
		v.index = vInfo.pointId;
		// add newly created vertex to vertices list
		m_sharedGeom.vertices.push_back(v);
	}
	// Make sure all vertices have the same number of texture coordinates
	for (int i=0; i<m_sharedGeom.vertices.size(); i++)
	{
		MayaVertex* pV = &m_sharedGeom.vertices[i];
		for (int j=pV->texcoords.size(); j<m_uvs.size(); j++)
		{
			texcoord newTexCoords;
			newTexCoords.u = 0;
			newTexCoords.v = 0;
			newTexCoords.w = 0;
			pV->texcoords.push_back(newTexCoords);
		}
	}
	// save number of vertices referring to this mesh dag in the dag path map
	di.numVertices = m_sharedGeom.vertices.size() - di.offset;
	m_sharedGeom.dagMap.push_back(di);

	return MS::kSuccess;
}




MStatus MayaMesh::_createSubmeshes(const MDagPath& meshDag,ExporterParams& params)
{
	MStatus stat;
	MFnMesh mesh(meshDag);
	for (int i=0; i<m_shaders.length(); i++)
	{
		// check if the submesh has at least 1 triangle
		if (m_polygonSets[i].size() > 0)
		{
			//create a name for the submesh
			MString submesh_name = meshDag.partialPathName();
			MFnDependencyNode shader_node(m_shaders[i]);
			if (m_shaders.length()>1)
			{
				submesh_name += "_";
				submesh_name += shader_node.name();
			}
			submesh_name=ExporterParams::fixName(submesh_name);
			//create new submesh
			MayaSubmesh* pSubmesh = new MayaSubmesh(this,submesh_name);
			//load linked shader
			stat = pSubmesh->readMaterial(m_shaders[i],m_newUVSets,params);
			if (stat != MS::kSuccess)
			{
				MFnDependencyNode shadingGroup(m_shaders[i]);
				MGlobal::displayInfo("Error loading material for submesh: " + submesh_name );
				return MS::kFailure;
			}
			//load vertex and face data
			stat = pSubmesh->read(meshDag,m_polygonSets[i],m_newVertices,m_newPoints,m_newNormals,m_newUVSets,params,m_opposite);
			//if we're not using shared geometry, save a pointer to the blend shape deformer
			
			//if (pBlendShape && !params.useSharedGeom)
			//	pSubmesh->m_pBlendShape = pBlendShape;

			//add submesh to current mesh
			m_submeshs.push_back(pSubmesh);
			//update number of triangles composing the mesh
			//m_numTriangles += pSubmesh->numTriangles();
		}
	}
	return MS::kSuccess;
}

void MayaMesh::_OnLoadComponent(const MDagPath& dpath,ExporterParams& params)
{
	if(m_componentType=="")
	{
		m_componentType="Scene";
	}
}

xml::XMLElement* MayaMesh::saveXML(xml::XMLElement* parent,ExporterParams& params)
{
	xml::XMLElement* ret=MayaNode::saveXML(parent,params);
	ret->addAttribute("Type","Mesh");
	ret->addAttribute("MeshFile",(m_name+".mdx").asChar());

	saveMesh(params,(params.basePath).asChar());
	return ret;
}


void MayaMesh::saveMesh(ExporterParams& params,const core::string& path)
{
	scene::SMesh* mesh=new scene::SMesh();
	for(int i=0;i<m_submeshs.size();++i)
	{
		scene::MeshBufferData* buffData= mesh->addNewBuffer();
		m_submeshs[i]->fillBuffer(buffData,params);
	}

	if(m_skeleton)
	{
		scene::Skeleton* skeleton=new scene::Skeleton((m_name+"_sk").asChar());
		m_skeleton->fillSkeleton(skeleton,params);
		mesh->setSkeleton(skeleton);
	}
	loaders::mdlWriter writer;
	writer.write(mesh,gFileSystem.openFile((path+m_name.asChar())+mT(".mdl"),OS::BIN_WRITE));

	xml::XMLElement* mdxRoot=new xml::XMLElement(mT("SLODAnimatedMesh"));
	mdxRoot->addAttribute(mT("Type"),mT("AnimatedMesh"));

	if(!params.exportMaterial)//if not exporting as one material file, then export individuals
	{
		xml::XMLElement* matRoot=new xml::XMLElement(mT("Materials"));
		mdxRoot->addSubElement(matRoot);

		for(int i=0;i<m_submeshs.size();++i)
		{
			m_submeshs[i]->GetMaterial()->saveXML(matRoot);
		}
	}
	xml::XMLElement* lods=new xml::XMLElement(mT("LODS"));
	mdxRoot->addSubElement(lods);

	xml::XMLElement* lod=new xml::XMLElement(mT("LOD"));
	lods->addSubElement(lod);
	lod->addAttribute(mT("Mesh"),core::string(m_name.asChar())+ mT(".mdl"));
	for(int i=0;i<m_submeshs.size();++i)
		m_submeshs[i]->saveXML(lod);

	OS::IStreamPtr mdxXML=gFileSystem.openFile((path+m_name.asChar())+".mdx",OS::BIN_WRITE);
	{
		xml::XMLWriter writer;
		writer.addElement(mdxRoot);
		core::string xmlStr=writer.flush();

		OS::StreamWriter sWriter(mdxXML);
		sWriter.writeString(xmlStr);
		mdxXML->close();
	}

	delete mdxRoot;
	delete mesh;
}


}
}