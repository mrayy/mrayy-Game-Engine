#include "stdafx.h"


#include "SMeshManipulator.h"



#include "ILogManager.h"
#include "SMesh.h"
#include "NvTriStrip.h"
#include "mraySystem.h"

namespace mray{

namespace scene{

SMeshManipulator::SMeshManipulator()
{
	#ifdef ___DEBUG___
	setDebugName("SMeshManipulator");
	#endif
}

SMeshManipulator::~SMeshManipulator()
{
}

void SMeshManipulator::recalcNormalsFlat(math::vector3d*vtx,math::vector3d*norm,void*ind,int iCnt,
										 video::IHardwareIndexBuffer::EIndexType indexType)
{
	if(!vtx || !ind)return;
	math::vector3d normal;
	union{
		ushort*i16;
		uint*i32;
	}index;
	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
		index.i16=(ushort*)ind;
	}else
		index.i32=(uint*)ind;

	uint indTuple[3];
	for(int i=0;i<iCnt;i+=3)
	{

		if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
			indTuple[0]=index.i16[i+0];
			indTuple[1]=index.i16[i+1];
			indTuple[2]=index.i16[i+2];
		}else{
			indTuple[0]=index.i32[i+0];
			indTuple[1]=index.i32[i+1];
			indTuple[2]=index.i32[i+2];
		}
		normal=(vtx[indTuple[0]]-vtx[indTuple[2]]).crossProduct(vtx[indTuple[0]]-vtx[indTuple[1]]);
		normal.Normalize();
		norm[indTuple[0]]=normal;
		norm[indTuple[1]]=normal;
		norm[indTuple[2]]=normal;
	}
}

void SMeshManipulator::recalcNormalsSmooth(math::vector3d*vtx,math::vector3d*norm,int vCnt,void*ind,int iCnt,
										   video::IHardwareIndexBuffer::EIndexType indexType)
{
	if(!vtx || !ind)return;
	math::vector3d normal;
	for(int i=0;i<vCnt;++i)
		norm[i]=0;

	union{
		ushort*i16;
		uint*i32;
	}index;
	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
		index.i16=(ushort*)ind;
	}else
		index.i32=(uint*)ind;

	uint indTuple[3];
	for(int i=0;i<iCnt;i+=3)
	{
		if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
			indTuple[0]=index.i16[i+0];
			indTuple[1]=index.i16[i+1];
			indTuple[2]=index.i16[i+2];
		}else{
			indTuple[0]=index.i32[i+0];
			indTuple[1]=index.i32[i+1];
			indTuple[2]=index.i32[i+2];
		}
		math::Plane p(vtx[indTuple[0]],vtx[indTuple[2]],vtx[indTuple[1]]);
		norm[indTuple[0]]+=p.Normal;
		norm[indTuple[1]]+=p.Normal;
		norm[indTuple[2]]+=p.Normal;
	}
	for(int i=0;i<vCnt;++i)
		norm[i].Normalize();
}

