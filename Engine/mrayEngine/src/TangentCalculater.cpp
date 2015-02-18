#include "stdafx.h"


#include "TangentCalculater.h"



namespace mray{
namespace scene{

TangentCalculater::TangentCalculater(bool generateBinormal)
{
	m_generateBinormal=generateBinormal;
}
TangentCalculater::~TangentCalculater()
{
}
void TangentCalculater::calculateTangents(SMesh* mesh){
	for (int i=0;i<mesh->getBuffersCount();++i)
	{
		calculateTangents(mesh->getBuffer(i));
	}
}
float TangentCalculater::_CalcAngleBetween(const math::vector3d&a,const math::vector3d&b)
{

	float LengthQ=a.Length()*b.Length();

	if(LengthQ<0.0001f)LengthQ=0.0001f;			// to prevent division by zero

	float f=a.dotProduct(b)/LengthQ;

	if(f>1.0f)f=1.0f;									// acos need input in the range [-1..1]
	else if(f<-1.0f)f=-1.0f;							//

	return (float)acos(f);								// cosf is not avaiable on every plattform

}

//based on Crytek's Tangent Space Calculation
void TangentCalculater::calculateTangents(IMeshBuffer* buffer)
{
	if(!buffer)return;

	video::IndexData* id=buffer->getIndexData();
	video::IHardwareIndexBuffer*ib=id->indexBuffer;
	if(!ib)
		return;

	video::IHardwareStreamBuffer* vStream=buffer->getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer* nStream=buffer->getStream(0,video::EMST_Normal);
	video::IHardwareStreamBuffer* uvStream=buffer->getStream(0,video::EMST_Texcoord);


	math::vector3d* vtx=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector3d* norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector2d* tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);


	int iCnt=id->indexCount;
	union{
		ushort*i16;
		uint*i32;
	}index;

	uint indTuple[3];
	//uint invertedIndTuple[3];

	void*idxData=(ushort*)ib->lock(id->firstIndex*ib->getIndexSize(),
		id->indexCount*ib->getIndexSize(),
		video::IHardwareIndexBuffer::ELO_ReadOnly);

	video::IHardwareIndexBuffer::EIndexType indexType=ib->getIndexType();

	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit)
		index.i16=(ushort*)idxData;
	else
		index.i32=(uint*)idxData;


	uint triCount=iCnt/3;
	m_baseVecs.clear();
	m_triBaseAssigment.clear();
	m_triBaseAssigment.reserve(triCount);
	m_redundantVerts.clear();

	bool bTextureCoordinatesBroken=false;

	std::multimap<uint,uint,CBaseIndexOrder>		mBaseMap;
	std::vector<CBase33> vTriangleBase;

