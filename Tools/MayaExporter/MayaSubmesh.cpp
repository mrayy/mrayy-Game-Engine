
#include "stdafx.h"
#include "MayaSubmesh.h"
#include "ExporterParams.h"
#include "MayaMaterial.h"
#include "MayaMaterialSet.h"
#include "IMeshBuffer.h"
#include "MeshBufferData.h"
#include "TangentCalculater.h"



namespace mray
{
namespace exporter
{

MayaSubmesh::MayaSubmesh(MayaMesh* creator,const MString &name)
{
	m_mesh=creator;
	m_name=name;
	m_hasVBA=false;
}
MayaSubmesh::~MayaSubmesh()
{
	clear();
}

void MayaSubmesh::clear()
{
}

MStatus MayaSubmesh::read(const MDagPath& dPath,
						  std::vector<face>& faces,std::vector<vertexInfo>& vertInfos,MPointArray& points,
						  MFloatVectorArray& normals,MStringArray& texcoordSets,
						  ExporterParams& params,bool opposite)
{
	MString message;
	MStatus stat;

	m_dPath=dPath;
	MFnMesh mesh(dPath);
	message= "Reading Submesh : " + mesh.fullPathName();
	MGlobal::displayInfo(message);

	//save uvsets info
	for(int i=m_uvs.size();i<texcoordSets.length();++i)
	{
		uvset uv;
		uv.size=2;
		m_uvs.push_back(uv);
	}

	//iterate over faces,to retreive vertices info
	std::vector<int> vtx_mapping;
	vtx_mapping.resize(vertInfos.size());
	for(int i=0;i<vtx_mapping.size();++i)
		vtx_mapping[i]=-1;
	for(int i=0;i<faces.size();++i)
	{
		face f;
		if(params.useSharedGeom)
		{
			if(opposite)
			{
				f.v[0]=faces[i].v[2];
				f.v[1]=faces[i].v[1];
				f.v[2]=faces[i].v[0];
			}else
			{
				f.v[0]=faces[i].v[0];
				f.v[1]=faces[i].v[1];
				f.v[2]=faces[i].v[2];
			}
		}else
		{
			for(int j=0;j<3;++j)
			{
				long newId;
				long id=faces[i].v[j];
				if(vtx_mapping[id]==-1)
				{
					m_indices.push_back(id);
					newId=m_indices.size()-1;
					vtx_mapping[id]=newId;
				}else
					newId=vtx_mapping[id];
				//save the index in the set to the face info
				if(opposite)
					f.v[2-j]=newId;
				else
					f.v[j]=newId;
			}
		}
		m_faces.push_back(f);
	}

	//get vertices info
	for(int i=0;i<m_indices.size();++i)
	{
		long vtx_id=m_indices[i];
		MayaVertex v;
		vertexInfo vinfo=vertInfos[vtx_id];

		MPoint pt=points[vinfo.pointId]*params.scaler;
		//if(fabs(pt.x)<PRECISION)
		//	pt.x=0;

		v.x=pt.x;
		v.y=pt.y;
		v.z=pt.z;

		MFloatVector normal=normals[vinfo.normalId];

		if(opposite)
			normal=-normal;

		v.normal.x=normal.x;
		v.normal.y=normal.y;
		v.normal.z=normal.z;

		v.normal.normalize();

		v.r=vinfo.r;
		v.g=vinfo.g;
		v.b=vinfo.b;
		v.a=vinfo.a;

		float sum=0;
		//vertex bone assignments 
		for(int k=0;k<vinfo.vba.size();++k)
		{
			vba newVba;
			newVba.boneId=vinfo.boneId[k];
			newVba.weight=vinfo.vba[k];
			if (newVba.weight > PRECISION)
			{
				sum+=newVba.weight;
				m_hasVBA=true;
				v.vbas.push_back(newVba);
			}
		}
		if(v.vbas.size()>4)
		{
			//limit to 4
			vba top[4];
			float sum=0;
			std::vector<vba>::iterator it;
			for(int i=0;i<4;++i)
			{
				std::vector<vba>::iterator best;

				for(it=v.vbas.begin();it!=v.vbas.end();++it)
				{
					if((*it).weight>top[i].weight)
					{
						top[i]=*it;
						best=it;
					}
				}
				sum+=top[i].weight;
				v.vbas.erase(best);
			}
			v.vbas.clear();
			for(int i=0;i<4;++i)
			{
				top[i].weight/=sum;
				v.vbas.push_back(top[i]);
			}
		}else if(sum!=1 && sum>0)
		{
			sum=1.0f/sum;
			for(int i=0;i<v.vbas.size();++i)
			{
				v.vbas[i].weight*=sum;
			}
		}
		//texcoords 
		for(int k=0;k<vinfo.u.size();++k)
		{
			texcoord tc;
			tc.u=vinfo.u[k];
			tc.v=vinfo.v[k];
			tc.w=0;
			v.texcoords.push_back(tc);
		}
		v.index=vinfo.pointId;
		m_vertices.push_back(v);
	}

	//use 32bit indices
	if(params.useSharedGeom || m_vertices.size()>65535 || m_faces.size()>65535)
		m_32bitIndices=true;
	else m_32bitIndices=false;

	MPoint min = mesh.boundingBox().min();
	MPoint max = mesh.boundingBox().max();
	MBoundingBox bbox(min,max);
	if (params.exportWorldCoords)
		bbox.transformUsing(dPath.inclusiveMatrix());

	min = bbox.min() * params.scaler;
	max = bbox.max() * params.scaler;
	m_boundingBox=MBoundingBox(min,max);

//	params.loadedSubmeshs.push_back(this);
	MGlobal::displayInfo("Submesh Parsed Successfully");

	return MS::kSuccess;
}

MStatus MayaSubmesh::readMaterial(MObject& shader,MStringArray&uvsets,ExporterParams& params)
{
	MPlug plug;
	MPlugArray srcplugarray;
	bool foundShader = false;
	MStatus stat;
	MFnDependencyNode* pShader;
	//get shader from shading group
	MFnDependencyNode shadingGroup(shader);
	plug = shadingGroup.findPlug("surfaceShader");
	plug.connectedTo(srcplugarray,true,false,&stat);
	for (int i=0; i<srcplugarray.length() && !foundShader; i++)
	{
		if (srcplugarray[i].node().hasFn(MFn::kLambert) || srcplugarray[i].node().hasFn(MFn::kSurfaceShader) || 
			srcplugarray[i].node().hasFn(MFn::kPluginHwShaderNode) )
		{
			pShader = new MFnDependencyNode(srcplugarray[i].node());
			foundShader = true;
		}
	}
	if (foundShader)
	{
		std::cout << "Found material: " << pShader->name().asChar() << "\n";
		std::cout.flush();
		//check if this material has already been created
		//fix material name, adding the requested prefix
		MString tmpStr = params.matPrefix;
		if (tmpStr != "")
			tmpStr += "/";
		tmpStr += pShader->name();
		MStringArray tmpStrArray;
		tmpStr.split(':',tmpStrArray);
		MString name = "";
		for (int i=0; i<tmpStrArray.length(); i++)
		{
			name += tmpStrArray[i];
			if (i < tmpStrArray.length()-1)
				name += "_";
		}
		MayaMaterial* pMaterial = MayaMaterialSet::GetInstance().GetMaterial(name.asChar());
		//if the material has already been created, update the pointer
		if (pMaterial)
			m_material = pMaterial;
		//else create it and add it to the material set
		else
		{
			pMaterial = new MayaMaterial();
			pMaterial->read(pShader,uvsets,params);
			m_material=MayaMaterialSet::GetInstance().AddMaterial(name.asChar(),pMaterial);
		}
		//delete temporary shader
		delete pShader;
	}
	else
	{
		std::cout << "Unsupported material, replacing with default lambert\n";
		std::cout.flush();
		m_material = MayaMaterialSet::GetInstance().GetDefaultMaterial();
	}

	//loading complete
	return MS::kSuccess;
}


void MayaSubmesh::fillBuffer(scene::MeshBufferData*buffData,ExporterParams& params)
{
	scene::IMeshBuffer* buffer=buffData->getMeshBuffer();
	core::string strName;
	core::char_to_string(m_name.asChar(),strName);
	buffer->setMeshName(strName);
	buffer->setBoundingBox(math::box3d(	m_boundingBox.min().x,m_boundingBox.min().y,m_boundingBox.min().z,
										m_boundingBox.max().x,m_boundingBox.max().y,m_boundingBox.max().z));
	
	buffer->setRenderType(video::MR_TRIANGLES);

	video::IHardwareIndexBuffer::EIndexType indexType=m_32bitIndices ? video::IHardwareIndexBuffer::EIT_32Bit : video::IHardwareIndexBuffer::EIT_16Bit;

	buffer->createIndexBuffer(indexType,m_faces.size()*3,params.indexBufferUsage,false,true);
	buffer->getIndexData()->firstIndex=0;
	buffer->getIndexData()->indexCount=m_faces.size()*3;
	video::IHardwareIndexBuffer* indexBuffer=buffer->getIndexData()->indexBuffer.pointer();
	void*data= indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	union 
	{
		ushort* i16;
		uint* i32;
	}idx;
	if(m_32bitIndices)
		idx.i32=(uint*)data;
	else
		idx.i16=(ushort*)data;

	for(int i=0;i<m_faces.size();++i)
	{
		if(m_32bitIndices)
		{
			*(idx.i32++)=(uint)m_faces[i].v[0];
			*(idx.i32++)=(uint)m_faces[i].v[1];
			*(idx.i32++)=(uint)m_faces[i].v[2];
		}
		else
		{
			*(idx.i16++)=(ushort)m_faces[i].v[0];
			*(idx.i16++)=(ushort)m_faces[i].v[1];
			*(idx.i16++)=(ushort)m_faces[i].v[2];
		}
	}

	
	{
		video::IHardwareStreamBuffer* posStream= buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,m_vertices.size(),params.streamsBufferUsage,false,true);


		math::vector3d* pPtr=(math::vector3d*)posStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		math::vector3d* nPtr=0;
		video::SColor*  cPtr=0;

		for(int i=0;i<m_vertices.size();++i)
		{
			(*pPtr++).set(m_vertices[i].x,m_vertices[i].y,m_vertices[i].z);
		}
		posStream->unlock();
		if(params.exportVertNorms)
		{
			video::IHardwareStreamBuffer* normStream=0;
			normStream=buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,m_vertices.size(),params.streamsBufferUsage,false,true);
			nPtr=(math::vector3d*)normStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			for(int i=0;i<m_vertices.size();++i)
			{
				(*nPtr++).set(m_vertices[i].normal.x,m_vertices[i].normal.y,m_vertices[i].normal.z);
			}
			normStream->unlock();
		}