void SMeshManipulator::makePlanarMappingT(math::vector3d*vtx,int vCnt,void*ind,int iCnt,
										  video::IHardwareIndexBuffer::EIndexType indexType,math::vector2d *coord,float resolution)
{
	union{
		ushort*i16;
		uint*i32;
	}index;
	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
		index.i16=(ushort*)ind;
	}else
		index.i32=(uint*)ind;
	uint indTuple[3];
	for(int i=0;i<iCnt;i+=3){
		if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
			indTuple[0]=index.i16[i+0];
			indTuple[1]=index.i16[i+1];
			indTuple[2]=index.i16[i+2];
		}else{
			indTuple[0]=index.i32[i+0];
			indTuple[1]=index.i32[i+1];
			indTuple[2]=index.i32[i+2];
		}
		math::Plane p(vtx[indTuple[0]],vtx[indTuple[1]],vtx[indTuple[2]]);
		p.Normal.x=fabs(p.Normal.x);
		p.Normal.y=fabs(p.Normal.y);
		p.Normal.z=fabs(p.Normal.z);

		///calc planar mapping worldspace coord
		if(p.Normal.x>p.Normal.y && p.Normal.x>p.Normal.z)
			for(int t=0;t<3;t++){
				coord[indTuple[t]].x=vtx[indTuple[t]].y * resolution;
				coord[indTuple[t]].y=vtx[indTuple[t]].z * resolution;
			}
		else 
			if(p.Normal.y>p.Normal.x && p.Normal.y>p.Normal.z)
				for(int t=0;t<3;t++){
					coord[indTuple[t]].x=vtx[indTuple[t]].x * resolution;
					coord[indTuple[t]].y=vtx[indTuple[t]].z * resolution;
				}
			else 
				for(int t=0;t<3;t++){
					coord[indTuple[t]].x=vtx[indTuple[t]].x * resolution;
					coord[indTuple[t]].y=vtx[indTuple[t]].y * resolution;
				}
	}
}
void SMeshManipulator::recalcNormalsFlat  (scene::SMesh*mesh)
{
	for(int i=0;i<mesh->getBuffersCount();++i){
		IMeshBuffer *buff= mesh->getBuffer(i);
		video::IHardwareStreamBuffer*vS=buff->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*nS=buff->getStream(0,video::EMST_Normal);

		video::IHardwareIndexBuffer*ib=buff->getIndexData()->indexBuffer;
		if(!ib)
			return;

		void*data=(void*)ib->lock(buff->getIndexData()->firstIndex*ib->getIndexSize(),
			buff->getIndexData()->indexCount*ib->getIndexSize(),
			video::IHardwareIndexBuffer::ELO_ReadOnly);

		recalcNormalsFlat((math::vector3d*)vS->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly),(math::vector3d*)nS->lock(0,0,video::IHardwareBuffer::ELO_Discard),
			data,buff->getIndexData()->indexCount,ib->getIndexType());
		ib->unlock();
		nS->unlock();

		vS->unlock();

	}


}


void SMeshManipulator::makePlanarTextureMapping(scene::SMesh*mesh,float resolution){
	int mcount=mesh->getBuffersCount();
	for(int i=0;i<mcount;++i){
		IMeshBuffer*buff=mesh->getBuffer(i);
		video::StreamsData*sd=buff->getStreamsData();
		video::IHardwareStreamBuffer*vS=buff->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*tS=buff->getStream(0,video::EMST_Texcoord);
		int vCnt=vS->length();
		video::IHardwareIndexBuffer*ib=buff->getIndexData()->indexBuffer;
		if(!ib)
			return;
		void*data=(void*)ib->lock(buff->getIndexData()->firstIndex*ib->getIndexSize(),
			buff->getIndexData()->indexCount*ib->getIndexSize(),
			video::IHardwareIndexBuffer::ELO_ReadOnly);

		makePlanarMappingT((math::vector3d*)vS->lock(sd->start,sd->count,video::IHardwareBuffer::ELO_ReadOnly),
			vCnt,data,buff->getIndexData()->indexCount,ib->getIndexType(),(math::vector2d*)tS->lock(0,0,video::IHardwareBuffer::ELO_Discard),resolution);
		ib->unlock();

		vS->unlock();
		tS->unlock();
	}
}



void SMeshManipulator::recalcNormalsSmooth(scene::SMesh*mesh)
{
	for(int i=0;i<mesh->getBuffersCount();++i){
		IMeshBuffer*buff= mesh->getBuffer(i);
		video::StreamsData*sd=buff->getStreamsData();
		video::IHardwareStreamBuffer*vS=buff->getStream(0,video::EMST_Position);
		video::IHardwareStreamBuffer*nS=buff->getStream(0,video::EMST_Normal);

		video::IHardwareIndexBuffer*ib=buff->getIndexData()->indexBuffer;
		if(!ib)
			return;
		void*data=(void*)ib->lock(buff->getIndexData()->firstIndex*ib->getIndexSize(),
			buff->getIndexData()->indexCount*ib->getIndexSize(),
			video::IHardwareIndexBuffer::ELO_ReadOnly);


		recalcNormalsSmooth((math::vector3d*)vS->lock(sd->start,sd->count,video::IHardwareBuffer::ELO_ReadOnly),
			(math::vector3d*)nS->lock(sd->start,sd->count,video::IHardwareBuffer::ELO_Discard),
			vS->length(),data,
			buff->getIndexData()->indexCount,ib->getIndexType());
		ib->unlock();

		vS->unlock();
		nS->unlock();

	}
	
}
void SMeshManipulator::flipMeshSurface(scene::SMesh*mesh)
{
#ifdef _TODO_
	for(int i=0;i<mesh->getBuffersCount();++i){
		IMeshBuffer*b1=mesh->getBuffer(i);
		int iCnt=b1->Indices.size();
		uint *ind=(uint*)b1->Indices.pointer();
		S3DVertex *vtx=b1->currFrame.pointer();

		for(int j=0;j<iCnt;j+=3)
		{
			math::Swap<S3DVertex>(vtx[j+1],vtx[j+2]);
		}
	}
#endif
}