	math::vector3d vPos[3];
	math::vector2d vUV[3];
	//calculate base vectors per triangle
	{
		math::vector3d vA,vB;
		math::vector3d vU,vV,vN;
		int currentIndex=0;
		for(int i=0;i<triCount;++i,currentIndex+=3)
		{
			if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
				indTuple[0]=index.i16[currentIndex+0];
				indTuple[1]=index.i16[currentIndex+1];
				indTuple[2]=index.i16[currentIndex+2];
			}else{
				indTuple[0]=index.i32[currentIndex+0];
				indTuple[1]=index.i32[currentIndex+1];
				indTuple[2]=index.i32[currentIndex+2];
			}
			for(int j=0;j<3;++j)
			{
				vPos[j]=vtx[indTuple[j]];
				vUV[j]=tc[indTuple[j]];
			}

			//tangent vectors
			vA=vPos[1]-vPos[0];
			vB = vPos[2] - vPos[0];
			vA.Normalize();
			vB.Normalize();

			math::vector2d Delta1(vUV[1].x - vUV[0].x, vUV[1].y - vUV[0].y);
			math::vector2d Delta2(vUV[2].x - vUV[0].x, vUV[2].y - vUV[0].y);
			Delta1.Normalize();
			Delta2.Normalize();

			float div=(Delta1.x*Delta2.y - Delta2.x*Delta1.y);
			if(_isnan(div))
			{
				bTextureCoordinatesBroken=true;
				div=0;
			}
			vN = vA.crossProduct(vB);// .Normalize();
			if(div!=0)
			{
				float area=fabs(div);
				div=1.0f/div;

				float a=Delta2.y*div;
				float b=-Delta1.y*div;
				float c=-Delta2.x*div;
				float d=Delta1.x*div;
				vU = (vA*a + vB*b).Normalize()*area;
				vV = (vA*c + vB*d).Normalize()*area;
			}else
			{
// 				vU.set(1,0,0);
// 				vV.set(0,1,0);
				vU=vA;
				vV=vB;
			}
			vTriangleBase.push_back(CBase33(vU,vV,vN));
		}
	}

	//distrbute the normals over verticies
	{

		// we create a new tangent base for every vertex index that has a different normal (later we split further for mirrored use)
		// and sum the base vectors (weighted by angle and mirrored if necessary)
		int currentIndex=0;
		for(int i=0;i<triCount;++i,currentIndex+=3)
		{

			if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
				indTuple[0]=index.i16[currentIndex+0];
				indTuple[1]=index.i16[currentIndex+1];
				indTuple[2]=index.i16[currentIndex+2];
			}else{
				indTuple[0]=index.i32[currentIndex+0];
				indTuple[1]=index.i32[currentIndex+1];
				indTuple[2]=index.i32[currentIndex+2];
			}
			CBase33& triBase=vTriangleBase[i];
			for(int j=0;j<3;++j)
			{
//				invertedIndTuple[j]=indTuple[j];//buffer->getInvertedIndex(indTuple[j]);
				vPos[j]=vtx[indTuple[j]];
			}
			
			for(int j=0;j<3;++j)
			{
				float weight=_CalcAngleBetween(vPos[(j+2)%3]-vPos[j],vPos[(j+1)%3]-vPos[j]);
				if(weight<=0.0f)
					weight=0.0001f;

				_AddNormal2Base(mBaseMap,indTuple[j],triBase.N*weight);
			}
		}
	}
	{

		int currentIndex=0;
		for(int i=0;i<triCount;++i,currentIndex+=3)
		{

			if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
				indTuple[0]=index.i16[currentIndex+0];
				indTuple[1]=index.i16[currentIndex+1];
				indTuple[2]=index.i16[currentIndex+2];
			}else{
				indTuple[0]=index.i32[currentIndex+0];
				indTuple[1]=index.i32[currentIndex+1];
				indTuple[2]=index.i32[currentIndex+2];
			}
			CTriBaseIndex ind;
			CBase33& triBase=vTriangleBase[i];
			for(int j=0;j<3;++j)
			{
				//invertedIndTuple[j]=indTuple[j];//buffer->getInvertedIndex(indTuple[j]);
				vPos[j]=vtx[indTuple[j]];
			}

			math::vector3d n=triBase.N;
			n.Normalize();
			for(int j=0;j<3;++j)
			{
				float weight=_CalcAngleBetween(vPos[(j+2)%3]-vPos[j],vPos[(j+1)%3]-vPos[j]);
				ind.p[j]=_AddUV2Base(mBaseMap,indTuple[j],triBase.U*weight,triBase.V*weight,n);
			}
			m_triBaseAssigment.push_back(ind);
		}
	}
	{
		std::vector<CBase33>::iterator it;
		math::vector3d vUout,vVout;

		for(it=m_baseVecs.begin();it!=m_baseVecs.end();++it)
		{
			CBase33& ref=*it;
			//vNout=ref.N;
			ref.N.Normalize();

			vUout=ref.U-ref.N*(ref.N.dotProduct(ref.U));
			vVout=ref.V-ref.N*(ref.N.dotProduct(ref.V));
			
			ref.U=vUout.Normalize();
			ref.V=vVout.Normalize();
			//ref.N=vNout;
		}
	}