		if(params.exportVertColor)
		{
			video::IHardwareStreamBuffer* clrStream=0;
			clrStream=buffer->createStream(0,video::EMST_Color,video::ESDT_Point4f,m_vertices.size(),params.streamsBufferUsage,false,true);
			cPtr=(video::SColor*)clrStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			for(int i=0;i<m_vertices.size();++i)
			{
				(*cPtr++).Set(m_vertices[i].r,m_vertices[i].g,m_vertices[i].b,m_vertices[i].a);
			}
			clrStream->unlock();
		}

		if(params.exportVBA && m_hasVBA)
		{
			video::IHardwareStreamBuffer* idStream= buffer->createStream(0,video::EMST_BlendIndicies,video::ESDT_Point4f,m_vertices.size(),params.streamsBufferUsage,false,true);
			video::IHardwareStreamBuffer* weightStream= buffer->createStream(0,video::EMST_BlendWeights,video::ESDT_Point4f,m_vertices.size(),params.streamsBufferUsage,false,true);
			float* idPtr=(float*)idStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			float* wPtr=(float*)weightStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
			for(int i=0;i<m_vertices.size();++i)
			{
				int cnt=math::Min<int>(m_vertices[i].vbas.size(),4);
				int weight=0;
				for(int v=0;v<cnt;++v)
				{
					idPtr[v]=m_vertices[i].vbas[v].boneId;
					wPtr[v] =m_vertices[i].vbas[v].weight;
					weight+=wPtr[v];
				}
				if(weight)
				{
					for(int v=0;v<cnt;++v)
					{
						wPtr[v]/=weight;
					}
				}
				for(int v=cnt;v<4;++v)
				{
					idPtr[v]=0;
					wPtr[v] =0;
				}
				idPtr+=4;
				wPtr+=4;
			}
			idStream->unlock();
			weightStream->unlock();

		}


		if(params.exportTexcoords)
		{
			for(int tcId=0;tcId<m_uvs.size();++tcId)
			{
				video::IHardwareStreamBuffer* tcStream= buffer->createStream(tcId,video::EMST_Texcoord,video::ESDT_Point2f,m_vertices.size(),params.streamsBufferUsage,false,true);
				math::vector2d*  tcPtr=(math::vector2d*)tcStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

				for(int i=0;i<m_vertices.size();++i)
				{
					(*tcPtr++).set(m_vertices[i].texcoords[tcId].u,1-m_vertices[i].texcoords[tcId].v);//,m_vertices[i].texcoords[tcId].w);
				}
				tcStream->unlock();
			}
		}
	}


	indexBuffer->unlock();

	if(params.exportTangents)
	{
		scene::TangentCalculater c(false);
		c.calculateTangents(buffer);
	}
}

xml::XMLElement* MayaSubmesh::saveXML(xml::XMLElement* elem)
{
	xml::XMLElement* e=new xml::XMLElement(mT("Material"));
	elem->addSubElement(e);
	e->addAttribute(mT("Name"),m_material->GetName().asChar());
	e->addAttribute(mT("Submodel"),m_name.asChar());
	return e;
}

}
}