int SMeshManipulator::getPolyCount(scene::SMesh*mesh)
{
	int pCnt=0;
	int mCnt=mesh->getBuffersCount();
	for(int i=0;i<mCnt;++i){
		video::IHardwareIndexBuffer*ib=mesh->getBuffer(i)->getIndexData()->indexBuffer;
		if(!ib)
			continue;

		int pSize=1;
		switch(mesh->getBuffer(i)->getRenderType()){
		case video::MR_POINTS:
			pSize=1;
			break;
		case video::MR_LINES:
			pSize=2;
			break;
		case video::MR_LINE_LOOP:
		case video::MR_LINE_STRIP:
			pSize=1;
			break;
		case video::MR_TRIANGLES:
			pSize=3;
			break;
		case video::MR_TRIANGLE_FAN:
		case video::MR_TRIANGLE_STRIP:
			pSize=1;
			break;
		case video::MR_QUADS:
			pSize=4;
			break;
		case video::MR_QUAD_STRIP:
			pSize=2;
			break;
		case video::MR_POLYGON:
			pSize=1;
			break;
		}

		pCnt+=ib->getIndexCount()/pSize;
	}

	return pCnt;
}

void SMeshManipulator::createTangentMesh(scene::SMesh*mesh,bool onlyCurrFrame){
#ifdef _TODO_
	if(!mesh)return;
	int bufCnt=mesh->getBuffersCount();
/*
	scene::SMesh*out=new SMesh();
	for(int i=0;i<bufCnt;++i){
		int iCnt=mesh->getBuffer(i)->getIndexCount();
		const uint*idx=mesh->getBuffer(i)->getIndices();
		IMeshBuffer*buf=new IMeshBuffer();
		*buf->material=*mesh->getBuffer(i)->material;
		if(!mesh->getBuffer(i)->getVertexCount())continue;

		if(!mesh->getBuffer(i)->currFrame.size())continue;

			S3DVertex*vtxT=(S3DVertex*)mesh->getBuffer(i)->currFrame.pointer();;
			for(int j=0;j<iCnt;j++){
				buf->currFrame.push_back(vtxT[idx[j]]);
			
		}
		buf->NumberOfTri=mesh->getBuffer(i)->NumberOfTri;
		buf->Indices.resize(iCnt);
		for(int j=0;j<iCnt;j++)
			buf->Indices[j]=j;
		out->addBuffer(buf);
	}
	out->setBoundingBox(mesh->getBoundingBox());*/

	//now for tangents..
	for(int m=0;m<bufCnt;m++){
		int iCnt=mesh->getBuffer(m)->getIndexCount();
		const uint*idx=mesh->getBuffer(m)->getIndices();
		S3DVertex*v=(S3DVertex*)mesh->getBuffer(m)->currFrame.pointer();

		for(int i=0;i<iCnt;i+=3){
			calcTangent(
				v[idx[i  ]].Normal,
				v[idx[i  ]].Tangent,
				v[idx[i  ]].Binormal,
				v[idx[i  ]].Pos,
				v[idx[i+1]].Pos,
				v[idx[i+2]].Pos,
				v[idx[i  ]].TCoord,
				v[idx[i+1]].TCoord,
				v[idx[i+2]].TCoord);
			
			calcTangent(
				v[idx[i+1]].Normal,
				v[idx[i+1]].Tangent,
				v[idx[i+1]].Binormal,
				v[idx[i+1]].Pos,
				v[idx[i+2]].Pos,
				v[idx[i+0]].Pos,
				v[idx[i+1]].TCoord,
				v[idx[i+2]].TCoord,
				v[idx[i+0]].TCoord);
			
			calcTangent(
				v[idx[i+2]].Normal,
				v[idx[i+2]].Tangent,
				v[idx[i+2]].Binormal,
				v[idx[i+2]].Pos,
				v[idx[i+0]].Pos,
				v[idx[i+1]].Pos,
				v[idx[i+2]].TCoord ,
				v[idx[i+0]].TCoord,
				v[idx[i+1]].TCoord);
		}
		mesh->getBuffer(m)->vertexType=EVertexCoordType::EVCT_TANGENTS;
	}
#endif
}