/*
	nStream->unlock();
	norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	for(int i=0;i<m_baseVecs.size();++i)
	{
		CBase33& base=m_baseVecs[i];

		norm[i]=base.N;
		tang[i]=base.U;
		if(m_generateBinormal)
			bin[i]=base.V;
	}

	{

		int currentIndex=0;
		//uint ind[3];
		for(int i=0;i<triCount;++i,currentIndex+=3)
		{

			if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
				indTuple[0]=index.i16[currentIndex+0];
				indTuple[1]=index.i16[currentIndex+1];
				indTuple[2]=index.i16[currentIndex+2];
			}else{
				indTuple[0]=index.i32[currentIndex+0];
				indTuple[1]=index.i32[currentIndex+1];
				indTuple[2]=index.i32[currentIndex+2];
			}

			CTriBaseIndex &indx=m_triBaseAssigment[i];
			//GetTriangleBaseInd(i,ind);
			for(int j=0;j<3;++j)
			{
//				invertedIndTuple[j]=ind[j];//buffer->getInvertedIndex(ind[j]);
				CBase33& base=m_baseVecs[indx.p[j]];

				norm[indx.p[j]]=base.N;
				tang[indx.p[j]]=base.U;
				if(m_generateBinormal)
					bin[indx.p[j]]=base.V;
			}
		}
	}*/
	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	ib->unlock();

	_RecreateBuffers(buffer);
}

void TangentCalculater::_RecreateBuffers(IMeshBuffer* buffer)
{
	video::IHardwareStreamBuffer* binStream=0;
	math::vector3d* bin=0;

	video::IndexData* id=buffer->getIndexData();
	video::IHardwareIndexBuffer*ib=id->indexBuffer;
	if(!ib)
		return;

	int vCount=m_baseVecs.size();

	video::StreamsDataCRef sdata=buffer->getStreamsData();
	for(video::StreamsMap::const_iterator it=sdata->streams.begin();it!=sdata->streams.end();++it)
	{
		video::MeshStreamKey key=it->first;
		video::IHardwareStreamBuffer* buff=it->second;
		void*data=new byte[buff->getSizeInBytes()];
		buff->readData(0,buff->getSizeInBytes(),data);
		int baseSize=buff->getSizeInBytes();
		int baseLength=baseSize/buff->getElementSize();
		buff->SetLength(vCount);
		void*newData=buff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
		memcpy(newData,data,baseSize);
		int targetIdx=baseLength;
		//write redundant data
		for(int i=0;i<m_redundantVerts.size();++i,++targetIdx)
		{
		
			void* ptr=data;
			switch(buff->getElementData())
			{
			case video::ESDT_Point1f:
				((float*)newData)[targetIdx]=((float*)data)[m_redundantVerts[i].idx];
				break;
			case video::ESDT_Point2f:
				((math::vector2d*)newData)[targetIdx]=((math::vector2d*)data)[m_redundantVerts[i].idx];
				break;
			case video::ESDT_Point3f:
				((math::vector3d*)newData)[targetIdx]=((math::vector3d*)data)[m_redundantVerts[i].idx];
				break;
			case video::ESDT_Point4f:
				((math::vector4d*)newData)[targetIdx]=((math::vector4d*)data)[m_redundantVerts[i].idx];
				break;
			}
		}

		delete [] data;
		buff->unlock();
	}

	if(m_generateBinormal)
	{
		binStream=buffer->createStream(0,video::EMST_Binormal,
			video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
		bin=(math::vector3d*)binStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	}

	video::IHardwareStreamBuffer* tanStream=buffer->createStream(0,video::EMST_Tangent,
		video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);


	math::vector3d* tang=(math::vector3d*)tanStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	for(int i=0;i<m_baseVecs.size();++i)
	{
		tang[i]=m_baseVecs[i].U;
		if(m_generateBinormal)
			bin[i]=m_baseVecs[i].V;
	}

	int iCnt=id->indexCount;
	union{
		ushort*i16;
		uint*i32;
	}index;

	uint indTuple[3];
	//uint invertedIndTuple[3];

	void*idxData=(ushort*)ib->lock(id->firstIndex*ib->getIndexSize(),
		id->indexCount*ib->getIndexSize(),
		video::IHardwareIndexBuffer::ELO_ReadOnly);

	video::IHardwareIndexBuffer::EIndexType indexType=ib->getIndexType();

	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit)
		index.i16=(ushort*)idxData;
	else
		index.i32=(uint*)idxData;
	
	int currentIdx=0;
	if(false)
	for(int i=0;i<m_triBaseAssigment.size();++i,currentIdx+=3)
	{
		index.i16[currentIdx+0]=m_triBaseAssigment[i].p[0];
		index.i16[currentIdx+1]=m_triBaseAssigment[i].p[1];
		index.i16[currentIdx+2]=m_triBaseAssigment[i].p[2];
	}

	if(binStream)
		binStream->unlock();
	tanStream->unlock();
	ib->unlock();
}


