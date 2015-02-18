#include "stdafx.h"

#include "CTriangleSelector.h"


namespace mray{
namespace scene{

CTriangleSelector::CTriangleSelector()
{
}

CTriangleSelector::~CTriangleSelector(){
	triangles.clear();
}
 
const math::box3d &CTriangleSelector::getBoundingbox()
{
	return m_boundingbox;
}

void CTriangleSelector::loadMeshBuffer(IMeshBuffer*mesh,const math::matrix4x4*transform){
	if(!mesh)return;
	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	video::IndexData* id=mesh->getIndexData();
	if(!vS->length() || id->indexCount==0)
		return;

	video::IHardwareIndexBuffer*b= mesh->getIndexData()->indexBuffer;
	if(!b)return;
	void*ind=0;

	ind=b->lock(id->firstIndex*b->getIndexSize(),id->indexCount*b->getIndexSize(),video::IHardwareBuffer::ELO_ReadOnly);

	bool is16bit=b->getIndexType()==video::IHardwareIndexBuffer::EIT_16Bit;

	math::vector3d*vert=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	int cnt	  =vS->length();

	std::vector<math::vector3d> transVerts;

	if(transform)
	{
		transVerts.resize(vS->length());
		transVerts[0]=(*transform)*(vert[0]);
		m_boundingbox.reset(transVerts[0]);
		for(int i=1;i<vS->length();++i){
			transVerts[i]=(*transform)*(vert[i]);
			m_boundingbox.AddPoint(transVerts[i]);
		}
		vert=&transVerts[0];
	}else
	{
		m_boundingbox.reset(vert[0]);
		for(int i=1;i<vS->length();++i){
			m_boundingbox.AddPoint(vert[i]);
		}
	}
	
//	math::matrix4x4 mat=mesh->getAbsoluteTransformation();
	math::triangle3d t;
	if(mesh->getRenderType()==video::MR_TRIANGLES){
		for(int i=0;i<id->indexCount;i+=3){
			if(is16bit){
				t=math::triangle3d(vert[((ushort*)ind)[i]],
					vert[((ushort*)ind)[i+1]],
					vert[((ushort*)ind)[i+2]]);
			}else
			{
				t=math::triangle3d(vert[((uint*)ind)[i]],
					vert[((uint*)ind)[i+1]],
					vert[((uint*)ind)[i+2]]);
			}
			
			triangles.push_back(t);
		}
	}else if(mesh->getRenderType()==video::MR_QUADS){
		for(int i=0;i<id->indexCount;i+=4){
			if(is16bit){
				t=math::triangle3d(vert[((ushort*)ind)[i]],
					vert[((ushort*)ind)[i+1]],
					vert[((ushort*)ind)[i+2]]);
				triangles.push_back(t);
				t=math::triangle3d(vert[((ushort*)ind)[i]],
					vert[((ushort*)ind)[i+2]],
					vert[((ushort*)ind)[i+3]]);
				triangles.push_back(t);
			}else
			{
				t=math::triangle3d(vert[((uint*)ind)[i]],
					vert[((uint*)ind)[i+1]],
					vert[((uint*)ind)[i+2]]);
				triangles.push_back(t);
				t=math::triangle3d(vert[((uint*)ind)[i]],
					vert[((uint*)ind)[i+2]],
					vert[((uint*)ind)[i+3]]);
				triangles.push_back(t);
			}
		}
	}

	vS->unlock();
	b->unlock();
}


void CTriangleSelector::setTriangles(IMeshBuffer*mesh,const math::matrix4x4*transform)
{
	if(!mesh)return;
	triangles.resize(0);
	loadMeshBuffer(mesh,transform);
}
void CTriangleSelector::setTriangles(scene::SMesh*mesh,const math::matrix4x4*transform)
{
	if(!mesh)return;
	triangles.resize(0);
	for(int i=0;i<mesh->getBuffersCount();++i)
		loadMeshBuffer(mesh->getBuffer(i),transform);
}

int CTriangleSelector::getTrianglesCount(){
	return triangles.size();
}
bool CTriangleSelector::getTriangle(math::triangle3d&t,int index)
{
	if(index>=triangles.size())
		return false;
	t= triangles[index];
	return true;
}

int CTriangleSelector::getTriangles(math::triangle3d*out,int arraySize,const math::matrix4x4*transform){
	if(arraySize>triangles.size())
		arraySize=triangles.size();
	
	math::matrix4x4 mat;
	if(transform)
		mat=*transform;

	for(int i=0;i<arraySize;++i){
		out[i]=triangles[i];
		out[i].pA=mat*(out[i].pA);
		out[i].pB=mat*(out[i].pB);
		out[i].pC=mat*(out[i].pC);
	}

	return arraySize;
}
int CTriangleSelector::getTriangles(math::triangle3d*out,int arraySize,const math::line3d&ln,const math::matrix4x4*transform){
	if(arraySize>triangles.size())
		arraySize=triangles.size();
	
	math::matrix4x4 mat;
	if(transform)
		mat=*transform;

	math::triangle3d t;
	int cnt=0;
	math::vector3d lineVec=ln.getVector();
	float lineLen=ln.LengthSQ();
	for(int i=0;i<arraySize;++i){
		t=triangles[i];
		t.pA=mat*(t.pA);
		t.pB=mat*(t.pB);
		t.pC=mat*(t.pC);
		math::vector3d v;
		if(t.isIntersectWithLine(ln.pStart,lineVec,v)){
			float tmp=v.getDistSQ(ln.pStart);
			float tmp2=v.getDistSQ(ln.pEnd);
			if(tmp<lineLen && tmp2<lineLen){
				out[cnt++]=t;
			}
		}
	}

	return cnt;
}

int CTriangleSelector::getTriangles(math::triangle3d*out,int arraySize,const math::box3d &box,const math::matrix4x4*transform){
	if(arraySize>triangles.size())
		arraySize=triangles.size();
	
	math::matrix4x4 mat;
	if(transform)
		mat=*transform;

	math::triangle3d t;
	int cnt=0;
	for(int i=0;i<arraySize;++i){
		t=triangles[i];
		t.pA=mat*(t.pA);
		t.pB=mat*(t.pB);
		t.pC=mat*(t.pC);
		if(t.isInsideBox(box))
			out[cnt++]=t;
	}

	return cnt;
}


}
}