void SMeshManipulator::calcTangent(math::vector3d &normal,math::vector3d &tangent,math::vector3d &binormal,
	const math::vector3d &v1,const math::vector3d &v2,const math::vector3d &v3,
	const math::vector2d &t1,const math::vector2d &t2,const math::vector2d &t3){
	

#ifndef USE_NVIDIA_GLH_VERSION
	
	math::vector3d vt1=v1-v2;
	math::vector3d vt2=v3-v1;
	normal=v2.crossProduct(v1);
	normal.Normalize();

	float dX1=t1.x-t2.x;
	float dX2=t3.x-t1.x;
	binormal=(vt1*dX2)-(vt2*dX1);
	binormal.Normalize();

	float dY1=t1.y-t2.y;
	float dY2=t3.y-t1.y;
	tangent=(vt1*dY2)-(vt2*dY1);
	tangent.Normalize();

	math::vector3d txb=tangent.crossProduct(binormal);
	if(txb.dotProduct(normal)<0){
		tangent*=-1;
		binormal*=-1;
	}
/*
	math::vector3d vt1=v2-v1;
	math::vector3d vt2=v3-v1;

	float s1=t2.x-t1.x;
	float s2=t3.x-t1.x;

	float t1=t2.y-t1.y;
	float t2=t3.y-t1.y;

	float r=1.0f/(s1*t2-s2*t1);
	math::vector3d sdir( (t2*vt1.x-t1*vt2.x)*r,
						 (t2*vt1.z-t1*vt2.y)*r,
						 (t2*vt1.y-t1*vt2.z)*r);

	math::vector3d tdir( (s1*vt2.x-s2*vt1.x)*r,
						 (s1*vt2.z-s2*vt1.y)*r,
						 (s1*vt2.y-s2*vt1.z)*r);


	
	normal=v2.crossProduct(v1);
	normal.Normalize();

	binormal=(vt1*dX2)-(vt2*dX1);
	binormal.Normalize();

	tangent=(vt1*dY2)-(vt2*dY1);
	tangent.Normalize();

	math::vector3d txb=tangent.crossProduct(binormal);
	if(txb.dotProduct(normal)<0){
		tangent*=-1;
		binormal*=-1;
	}*/

#elif
	tangent=0;
	binormal=0;

	math::vector3d vt1(v2.x-v1.x,t2.x-t1.x,t2.y-t1.y);
	math::vector3d vt1(v3.x-v1.x,t3.x-t1.x,t3.y-t1.y);

	math::vector3d txb=v1.crossProduct(v2);
	if(!math::isZero(txb.x)){
		tangent.x=-txb.y/txb.x;
		binormal.x=-txb.z/txb.x;
	}

	vt1.x=v2.y-v1.y;
	vt2.x=v3.y-v1.y;
	
	math::vector3d txb=v1.crossProduct(v2);
	if(!math::isZero(txb.x)){
		tangent.y=-txb.y/txb.x;
		binormal.y=-txb.z/txb.x;
	}

	vt1.x=v2.z-v1.z;
	vt2.x=v3.z-v1.z;
	
	math::vector3d txb=v1.crossProduct(v2);
	if(!math::isZero(txb.x)){
		tangent.z=-txb.y/txb.x;
		binormal.z=-txb.z/txb.x;
	}
	tangent.Normalize();
	binormal.Normalize();

	normal=tangent.crossProduct(binormal);
	normal.Normalize();

	binormal=tangent.crossProduct(normal);
	binormal.Normalize();

	math::Plane p(v1,v2,v3);
	if(normal.dotProduct(p.Normal)<0)
		normal*=-1;
#endif
}

void SMeshManipulator::setTextureTilt(IMeshBuffer*mesh,float u,float v){
//	math::vector2d*uvCrd=mesh->getOrginalUV();
	video::IHardwareStreamBuffer*tcS=mesh->getStream(0,video::EMST_Texcoord);
	int vtxCnt	  =tcS->length();
// 	int uvCnt	  =mesh->getOrginalUVCount();
// 	if(vtxCnt!=uvCnt){
// 		gLogManager.log(mT("SMeshManipulator::setTextureTilt() - vertex count and uv count not match"),ELL_WARNING);
// 		return;
// 	} 
	math::vector2d*tc=(math::vector2d*)tcS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	for(int i=0;i<vtxCnt;++i){
		tc[i].x*=u;
		tc[i].y*=v;
	}
	tcS->unlock();
}
void SMeshManipulator::setTextureTilt(scene::SMesh*mesh,float u,float v){
	int cnt=mesh->getBuffersCount();
	for(int i=0;i<cnt;++i){
		setTextureTilt(mesh->getBuffer(i),u,v);
	}
}