uint TangentCalculater::_AddUV2Base(std::multimap<uint,uint,CBaseIndexOrder> &inMap, const uint indwPosNo, 
									const math::vector3d &inU, const math::vector3d &inV, const math::vector3d &inNormN )
{
	// no mesh is perfect
	//	assert(IsNormalized(inNormN));

	uint Indx;

	Indx=indwPosNo;

	std::multimap<uint,uint,CBaseIndexOrder>::iterator iFind,iFindEnd;

	iFind = inMap.lower_bound(Indx);

	assert(iFind!=inMap.end());

	math::vector3d vNormal=m_baseVecs[(*iFind).second].N;

	iFindEnd = inMap.upper_bound(Indx);

	uint dwBaseUVIndex=0xffffffff;													// init with not found

	bool bParity=(inU.crossProduct(inV).dotProduct(inNormN)>0.0f);

	for(;iFind!=iFindEnd;++iFind)
	{
		CBase33 &refFound=m_baseVecs[(*iFind).second];

		if(!math::isZero(refFound.U.LengthSQ())) 
		{
			bool bParityRef=(refFound.U.crossProduct(refFound.V).dotProduct(refFound.N)>0.0f);
			bool bParityCheck=(bParityRef==bParity);

			if(!bParityCheck)
				continue;

			math::vector3d vrefUV = refFound.U+refFound.V;

			// project down to the plane (plane normal = inNormN)
			math::vector3d vRotHalf = vrefUV - inNormN*(inNormN.dotProduct(vrefUV));

			//			bool bHalfAngleCheck=normalize(inU+inV) * normalize(vRotHalf) > cos(fThreshold);
			bool bHalfAngleCheck = (inU+inV) .dotProduct(vRotHalf) > 0.0f;		// length doesn't matter so normalize is not needed

			if(!bHalfAngleCheck)
				continue;
		}

		dwBaseUVIndex=(*iFind).second;
		break;
	}

	if(dwBaseUVIndex==0xffffffff)														// not found
	{	
		// otherwise create a new base

		CBase33 Base( math::vector3d(0,0,0), math::vector3d(0,0,0), vNormal );

		dwBaseUVIndex = m_baseVecs.size();

		m_redundantVerts.push_back(CRedundantVerts(Indx,dwBaseUVIndex) );
		inMap.insert( std::pair<uint,uint>(Indx,dwBaseUVIndex) );
		m_baseVecs.push_back(Base);
	}

	CBase33 &refBaseUV=m_baseVecs[dwBaseUVIndex];

	refBaseUV.U=refBaseUV.U+inU;
	refBaseUV.V=refBaseUV.V+inV;

	//no mesh is perfect 
	if(inU.x!=0.0f || inU.y!=0.0f || inU.z!=0.0f)
		assert(refBaseUV.U.x!=0.0f || refBaseUV.U.y!=0.0f || refBaseUV.U.z!=0.0f);
	// no mesh is perfect
	if(inV.x!=0.0f || inV.y!=0.0f || inV.z!=0.0f)
		assert(refBaseUV.V.x!=0.0f || refBaseUV.V.y!=0.0f || refBaseUV.V.z!=0.0f);

	return dwBaseUVIndex;
}
void TangentCalculater::_AddNormal2Base( std::multimap<uint,uint,CBaseIndexOrder> &inMap, const uint indwPosNo, const math::vector3d &inNormal )
{
	uint Indx;

	Indx=indwPosNo;

	std::multimap<uint,uint,CBaseIndexOrder>::iterator iFind = inMap.find(Indx);

	unsigned int dwBaseNIndex;

	if(iFind!=inMap.end())																// found
	{
		// reuse the existing one

		dwBaseNIndex=(*iFind).second;
	}
	else
	{
		// otherwise create a new base

		CBase33 Base( math::vector3d::Zero, math::vector3d::Zero,math::vector3d::Zero);

		dwBaseNIndex=m_baseVecs.size();
		inMap.insert( std::pair<uint,uint>(Indx,dwBaseNIndex) );
		m_baseVecs.push_back(Base);
	}

	CBase33 &refBaseN=m_baseVecs[dwBaseNIndex];

	refBaseN.N=refBaseN.N+inNormal;
}


void TangentCalculater::GetTriangleBaseInd(uint id,uint outInd[3])
{

	CTriBaseIndex &indx=m_triBaseAssigment[id];

	for(uint i=0;i<3;i++)
		outInd[i]=indx.p[i];
}
void TangentCalculater::GetBase(const uint indwPosNo ,math::vector3d&U,math::vector3d&V,math::vector3d&N)
{
	CBase33 &base=m_baseVecs[indwPosNo];

	U=base.U;
	V=base.V;
	N=base.N;
}

#if 0
void TangentCalculater::pass1(IMeshBuffer* buffer){


	video::IndexData* id=buffer->getIndexData();
	video::IHardwareIndexBuffer*ib=id->indexBuffer;
	if(!ib)
		return;


	video::IHardwareStreamBuffer* vStream=buffer->getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer* nStream=buffer->getStream(0,video::EMST_Normal);
	video::IHardwareStreamBuffer* uvStream=buffer->getStream(0,video::EMST_Texcoord);

// 	video::IHardwareStreamBuffer* tanStream=buffer->createStream(0,video::EMST_Tangent,video::ESDT_Point3f,vStream->length());
// 	video::IHardwareStreamBuffer* binStream=buffer->createStream(0,video::EMST_Binormal,video::ESDT_Point3f,vStream->length());

	video::IHardwareStreamBuffer* binStream=buffer->createStream(m_BinStream,video::EMST_Texcoord,
		video::ESDT_Point3f,vStream->length(),video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	video::IHardwareStreamBuffer* tanStream=buffer->createStream(m_TanStream,video::EMST_Texcoord,
		video::ESDT_Point3f,vStream->length(),video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
/**/

	math::vector3d* vtx=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector3d* norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector2d* tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);

// 	math::vector3d* tang=(math::vector3d*)tanStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
// 	math::vector3d* bin=(math::vector3d*)binStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);


	int vtCnt=vStream->length();
	m_tang1.resize(vtCnt);
	//m_tang2.resize(vtCnt);

	int iCnt=id->indexCount;
	union{
		ushort*i16;
		uint*i32;
	}index;

	uint indTuple[3];

	void*idxData=(ushort*)ib->lock(id->firstIndex*ib->getIndexSize(),
		id->indexCount*ib->getIndexSize(),
		video::IHardwareIndexBuffer::ELO_ReadOnly);

	video::IHardwareIndexBuffer::EIndexType indexType=ib->getIndexType();

	if(indexType==video::IHardwareIndexBuffer::EIT_16Bit){
		index.i16=(ushort*)idxData;
	}else
		index.i32=(uint*)idxData;

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
		calcTangent(vtx[indTuple[1]],vtx[indTuple[0]],vtx[indTuple[2]],
			tc[indTuple[1]],tc[indTuple[0]],tc[indTuple[2]],
			indTuple[0],indTuple[1],indTuple[2]);
// 		tang[b]=v1.Tangent;
// 		tang[a]=v2.Tangent;
// 		tang[c]=v3.Tangent;
// 
// 		bin[b]=v1.Binormal;
// 		bin[a]=v2.Binormal;
// 		bin[c]=v3.Binormal;
	}

	for(int i=0;i<m_tang1.size();++i)
		m_tang1[i].Normalize();

	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
// 	tanStream->unlock();
// 	binStream->unlock();

	ib->unlock();
	
}
void TangentCalculater::pass2(IMeshBuffer* buffer){
	video::IHardwareStreamBuffer* nStream=buffer->getStream(0,video::EMST_Normal);
// 	video::IHardwareStreamBuffer* binStream=buffer->getStream(0,video::EMST_Binormal);
// 	video::IHardwareStreamBuffer* tanStream=buffer->getStream(0,video::EMST_Tangent);
	video::IHardwareStreamBuffer* binStream=buffer->getStream(m_BinStream,video::EMST_Texcoord);
	video::IHardwareStreamBuffer* tanStream=buffer->getStream(m_TanStream,video::EMST_Texcoord);
	int vtCnt=nStream->length();
	math::vector3d* norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	math::vector3d* bin=(math::vector3d*)binStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d* tang=(math::vector3d*)tanStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	for(int i=0;i<vtCnt;++i){
		const math::vector3d& n = norm[i];
		const math::vector3d& t = m_tang1[i];

		// Gram-Schmidt orthogonalize
		tang[i] = (t - n * n.dotProduct(t)).Normalize();

		// Calculate Binormal
		bin[i]=tang[i].crossProduct(n);
		// Calculate handedness
		math::vector3d txb=tang[i].crossProduct(bin[i]);
		if(txb.dotProduct(n)<0){
			bin[i]*=-1;
			tang[i]*=-1;
		}
		//v[i].Tangent.w = (n.crossProduct(t).dotProduct(m_tang2[i])) < 0.0) ? -1.0 : 1.0;
	}
	nStream->unlock();
	tanStream->unlock();
	binStream->unlock();
}

void TangentCalculater::calcTangent(const math::vector3d&v1,const math::vector3d&v2,const math::vector3d&v3,
									const math::vector2d&tc1,const math::vector2d&tc2,const math::vector2d&tc3,int i1,int i2,int i3)
{

	math::vector3d sdir;
	math::vector3d vA=v2-v1;
	math::vector3d vB=v3-v1;

	float s1 = tc2.x - tc1.x;
	float s2 = tc3.x - tc1.x;
	float t1 = tc2.y - tc1.y;
	float t2 = tc3.y - tc1.y;

	float div=(s1 * t2 - s2 * t1);

	float r = 1.0f / div;
	sdir.x=(t2 * vA.x - t1 * vB.x) * r;
	sdir.y=(t2 * vA.y - t1 * vB.y) * r;
	sdir.z=(t2 * vA.z - t1 * vB.z) * r;

	m_tang1[i1] += sdir;
	m_tang1[i2] += sdir;
	m_tang1[i3] += sdir;

//	math::vector3d tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,(s1 * z2 - s2 * z1) * r);
// 	m_tang2[i1] += tdir;
// 	m_tang2[i2] += tdir;
// 	m_tang2[i3] += tdir;
}

#endif

}
}