void SMeshManipulator::translateMesh(IMeshBuffer*mesh,math::vector3d t){
	math::box3d bb;

	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*v=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int vtxCnt	  =vS->length();
	for(int i=0;i<vtxCnt;++i){
		v[i]+=t;
		if(i==0){
			bb.reset(v[i]);
		}else{
			bb.AddPoint(v[i]);
		}
	}
	vS->unlock();
	mesh->setBoundingBox(bb);

}
void SMeshManipulator::rotateMesh(IMeshBuffer*mesh,math::quaternion&q){
	math::box3d bb;

	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*v=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int vtxCnt	  =vS->length();
	for(int i=0;i<vtxCnt;++i){
		v[i]=q*v[i];
		if(i==0){
			bb.reset(v[i]);
		}else{
			bb.AddPoint(v[i]);
		}
	}
	vS->unlock();
	mesh->setBoundingBox(bb);
}
void SMeshManipulator::scaleMesh(IMeshBuffer*mesh,math::vector3d s,math::vector3d center){
	math::box3d bb;
	
	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*v=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int vtxCnt	  =vS->length();
	for(int i=0;i<vtxCnt;++i){
		v[i]=(v[i]-center)*s+center;
		if(i==0){
			bb.reset(v[i]);
		}else{
			bb.AddPoint(v[i]);
		}
	}
	vS->unlock();
	mesh->setBoundingBox(bb);
}
void SMeshManipulator::applyTransformationOnMesh(IMeshBuffer*mesh,math::matrix4x4&m){
	math::box3d bb;

	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*v=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int vtxCnt	  =vS->length();
	for(int i=0;i<vtxCnt;++i){
		v[i]=m*v[i];
		if(i==0){
			bb.reset(v[i]);
		}else{
			bb.AddPoint(v[i]);
		}
	}
	vS->unlock();
	mesh->setBoundingBox(bb);

}

void SMeshManipulator::calculateBoundingSphere(IMeshBuffer*mesh,math::vector3d&center,float &radius){
	if(!mesh)return;
	mesh->recalculateBoundingBox();
	math::box3d b=mesh->getBoundingBox();
	center=b.getCenter();
	float maxRad=0;
	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*v=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	int vtxCnt	  =vS->length();
	for(int i=0;i<vtxCnt;++i){
		float d=(v[i]-center).LengthSQ();
		if(d>maxRad)
			maxRad=d*d;
	}
	vS->unlock();
	radius=sqrtf(maxRad);
}


bool SMeshManipulator::ConvertTrisToTriStrips(const ushort *ind,int iCnt,ushort** retIndicies,ushort& retCount)
{

	PrimitiveGroup *groups;
	ushort numGroups;
	if(!GenerateStrips(ind,iCnt,&groups,&numGroups,false))
		return false;

	retCount=groups[0].numIndices;
	*retIndicies=new ushort[retCount];
	mraySystem::memCopy(*retIndicies,groups[0].indices,sizeof(ushort)*retCount);
	delete [] groups;
	return true;
}
bool SMeshManipulator::ConvertTrisToTriStrips(IMeshBuffer*mesh)
{
	if(mesh->getRenderType()!=video::MR_TRIANGLES)
		return false;

	video::IndexData*ind=mesh->getIndexData();
	ushort *idx=new ushort[ind->indexCount];

	PrimitiveGroup *groups;
	ushort numGroups;

	ind->indexBuffer->readData(ind->firstIndex*sizeof(ushort),ind->indexCount*sizeof(ushort),idx);

	if(!GenerateStrips(idx,ind->indexCount,&groups,&numGroups,false))
	{
		delete [] idx;
		return false;
	}


	ind->indexCount=groups[0].numIndices;
	ind->indexBuffer->resize(groups[0].numIndices*sizeof(ushort));
	ind->indexBuffer->writeData(0,groups[0].numIndices*sizeof(ushort),(void*)groups[0].indices);
	mesh->setRenderType(video::MR_TRIANGLE_STRIP);

	delete [] groups;
	delete [] idx;
	return true;
}

}
}
