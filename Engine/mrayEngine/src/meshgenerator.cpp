#include "stdafx.h"

#include "MeshGenerator.h"
#include "ITexture.h"
#include "TextureResourceManager.h"
#include "IHardwareBufferManager.h"
#include "ShaderResourceManager.h"
#include "MaterialResourceManager.h"
#include "MeshBufferData.h"

#include "IMeshBuffer.h"
#include "mraySystem.h"

#include "ILogManager.h"
#include "RenderTechnique.h"
#include "RenderPass.h"


namespace mray{

namespace scene{

const mchar *GenTypesName[]={

	mT("genBox"),
	mT("genSphere"),
	mT("genCapsule"),
	mT("genTorus"),
	mT("genCylinder"),
	mT("genPlane"),
	mT("genSkydome"),
	mT("genSkyPlane"),
	mT("genSkybox"),
	mT("genTerrain"),
	mT("genBillboard"),
	mT("genBillboardBeam")
};

MeshGenerator::MeshGenerator(){
	mraySystem::memSet(m_genId,0,sizeof(m_genId));
}
MeshGenerator::~MeshGenerator(){

}

void MeshGenerator::setMeshName(SMesh*mesh,EGenTypes type){

	core::string name=GenTypesName[type];
	name+=mT("_");
	name+=m_genId[type];
	mesh->setResourceName(name);
	m_genId[type]++;
}


void MeshGenerator::generateBox(const math::vector3d&  extent, IMeshBuffer*buf, int startIndex, int startVertex)
{

	if(!buf)
		return;

	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,24,
			video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(24*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,24,
		video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(24*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,24,
		video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(24*vStream->getElementSize());
	} 

	video::IHardwareIndexBuffer* indBuff=buf->getIndexData()->indexBuffer;
	if(!indBuff){
		indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,36,video::IHardwareIndexBuffer::EUT_Static,true);
	}else{
		indBuff->resize(36*indBuff->getIndexSize());
	}

	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	vert[0]=math::vector3d(-extent.x,-extent.y,-extent.z);
	norm[0]=math::vector3d(0,0,-1);
	tc  [0]=math::vector2d(0,0);

	vert[1]=math::vector3d( extent.x,-extent.y,-extent.z);
	norm[1]=math::vector3d(0,0,-1);
	tc  [1]=math::vector2d(1,0);

	vert[2]=math::vector3d( extent.x, extent.y,-extent.z);
	norm[2]=math::vector3d(0,0,-1);
	tc  [2]=math::vector2d(1,1);

	vert[3]=math::vector3d(-extent.x, extent.y,-extent.z);
	norm[3]=math::vector3d(0,0,-1);
	tc  [3]=math::vector2d(0,1);

//
	vert[4]=math::vector3d( extent.x,-extent.y, extent.z);
	norm[4]=math::vector3d(0,0,1);
	tc  [4]=math::vector2d(0,0);

	vert[5]=math::vector3d(-extent.x,-extent.y, extent.z);
	norm[5]=math::vector3d(0,0,1);
	tc  [5]=math::vector2d(1,0);

	vert[6]=math::vector3d(-extent.x, extent.y, extent.z);
	norm[6]=math::vector3d(0,0,1);
	tc  [6]=math::vector2d(1,1);

	vert[7]=math::vector3d( extent.x, extent.y, extent.z);
	norm[7]=math::vector3d(0,0,1);
	tc  [7]=math::vector2d(0,1);

//
	vert[8]=math::vector3d(-extent.x,-extent.y, extent.z);
	norm[8]=math::vector3d(-1,0,0);
	tc  [8]=math::vector2d(0,0);

	vert[9]=math::vector3d(-extent.x,-extent.y,-extent.z);
	norm[9]=math::vector3d(-1,0,0);
	tc  [9]=math::vector2d(1,0);

	vert[10]=math::vector3d(-extent.x, extent.y,-extent.z);
	norm[10]=math::vector3d(-1,0,0);
	tc  [10]=math::vector2d(1,1);

	vert[11]=math::vector3d(-extent.x, extent.y, extent.z);
	norm[11]=math::vector3d(-1,0,0);
	tc  [11]=math::vector2d(0,1);

	//
	vert[12]=math::vector3d( extent.x,-extent.y,-extent.z);
	norm[12]=math::vector3d( 1,0,0);
	tc  [12]=math::vector2d(0,0);

	vert[13]=math::vector3d( extent.x,-extent.y, extent.z);
	norm[13]=math::vector3d( 1,0,0);
	tc  [13]=math::vector2d(1,0);

	vert[14]=math::vector3d( extent.x, extent.y, extent.z);
	norm[14]=math::vector3d( 1,0,0);
	tc  [14]=math::vector2d(1,1);

	vert[15]=math::vector3d( extent.x, extent.y,-extent.z);
	norm[15]=math::vector3d( 1,0,0);
	tc  [15]=math::vector2d(0,1);

	//
	vert[16]=math::vector3d(-extent.x,-extent.y, extent.z);
	norm[16]=math::vector3d( 0,-1,0);
	tc  [16]=math::vector2d(0,0);

	vert[17]=math::vector3d( extent.x,-extent.y, extent.z);
	norm[17]=math::vector3d( 0,-1,0);
	tc  [17]=math::vector2d(1,0);

	vert[18]=math::vector3d( extent.x,-extent.y,-extent.z);
	norm[18]=math::vector3d( 0,-1,0);
	tc  [18]=math::vector2d(1,1);

	vert[19]=math::vector3d(-extent.x,-extent.y,-extent.z);
	norm[19]=math::vector3d( 0,-1,0);
	tc  [19]=math::vector2d(0,1);

	//
	vert[20]=math::vector3d( extent.x, extent.y, extent.z);
	norm[20]=math::vector3d( 0, 1,0);
	tc  [20]=math::vector2d(0,0);

	vert[21]=math::vector3d(-extent.x, extent.y, extent.z);
	norm[21]=math::vector3d( 0, 1,0);
	tc  [21]=math::vector2d(1,0);

	vert[22]=math::vector3d(-extent.x, extent.y,-extent.z);
	norm[22]=math::vector3d( 0, 1,0);
	tc  [22]=math::vector2d(1,1);

	vert[23]=math::vector3d( extent.x, extent.y,-extent.z);
	norm[23]=math::vector3d( 0, 1,0);
	tc  [23]=math::vector2d(0,1);

	//	for(int i=0;i<24;++i)ind[i]=i;


	ushort*ind=(ushort*)indBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	for(int i=0;i<6;++i){
		ind[6*i+0]=4*i;
		ind[6*i+1]=4*i+2;
		ind[6*i+2]=4*i+1;

		ind[6*i+3]=4*i;
		ind[6*i+4]=4*i+3;
		ind[6*i+5]=4*i+2;
	}

	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	indBuff->unlock();

	buf->setRenderType(video::MR_TRIANGLES);
	math::box3d b;
	b.reset(-extent);
	b.AddPoint(extent);

	buf->setBoundingBox(b);
	//mesh->setBoundingBox(b);

	//setMeshName(mesh,EGT_Box);
}
void MeshGenerator::generateSphere(float radius, int uSlices, int vSlices, IMeshBuffer*buf, int startIndex, int startVertex){

	if(!buf)
		return;

	if(uSlices<3)uSlices=3;
	if(vSlices<3)vSlices=3;

	if(uSlices>100)uSlices=100;
	if(vSlices>100)vSlices=100;

	uint NumberOfVertices=(uSlices+1)*(vSlices+1);
	uint NumberOfTri=2*(uSlices)*(vSlices+1);
//	buf->setOrginalUVCount(NumberOfVertices);

	video::IHardwareStreamBuffer::EUsageType usage=video::IHardwareStreamBuffer::EUT_Dynamic;

	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,NumberOfVertices,
		usage,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(NumberOfVertices*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,NumberOfVertices,
		usage,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(NumberOfVertices*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,NumberOfVertices,
		usage,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(NumberOfVertices*vStream->getElementSize());
	} 


	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);


	math::vector3d vec;
	float uStep=math::PI32/(float)uSlices;
	//uStep=toRad(uStep);
	float vStep=2*math::PI32/(float)vSlices;
	//vStep=toRad(vStep);

	//calc vertecis position
	int currVert=0;
//	math::vector2d*  uv=buf->getOrginalUV();
	m_tmpIndiciesUSHORT.resize(0);

	float angleY=0,angleX=0;
	for(int j=0;j<=uSlices;j++)
	{
		double sinAY=math::sinr(angleY);
		float r0 = radius * sinAY;
		float y0 = radius * math::cosr (angleY);
		//float ty=(float)(angleY/math::PI64);
		float t=(float)j/(float)(uSlices-1);
		angleX=0;
		for(int i=0;i<=vSlices;++i)
		{
			vec.x = r0 * math::sinr(angleX);
			vec.y = y0;
			vec.z = r0 * math::cosr(angleX);

			float s=(float)i/(float)(vSlices-1);


			*vert=vec;
			math::Point3d<float> normal(vec);
			normal.Normalize();
			*norm=normal;

			math::vector2d uvCrd;
/*
			float tu=0.5;
			if (j==0)
			{
				if (sinAY!=0)
					tu = (float)(acos(math::clamp(normal.x/sinAY, -1.0, 1.0)) * 0.5 /math::PI64);
				if (normal.z < 0.0f)
					tu=1-tu;
			}
			else
				tu = buf->UVCoords[currVert-(vSlices+1)].x;
*/
			uvCrd.x=t;//(float)i/(float)vSlices();
			uvCrd.y=s;//(float)j/(float)uSlices();
			*tc=uvCrd;

			if(j!=uSlices){
				//calc indices
				math::Point3di ind;
				ind.x=currVert+vSlices+1;
				ind.y=currVert;
				ind.z=currVert+vSlices;
				m_tmpIndiciesUSHORT.push_back(ind.x);
				m_tmpIndiciesUSHORT.push_back(ind.y);
				m_tmpIndiciesUSHORT.push_back(ind.z);

				///		ind.x=i*uSlices+j;  //same as above
				ind.y=currVert+1;
				ind.z=currVert;
				m_tmpIndiciesUSHORT.push_back(ind.x);
				m_tmpIndiciesUSHORT.push_back(ind.y);
				m_tmpIndiciesUSHORT.push_back(ind.z);
				currVert++;
			}
			vert++;
			tc++;
			norm++;
			angleX+=vStep;
		}
		angleY+=uStep;
	}
	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();


	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);

	buf->setBoundingBox(math::box3d(-radius,-radius,-radius,
		radius,radius,radius));
	/*
	mesh->setBoundingBox(math::box3d(-radius,-radius,-radius,
		radius,radius,radius));
	setMeshName(mesh,EGT_Sphere);*/
}
void MeshGenerator::generateCapsule(float radius, float height, int segments, IMeshBuffer*buf, int startIndex, int startVertex){

	if(!buf)
		return;
	if(segments<4)segments=4;
	if(segments>200)segments=200;

	//fix segments to be dividable by 4
	segments+=segments%4;

	//+2 + (segments*segments)/2
	int vCount=(segments*2 + segments*segments*0.5 + 2);//plus 2 center points


	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,
		video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,
		video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,
		video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);



	int n=0;
	float step=math::PI32*2.0f/(float)segments;
	float sf=sin(step);
	float cf=cos(step);

	float x=0;
	float y=-height*0.5f;
	float z=1;
	float tmp=0;

	//side
	for(int j=0;j<2;j++){
		for(int i=0;i<segments;++i){
			vert[n].x = x*radius;
			vert[n].y = y;
			vert[n].z = z*radius;

			norm[n].set(x,0,z);

			//TODO:calc texture coord
			//tc[n];

			tmp=cf*x - sf*z;
			z=sf*x + cf*z;
			x=tmp;
			n++;

		}
		y+=height;
	}


	float sx=0;
	float sz=1;

	float step2=360 / (float)segments;
	float sf2=math::sind(step2);
	float cf2=math::cosd(step2);

	y=0;
	int voffset=segments*segments*0.25;
	for(int j=0;j<segments*0.25;j++){

		x=0;
		y=sx;
		z=sz;

		for(int i=0;i<segments;++i){
			//top cap
			vert[n].x=x*radius;
			vert[n].y=y*radius + height*0.5;
			vert[n].z=z*radius;
			norm[n]=math::vector3d(x,y,z);


			//bottom cap
			vert[n+voffset]=vert[n];
			norm[n+voffset]=norm[n];
			vert[n+voffset].y*=-1;
			norm[n+voffset].y*=-1;
			n++;


			tmp=cf*z - sf*x;
			x  =sf*z + cf*x;
			z  =tmp;

		}
		tmp= cf*sz - sf*sx;
		sx = sf*sz + cf*sx;
		sz=tmp;

	}

	n+=voffset;

	int ti=n;
	//add center points
	vert[n]=math::vector3d(0,radius+height*0.5f,0);
	norm[n]=math::vector3d(0,1,0);
	n++;
	int bi=n;
	vert[n]=math::vector3d(0,-radius-height*0.5f,0);
	norm[n]=math::vector3d(0, -1,0);


	//for each cap there is 3*segments index,and for the side there is 2*3*segments index
	m_tmpIndiciesUSHORT.resize(segments*6 + 2*6*segments*(segments*0.25-1) + 2*3*segments);

	ushort*ind=&m_tmpIndiciesUSHORT[0];

	//sides indices
	n=0;
	int j=0;
	int ioffset=3*segments+6*segments*(segments*0.25 - 1);
	for(int i=0;i<segments;++i){
		ind[n++]=i;
		ind[n++]=segments+i;
		if(i==segments-1)
			ind[n++]=segments;
		else
			ind[n++]=segments+i+1;

		ind[n++]=i;
		if(i==segments-1){
			ind[n++]=segments;
			ind[n++]=0;
		}
		else{
			ind[n++]=segments+i+1;
			ind[n++]=i+1;
		}
	}

	//top cap
	for(j=0;j<segments*0.25 - 1;j++){
		for(int i=0;i<segments;++i){
			ind[n]=i + (2+j)*segments;
			ind[ioffset+n]=i+1 + (2+j)*segments + voffset;
			n++;
			ind[ioffset+n]=i + (2+j)*segments + voffset;
			if(i==segments-1){
				ind[n]= (2+j)*segments;
				n++;
			}
			else{
				ind[n]=segments+i+1 + (2+j)*segments;;
				n++;
			}
			if(i==segments-1){
				ind[ioffset+n]= (2+j)*segments + voffset;
			}
			else{
				ind[ioffset+n]=segments+i+1 + (2+j)*segments + voffset;;
			}
			ind[n]=i+1 + (2+j)*segments;
			n++;

			ind[n]=i + (2+j)*segments;
			ind[n]=i + (2+j + 1)*segments;
			n++;
			ind[ioffset+n]=i + (2+j + 1)*segments + voffset;

			ind[ioffset+n]=i + (2+j)*segments + voffset;
			n++;
			if(i==segments-1){
				ind[ioffset+n]=segments+ (2+j)*segments + voffset;
				ind[n]=segments+ (2+j)*segments;
				n++;
			}
			else{
				ind[ioffset+n]=segments+i+1 + (2+j)*segments + voffset;;
				ind[n]=segments+i+1 + (2+j)*segments;;
				n++;
			}
		}
	}
	j=segments*0.25 - 1;
	//connect top and bottom points
	for(int i=0;i<segments;++i){		
		ind[n]=i + (2+j)*segments;
		ind[n]=ti;
		n++;
		ind[ioffset+n]=i + (2+j)*segments + voffset;

		ind[ioffset+n]=bi;
		n++;

		if(i==segments-1){
			ind[ioffset+n]= (2+j)*segments+voffset;
			ind[n]= (2+j)*segments;
			n++;
		}
		else{
			ind[ioffset+n]=i+1 + (2+j)*segments+voffset;
			ind[n]=i+1 + (2+j)*segments;;
			n++;
		}
	}
	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);

	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	buf->setRenderType(video::MR_TRIANGLES);
	math::box3d b;
	b.reset(math::vector3d(-radius,-height*0.5-radius,-radius));
	b.AddPoint(math::vector3d(radius,height*0.5+radius,radius));
	buf->setBoundingBox(b);
	/*
	mesh->setBoundingBox(b);

	setMeshName(mesh,EGT_Capsule);*/
}
void MeshGenerator::generateTorus(float radius1, float radius2, int segments, int sides, IMeshBuffer*buf, int startIndex, int startVertex)
{

	if(!buf)
		return;

	if(segments<3)segments=3;
	if(segments>200)segments=200;
	if(sides<3)sides=3;
	if(sides>200)sides=200;


	int vCount=((segments+1)*(sides+1));

	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,
		vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);



	int n=0;
	float step=360.0f/(float)segments;
	float step2=360.0f/(float)sides;
	float sf=math::sind(step);
	float cf=math::cosd(step);

	float sf2=math::sind(step2);
	float cf2=math::cosd(step2);

	float x2,y2,z2;
	for(int j=0;j<segments+1;j++){
		sf=math::sind(j*step);
		cf=math::cosd(j*step);
		for(int i=0;i<sides+1;++i){
			x2=cf*(radius1+radius2*math::cosd(i*step2));
			y2=              radius2*math::sind(i*step2);
			z2=sf*(radius1+radius2*math::cosd(i*step2));
			vert[n].x = x2;
			vert[n].y = y2;
			vert[n].z = z2;

			norm[n]=vert[n];
			norm[n].Normalize();

			//TODO:calc texture coord
			tc[n].x=(float)j/(float)(segments);
			tc[n].y=(float)i/(float)(sides);
			n++;

		}
	}


	m_tmpIndiciesUSHORT.resize((segments+1)*(sides+1)*6);

	ushort*ind=&m_tmpIndiciesUSHORT[0];
	int k=n;
	//sides indices
	n=0;
	for(int j=0;j<segments+1;j++){
		for(int i=0;i<sides+1;++i){
			ind[n++]=i+(sides+1)*j;
			if(i==sides)
				ind[n++]=(sides+1)*j;
			else
				ind[n++]=i+1+(sides+1)*j;
			if(j==segments){
				if(i==sides)
					ind[n++]=0;
				else
					ind[n++]=i+1;
			}
			else{
				if(i==sides)
					ind[n++]=(sides+1)*(j+1);
				else
					ind[n++]=i+1+(sides+1)*(j+1);
			}

			ind[n++]=i+(sides+1)*j;
			if(j==segments){

				if(i==sides)
					ind[n++]=0;
				else
					ind[n++]=i+1;
				ind[n++]=i;
			}
			else{

				if(i==sides)
					ind[n++]=(sides+1)*(j+1);
				else
					ind[n++]=i+1+(sides+1)*(j+1);
				ind[n++]=i+(sides+1)*(j+1);
			}
		}
	}
	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);

	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	math::box3d b;
	buf->setRenderType(video::MR_TRIANGLES);
	b.reset(math::vector3d(-(radius1+radius2),-radius2,-(radius1+radius2)));
	b.AddPoint(math::vector3d((radius1+radius2),radius2,(radius1+radius2)));
	buf->setBoundingBox(b);
	/*
	mesh->setBoundingBox(b);

	setMeshName(mesh,EGT_Torus);*/
}
void MeshGenerator::generateCylinder(float radius, float height, int segments, IMeshBuffer*buf, int startIndex, int startVertex)
{

	if(!buf)
		return;


	if(segments<3)segments=3;
	if(segments>200)segments=200;


	int vCount=(segments*2+2);//plus 2 center points


	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	int n=0;
	float step=360.0f/(float)segments;
	float y=-height*0.5f;
	float sf=math::sind(step);
	float cf=math::cosd(step);

	float x=0;
	float z=1;
	for(int j=0;j<2;j++){
		for(int i=0;i<segments;++i){
			vert[n].x = x*radius;
			vert[n].y = y;
			vert[n].z = z*radius;

			norm[n].set(x,0,z);

			//TODO:calc texture coord
			//tc[n];

			float tmp=cf*x - sf*z;
			z=sf*x + cf*z;
			x=tmp;
			n++;

		}
		y+=height;
	}

	int bi=n;
	//add center points
	vert[n]=math::vector3d(0,-height*0.5f,0);
	norm[n]=math::vector3d(0,-1,0);
	n++;
	int ti=n;
	vert[n]=math::vector3d(0,height*0.5f,0);
	norm[n]=math::vector3d(0, 1,0);

	//for each cap there is 3*segments index,and for the side there is 2*3*segments index
	m_tmpIndiciesUSHORT.resize(segments*12);

	ushort*ind=&m_tmpIndiciesUSHORT[0];
	//sides indices
	n=0;
	for(int i=0;i<segments;++i){
		ind[n++]=i;
		ind[n++]=segments+i;
		if(i==segments-1)
			ind[n++]=segments;
		else
			ind[n++]=segments+i+1;

		ind[n++]=i;
		if(i==segments-1){
			ind[n++]=segments;
			ind[n++]=0;
		}
		else{
			ind[n++]=segments+i+1;
			ind[n++]=i+1;
		}
	}

	//top cap
	for(int i=0;i<segments;++i){
		ind[n++]=segments+i;
		ind[n++]=ti;
		if(i==segments-1)
			ind[n++]=segments;
		else
			ind[n++]=segments+i+1;
	}

	//bottom cap
	for(int i=0;i<segments;++i){
		ind[n++]=i;
		if(i==segments-1)
			ind[n++]=0;
		else
			ind[n++]=i+1;
		ind[n++]=bi;
	}
	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);


	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	math::box3d b;
	buf->setRenderType(video::MR_TRIANGLES);
	b.reset(math::vector3d(-radius*0.5,-height*0.5,-radius*0.5));
	b.AddPoint(math::vector3d(radius*0.5,height*0.5,radius*0.5));
	buf->setBoundingBox(b);
	/*
	mesh->setBoundingBox(b);

	setMeshName(mesh,EGT_Cylinder);*/
}
void MeshGenerator::generatePlane(int widthSegment, int lengthSegment, IMeshBuffer*buf, int startIndex, int startVertex)
{

	if(!buf)
		return;
	if(lengthSegment==0)lengthSegment=1;
	if(widthSegment==0)widthSegment=1;

	int vCount=(widthSegment+1)*(lengthSegment+1);
	int indCount=(widthSegment)*(lengthSegment)*2*3;

	video::IHardwareStreamBuffer* vStream= buf->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buf->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buf->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buf->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buf->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buf->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

//	buf->setOrginalUVCount(vCount);
	//	mBuffer->UVCoords.resize(vtxCount);

	float xPos,yPos;
	int n=0;
	//  -0.5 --> 0.5
	//   |
	//   0.5

//	math::vector2df *uv=buf->getOrginalUV();

	for(int i=0;i<=lengthSegment;++i){
		yPos=((float)i/(float)(lengthSegment));
		for(int j=0;j<=widthSegment;j++){
			xPos = ((float)j / (float)(widthSegment)) ;
			vert[n]=math::vector3d(xPos-0.5f,0,yPos- 0.5f);
			norm[n]=math::vector3d(0,1,0);
			tc[n]=math::vector2d(xPos,yPos);
			n++;
		}
	}

	m_tmpIndiciesUSHORT.resize(indCount);

	ushort*ind=&m_tmpIndiciesUSHORT[0];
	n=0;
	for(int i=0;i<lengthSegment;++i){
		for(int j=0;j<widthSegment;j++){
			ind[n++]=i*(widthSegment+1)+j;		//0
			ind[n++]=(i+1)*(widthSegment+1)+j;	//2
			ind[n++]=i*(widthSegment+1)+j+1;	//1

			ind[n++]=(i+1)*(widthSegment+1)+j;		//3
			ind[n++]=(i+1)*(widthSegment+1)+j+1;	//5
			ind[n++]=i*(widthSegment+1)+j+1;		//4
		}
	}
	video::IHardwareIndexBuffer*indBuff=buf->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);



	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	math::box3d b;
	b.MinP.set(-0.5,0,-0.5);
	b.MaxP.set(0.5,0,0.5);
	buf->setBoundingBox(b);

	buf->setRenderType(video::MR_TRIANGLES);
//	setMeshName(mesh,EGT_Plane);
}

void MeshGenerator::generateSkyDome(video::ITexture*tex,int uSegments,int vSegments,
									double texPer,double spherePer,float radius,SMesh*skyMesh)
{


	uint curr=0;
	double azimuth_step;
	double elevation;
	double elevation_step;
	double azimuth;

	MeshBufferData*meshData=skyMesh->getBufferData(0);
	if(!meshData)
		meshData=skyMesh->addNewBuffer();

	IMeshBuffer*mesh=meshData->getMeshBuffer();

	meshData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);
	meshData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setRenderState(video::RS_CullFace,video::ES_DontUse);
	meshData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);

	azimuth_step=4.0*math::PI64/(double)vSegments;

	if(spherePer<0)
		spherePer=-spherePer;
	if(spherePer>2)
		spherePer=2;

	elevation_step=spherePer*math::PI64/(2.0*(double)uSegments);

	uint NumberOfVertices=(uSegments+1)*(vSegments+1);


	video::IHardwareStreamBuffer* vStream= mesh->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=mesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,NumberOfVertices,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(NumberOfVertices*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= mesh->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=mesh->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,NumberOfVertices,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(NumberOfVertices*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	uint NumberOfTri=(uSegments)*vSegments*6;

	curr=0;
	azimuth=0;
	math::vector2d coord;
	for(uint i=0;i<=vSegments;++i)
	{
		elevation=math::PI64/2;
		for(uint j=0;j<=uSegments;j++)
		{
			vert[curr].set((float)(radius*cos(elevation)*sin(azimuth)),
				(float)(radius*sin(elevation)),
				(float)(radius*cos(elevation)*cos(azimuth)));

			tc[curr].x=(float)i/(float)vSegments*(float)texPer;
			tc[curr].y=(float)j/(float)uSegments*(float)texPer;
			//	mesh->UVCoords.push_back(coord);

			elevation-=elevation_step;
			++curr;
		}
		azimuth+=azimuth_step;
	}

	m_tmpIndiciesUSHORT.resize(NumberOfTri);

	ushort*ind=&m_tmpIndiciesUSHORT[0];
	curr=0;
	for(uint i=0;i<vSegments;++i)
	{

		for(uint j=0;j<uSegments;j++)
		{
			ind[curr++]=uSegments+2+(uSegments+1)*i+j;
			ind[curr++]=(uSegments+1)*i+j;
			ind[curr++]=1+(uSegments+1)*i+j;

			ind[curr++]=uSegments+1+(uSegments+1)*i+j;
			ind[curr++]=(uSegments+1)*i+j;
			ind[curr++]=uSegments+2+(uSegments+1)*i+j;
		}
	}
	video::IHardwareIndexBuffer*indBuff=mesh->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);

	vStream->unlock();
	uvStream->unlock();

	setMeshName(skyMesh,EGT_Skydome);
}
void MeshGenerator::generateSkyPlane(video::ITexture*tex,int uSegments,int vSegments,
									 double texPercentage,float hight,float size,SMesh*skyMesh)
{
	if(uSegments==0)uSegments=1;
	if(vSegments==0)vSegments=1;

	int vCount=(uSegments+1)*(vSegments+1);
	int indCount=(uSegments)*(vSegments)*2*3;




	MeshBufferData*bufferData=skyMesh->getBufferData(0);
	if(!bufferData)
		bufferData=skyMesh->addNewBuffer();
	IMeshBuffer*mBuffer=bufferData->getMeshBuffer();

	bufferData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setRenderState(video::RS_Lighting,video::ES_DontUse);
	bufferData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setRenderState(video::RS_CullFace,video::ES_DontUse);
	bufferData->getMaterial()->GetActiveTechnique()->GetPassAt(0)->setTexture(tex,0);



	video::IHardwareStreamBuffer* vStream= mBuffer->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=mBuffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= mBuffer->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=mBuffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_Static,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	//	mBuffer->UVCoords.resize(vtxCount);

	float xPos,zPos;
	int n=0;
	//  -0.5 --> 0.5
	//   |
	//   0.5

	//	math::vector2d *uv=mBuffer->UVCoords.pointer();

	for(int i=0;i<=uSegments;++i){
		zPos=((float)i/(float)uSegments)-0.5f;
		for(int j=0;j<=vSegments;j++){
			xPos=((float)j/(float)vSegments)-0.5f;
			float yPos=hight*math::sinr(math::PI32*(float)i/(float)uSegments)*
				math::sinr(math::PI32*(float)j/(float)vSegments)-30;
			vert[n]=math::vector3d(xPos*size,yPos,zPos*size);
			tc[n]=math::vector2d((xPos+0.5f)*texPercentage,(zPos+0.5f)*texPercentage);
			n++;
		}
	}

	m_tmpIndiciesUSHORT.resize(3*indCount);

	ushort*ind=&m_tmpIndiciesUSHORT[0];
	n=0;
	for(int i=0;i<uSegments;++i){
		for(int j=0;j<vSegments;j++){
			ind[n++]=i*(vSegments+1)+j;		//0
			ind[n++]=i*(vSegments+1)+j+1;	//2
			ind[n++]=(i+1)*(vSegments+1)+j;	//1

			ind[n++]=(i+1)*(vSegments+1)+j;		//3
			ind[n++]=i*(vSegments+1)+j+1;		//5
			ind[n++]=(i+1)*(vSegments+1)+j+1;	//4
		}
	}

	video::IHardwareIndexBuffer*indBuff=mBuffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,m_tmpIndiciesUSHORT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);

	vStream->unlock();
	uvStream->unlock();
	math::box3d b;

	b.MinP.set(-size/2.0f,0,-size/2.0f);
	b.MaxP.set(size/2.0f,hight,size/2.0f);

	mBuffer->setBoundingBox(b);

	mBuffer->setRenderType(video::MR_TRIANGLES);

	setMeshName(skyMesh,EGT_SkyPlane);
}
void MeshGenerator::generateSkyBox(SMesh*skyMesh)
{



	math::vector3d*vert;

	video::IHardwareStreamBuffer* vStream;

	m_tmpIndiciesUSHORT.resize(24);
	m_tmpIndiciesUSHORT[0]=0;
	m_tmpIndiciesUSHORT[1]=3;
	m_tmpIndiciesUSHORT[2]=2;
	m_tmpIndiciesUSHORT[3]=1;

	m_tmpIndiciesUSHORT[4]=4;
	m_tmpIndiciesUSHORT[5]=7;
	m_tmpIndiciesUSHORT[6]=6;
	m_tmpIndiciesUSHORT[7]=5;

	m_tmpIndiciesUSHORT[8]=8;
	m_tmpIndiciesUSHORT[9]=11;
	m_tmpIndiciesUSHORT[10]=10;
	m_tmpIndiciesUSHORT[11]=9;

	m_tmpIndiciesUSHORT[12]=12;
	m_tmpIndiciesUSHORT[13]=15;
	m_tmpIndiciesUSHORT[14]=14;
	m_tmpIndiciesUSHORT[15]=13;

	m_tmpIndiciesUSHORT[16]=16;
	m_tmpIndiciesUSHORT[17]=19;
	m_tmpIndiciesUSHORT[18]=18;
	m_tmpIndiciesUSHORT[19]=17;

	m_tmpIndiciesUSHORT[20]=20;
	m_tmpIndiciesUSHORT[21]=23;
	m_tmpIndiciesUSHORT[22]=22;
	m_tmpIndiciesUSHORT[23]=21;

	MeshBufferData*mbd=skyMesh->getBufferData(0);
	if(!mbd)
		mbd=skyMesh->addNewBuffer();
	IMeshBuffer*mb=mbd->getMeshBuffer();

	video::RenderMaterial*mat = gMaterialResourceManager.getMaterial(mT("skybox"));
	video::RenderPass*p=0;
	if(!mat)
	{
		mat=new video::RenderMaterial();
		p= mat->CreateTechnique(mT(""))->CreatePass(mT(""));
		p->SetDiffuse(1);
		p->SetAmbient(1);
		p->SetSpecular(0);
	//	mat->setRenderShader=gShaderResourceManager.loadShader("skybox.cg","skybox.cg",mT("cg"));
		p->setRenderState(video::RS_CullFace,video::ES_DontUse);
		p->setRenderState(video::RS_Lighting,video::ES_DontUse);
		p->setRenderState(video::RS_Smooth,video::ES_DontUse);
		p->setRenderState(video::RS_ZTest,video::ES_DontUse);
		gMaterialResourceManager.addResource(mat,mT("skybox"));
	}else
		p=mat->GetActiveTechnique()->GetPassAt(0);

	mbd->setMaterial(mat);
/*
	video::RenderMaterial* mat=mbd->getMaterial();
	mat->diffuseColor=255;
	mat->ambientColor=255;
	mat->specularColor=0;
	mat->setRenderShader=gShaderResourceManager.loadShader("skybox.cg","skybox.cg",mT("cg"));
	mat->setRenderState(video::RS_CullFace,video::ES_DontUse);
	mat->setRenderState(video::RS_Lighting,video::ES_DontUse);
	mat->setRenderState(video::RS_Smooth,video::ES_DontUse);
	mat->setRenderState(video::RS_ZTest,video::ES_DontUse);
*/

	video::IHardwareIndexBuffer*indBuff=mb->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,24,video::IHardwareBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUSHORT[0]);


	 vStream= mb->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=mb->createStream(0,video::EMST_Position,video::ESDT_Point3f,24,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(4*vStream->getElementSize());
	}
	vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	mb->setRenderType(video::MR_QUADS);


	/*
	float l=100;
	float u=0;
	float v=1;
	video::ITexture* tex=0;
	if(front ){
		tex=front;
	}
	if(back  ){
		tex=back;
	}
	if(left  ){
		tex=left;
	}
	if(right ){
		tex=right;
	}
	if(top   ){
		tex=top;
	}
	if(bottom){
		tex=bottom;
	}
	if(tex)
	{
		float dim=1;
		if(tex->getType()==video::ETT_2DTex)
			dim=((video::ITexture* )tex)->getSize().x;
		if(tex->getType()==video::ETT_CubeTex)
			dim=((video::ITexture*)tex)->getSize(video::ECF_X_Positive).x;
		float pixel=1.0f/(dim*1.5f);
		u=pixel;
		v=1-pixel;
	}*/

	//u==t
	//v==o

	int hi=1;
	int low=-1;
	int l=1;

	vert[0]=math::vector3d(-l, low,-l);
	vert[1]=math::vector3d( l, low,-l);
	vert[2]=math::vector3d( l, hi,-l);
	vert[3]=math::vector3d(-l, hi,-l);

	vert[4]=math::vector3d( l, low, l);
	vert[5]=math::vector3d(-l, low, l);
	vert[6]=math::vector3d(-l, hi, l);
	vert[7]=math::vector3d( l, hi, l);

	vert[8]=math::vector3d( l, low,-l);
	vert[9]=math::vector3d( l, low, l);
	vert[10]=math::vector3d( l, hi, l);
	vert[11]=math::vector3d( l, hi,-l);

	vert[12]=math::vector3d(-l, low, l);
	vert[13]=math::vector3d(-l, low,-l);
	vert[14]=math::vector3d(-l, hi,-l);
	vert[15]=math::vector3d(-l, hi, l);

	vert[16]=math::vector3d( l, hi,-l);
	vert[17]=math::vector3d( l, hi, l);
	vert[18]=math::vector3d(-l, hi, l);
	vert[19]=math::vector3d(-l, hi,-l);

	vert[20]=math::vector3d(-l, low,-l);
	vert[21]=math::vector3d(-l, low, l);
	vert[22]=math::vector3d( l, low, l);
	vert[23]=math::vector3d( l, low,-l);

	vStream->unlock();
	setMeshName(skyMesh,EGT_Skybox);
}

void MeshGenerator::generateTerrain(video::ITexture*heightMap,int patchSize,SMesh*terrMesh){


	//	for(int lod=0;lod<maxLOD;lod++){

	video::IHardwarePixelBuffer*pbuffer= heightMap->getSurface();
	if(!pbuffer){
		gLogManager.log(mT("MeshGenerator::generateTerrain() - there's no pixel buffer in the height map!"),ELL_WARNING);
		return ;
	}

	float terrinSize=128;

	int meshSize=heightMap->getSize().x/patchSize;

	int vertPerEdge=meshSize;

	IMeshBuffer* buffer=terrMesh->getBuffer(0);
	if(!buffer)
		buffer=terrMesh->addNewBuffer()->getMeshBuffer();

	int vCount=(meshSize*meshSize);

	video::IHardwareStreamBuffer* vStream= buffer->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= buffer->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* uvStream= buffer->getStream(0,video::EMST_Texcoord);
	if(!uvStream) 
		uvStream=buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		uvStream->setElementType(video::ESDT_Point2f);
		uvStream->resize(vCount*vStream->getElementSize());
	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm=(math::vector3d*)nStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc=(math::vector2d*)uvStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

//	buffer->setOrginalUVCount(meshSize*meshSize);
	uint NumberOfVertices=meshSize*meshSize;
	uint NumberOfTri=2*(meshSize-1)*(meshSize-1);



	math::vector3d scale=math::vector3d(terrinSize*(int)patchSize
		/(float)heightMap->getSize().x,1,terrinSize*(int)patchSize
		/(float)heightMap->getSize().y);/**/
	int ver=0,mapVer=0; 
	//creating Vertex Array 

//	math::vector2d*uv=buffer->getOrginalUV();

	int y;
	math::vector2d uvCrd=0;


	math::box3d box(0,0,0,pbuffer->getWidth(),pbuffer->getHeight(),1);

	const video::LockedPixelBox &lockedBox=pbuffer->lock(box,video::IHardwareBuffer::ELO_ReadOnly);
	uchar*data=(uchar*)lockedBox.data;

	float maxY=data[0];
	float minY=data[0];
	for(int i=0;i<meshSize;++i){
		for(int j=0;j<meshSize;j++)
		{
			mapVer=(i*patchSize)*heightMap->getSize().x+j*patchSize;
			y=data[mapVer];
			if(y>maxY)maxY=y;
			if(y<minY)minY=y;
			ver=i*meshSize+j;

			vert[ver].x=(i )- terrinSize*0.5f;
			vert[ver].z=(j) - terrinSize*0.5f;
			vert[ver].y=(float)y*scale.y/128.0f;//rescale by divide with 128

			norm[ver].x=0;
			norm[ver].y=1;
			norm[ver].z=0;

			tc[ver].x=-(float)i*patchSize/(float)(meshSize);
			tc[ver].y=(float)j*patchSize/(float)(meshSize);

			//uv[ver]=tc[ver];

			//		uvCrd*=10;//to repeat uv
			//		buffer->UVCoords.push_back(uvCrd);
		}
	}

	pbuffer->unlock();

	//creating Indicies Array
	/*

	meshSize=3
	0--1--2
	|\ |\ |
	| \| \|
	3--4--5
	t1=(0,4,1)
	t2=(0,3,4)
	...and so on

	*/
	m_tmpIndiciesUINT.resize(0);
	math::Point3di idx;
	for(int i=0;i<meshSize-1;++i){
		for(int j=0;j<meshSize-1;j++)
		{
			idx.x=i*meshSize+j;
			idx.y=(i+1)*meshSize+j+1;
			idx.z=(i)*meshSize+j+1;
			m_tmpIndiciesUINT.push_back(idx.x);
			m_tmpIndiciesUINT.push_back(idx.z);
			m_tmpIndiciesUINT.push_back(idx.y);

			//		ind.x=i*meshSize+j; //same as above
			idx.z=idx.y;//(i+1)*meshSize+j+1;
			idx.y=(i+1)*meshSize+j;
			m_tmpIndiciesUINT.push_back(idx.x);
			m_tmpIndiciesUINT.push_back(idx.z);
			m_tmpIndiciesUINT.push_back(idx.y);


		}
	}

	video::IHardwareIndexBuffer*indBuff=buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_32Bit,m_tmpIndiciesUINT.size(),video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false);
	indBuff->writeData(0,indBuff->getSizeInBytes(),&m_tmpIndiciesUINT[0]);

	buffer->setBoundingBox(math::box3d(-terrinSize/2.0f,minY,-terrinSize/2.0f,
		terrinSize/2.0f,maxY,terrinSize/2.0f));

//	buffer->position.x=- terrinSize/2.0f;
//	buffer->position.z=- terrinSize/2.0f;

	vStream->unlock();
	nStream->unlock();
	uvStream->unlock();
	setMeshName(terrMesh,EGT_Terrain);
}

void MeshGenerator::generateBillboard(IViewNode*cam, const math::vector3d&  pos, const math::vector2d&  size, float angle, bool rotHor, bool rotVer, math::vector3d*vert, int startIndex, int startVertex)
{

	math::vector3d hor(1,0,0);
	math::vector3d ver(0,1,0);

	float rotZ=angle;

	float ca = math::cosd(rotZ);
	float sa = math::sind(rotZ);

	// X = U x F/||U x F||
	if(rotHor){
		hor=cam->getViewMatrix().getAxisVector(0);
	}


	// Y = X x F/||X x F||
	if(rotVer){
		ver=cam->getViewMatrix().getAxisVector(1);
	}

	//  	view*=-1;
	//  	for(int i=0;i<4;++i)
	//  		norm[i]=view;

	math::vector3d X,Y;

	X =( ver * ca + hor * sa) * 0.5f * size.x;
	Y =(-ver * sa + hor * ca) * 0.5f * size.y;


	vert[0]=pos+X+Y;
	vert[1]=pos-X+Y;
	vert[2]=pos-X-Y;
	vert[3]=pos+X-Y;
}
void MeshGenerator::generateBillboard(IViewNode*cam,const math::vector3d&  pos,const math::vector2d&  size,float angle,
	bool rotHor, bool rotVer, IMeshBuffer*buffer, int startIndex, int startVertex)
{

	if(!buffer)
		return;
	int vCount=4;

	video::IHardwareStreamBuffer* vStream= buffer->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_Dynamic,false);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}

//	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);
// 

	math::vector3d vert[4];

	generateBillboard(cam,pos,size,angle,rotHor,rotVer,vert);

	//vStream->unlock();
	vStream->writeData(0,4*sizeof(math::vector3d),vert);

	math::box3d b;
	b.reset(pos);
	b.AddPoint(pos-math::Max(size.x,size.y)*0.5f);
	b.AddPoint(pos+math::Max(size.x,size.y)*0.5f);

	buffer->setBoundingBox(b);
//	buffer->recalculateBoundingBox();
//	setMeshName(mesh,EGT_Billboard);
}
void MeshGenerator::generateBillboardBeam(IViewNode*cam,float width,const math::vector3d&  start,
	const math::vector3d&  end, IMeshBuffer*mesh, const math::matrix4x4* worldMat, int startIndex , int startVertex )
{


	if(!mesh)
		return;

	int vCount=4;

	math::line3d beamLine(start,end);

	if(worldMat)
	{
		beamLine.pStart=(*worldMat)*beamLine.pStart;
		beamLine.pEnd  =(*worldMat)*beamLine.pEnd;
	}

	video::IHardwareStreamBuffer* vStream= mesh->getStream(0,video::EMST_Position);
	if(!vStream)
		vStream=mesh->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream= mesh->getStream(0,video::EMST_Normal);
	if(!nStream) 
		nStream=mesh->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
// 	video::IHardwareStreamBuffer* uvStream= mesh->getStream(0,video::EMST_Texcoord);
// 	if(!uvStream) 
// 		uvStream=mesh->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCount,video::IHardwareStreamBuffer::EUT_StaticWriteOnly,true);
// 	else{
// 		uvStream->setElementType(video::ESDT_Point2f);
// 		uvStream->resize(vCount*vStream->getElementSize());
// 	} 
	math::vector3d*vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareStreamBuffer::ELO_Discard);

	/*
	math::vector3d segment=end-start;
	math::vector3d rightVec=segment;
	rightVec.Normalize();


	math::vector3d frontVec=cam->getViewPos() - start;//(segment/2+start);
	frontVec.Normalize();

	math::vector3d upVec=frontVec.crossProduct(rightVec);
	frontVec=rightVec.crossProduct(upVec);
	*/


	const math::matrix4x4 &mat=cam->getViewMatrix();
	//eye vector to beam
	//E=Cp-Bs
	math::vector3d eye(cam->getViewPos() - beamLine.pStart);
	eye.Normalize();
	//beam vector defined by endPoints
	//B = Be - Bs
	math::vector3d beam=beamLine.pEnd-beamLine.pStart;
	beam.Normalize();
	//Cross Produt of beam and eye
	//P=B x E
	math::vector3d rightVec = eye.crossProduct(beam);

	//Bf=Cf
	math::vector3d frontVec(mat.f31, mat.f32, mat.f33);

	//find up and right vectors
	//Bu=(Bf x P) / |Bf x P|
	math::vector3d upVec=frontVec.crossProduct(rightVec);
//	upVec.Normalize();

	frontVec=upVec.crossProduct(rightVec);
	//Br=Bf x Bu
	//rightVec=frontVec.crossProduct(upVec);
	
	//calc matrix
	/*
	math::matrix4x4 mBeam;

	mBeam.f11=rightVec.x;
	mBeam.f12=rightVec.y;
	mBeam.f13=rightVec.z;

	mBeam.f21=upVec.x;
	mBeam.f22=upVec.y;
	mBeam.f23=upVec.z;

	mBeam.f31=frontVec.x;
	mBeam.f32=frontVec.y;
	mBeam.f33=frontVec.z;

	//create a matrix for each end point

	math::matrix4x4 mSPoint=mBeam;

	mSPoint.f14=start.x;
	mSPoint.f24=start.y;
	mSPoint.f34=start.z;

	math::matrix4x4 mEPoint=mBeam;

	mEPoint.f14=end.x;
	mEPoint.f24=end.y;
	mEPoint.f34=end.z;*/

	vert[0]=math::vector3d(rightVec.x,upVec.x,frontVec.x)*(-width)+beamLine.pStart;// mSPoint*(math::vector3d(-width,0,0));
	vert[1]=math::vector3d(rightVec.x,upVec.x,frontVec.x)*(width)+beamLine.pStart;//mSPoint*(math::vector3d( width,0,0));

	vert[2]=math::vector3d(rightVec.x,upVec.x,frontVec.x)*(-width)+beamLine.pEnd;//mEPoint*(math::vector3d(-width,0,0));
	vert[3]=math::vector3d(rightVec.x,upVec.x,frontVec.x)*(width)+beamLine.pEnd;//mEPoint*(math::vector3d( width,0,0));


	vStream->unlock();
/*
	mesh->recalculateBoundingBox();

	setMeshName(bill,EGT_BillboardBeam);*/
}


void MeshGenerator::generateBillboardChain(IViewNode*cam, float startW, float endW, const math::vector3d* points, int count,  IMeshBuffer*mesh, const math::matrix4x4* worldMat, int startIndex, int startVertex)
{

	if (!mesh || count<2)
		return;

	int vCount = count*2;


	video::IHardwareStreamBuffer* vStream = mesh->getStream(0, video::EMST_Position);
	if (!vStream)
		vStream = mesh->createStream(0, video::EMST_Position, video::ESDT_Point3f, vCount, video::IHardwareStreamBuffer::EUT_StaticWriteOnly, true);
	else{
		vStream->setElementType(video::ESDT_Point3f);
		vStream->resize(vCount*vStream->getElementSize());
	}

	video::IHardwareStreamBuffer* tcStream = mesh->getStream(0, video::EMST_Texcoord);
	if (!tcStream)
		tcStream = mesh->createStream(0, video::EMST_Position, video::ESDT_Point2f, vCount, video::IHardwareStreamBuffer::EUT_StaticWriteOnly, true);
	else{
		tcStream->setElementType(video::ESDT_Point2f);
		tcStream->resize(vCount*tcStream->getElementSize());
	}
	video::IHardwareStreamBuffer* nStream = mesh->getStream(0, video::EMST_Normal);
	if (!nStream)
		nStream = mesh->createStream(0, video::EMST_Normal, video::ESDT_Point3f, vCount, video::IHardwareStreamBuffer::EUT_StaticWriteOnly, true);
	else{
		nStream->setElementType(video::ESDT_Point3f);
		nStream->resize(vCount*vStream->getElementSize());
	}
	math::vector3d*vert = (math::vector3d*)vStream->lock(0, 0, video::IHardwareStreamBuffer::ELO_Discard);
	math::vector2d*tc = (math::vector2d*)tcStream->lock(0, 0, video::IHardwareStreamBuffer::ELO_Discard);
	math::vector3d*norm = (math::vector3d*)nStream->lock(0, 0, video::IHardwareStreamBuffer::ELO_Discard);

	const math::matrix4x4 &mat = cam->getViewMatrix();
	//Bf=Cf
	//(mat(0,0), mat(0,1), mat(0,2));
	//math::vector3d camPos=cam->getAbsolutePosition();
	//math::vector3d frontVec=cam->target()-camPos;
	//int cnt = m_currIndex - m_startIndex + 1;
	int indexCount = 2 * count - 2;
	// 	if(cnt>0)
	// 		m_vPoses[0]=getAbsolutePosition();
	int n = 0;

	float deltaV = 1.0f / (float)count;

	math::vector3d eye = mat.getAxisVector(2);
	float currV = 0;
	for (int i = 0; i<count-1; i += 2){
		//eye vector to beam
		//E=Cp-Bs
		int a = (i + 0);
		int b = (a + 1);

		math::vector3d vStartPoint = points[i];
		math::vector3d vEndPoint = points[i+1];

		//beam vector defined by endPoints
		//B = Be - Bs
		math::vector3d beam = vStartPoint - vEndPoint;
		beam.Normalize();
		// 
		// 		//Cross Produt of beam and eye
		// 		//P=B x E
		// 		math::vector3d pBeam = eye.crossProduct(beam);
		// 
		// 		//find up and right vectors
		// 		//Bu=(Bf x P) / |Bf x P|
		// 		//Br=Bf x Bu
		// 		math::vector3d upVec=frontVec.crossProduct(pBeam);
		// 		upVec.Normalize();
		// 
		//		math::vector3d rightVec=frontVec.crossProduct(upVec);
		//Cross Produt of beam and eye
		//P=B x E
		math::vector3d upVec = eye.crossProduct(beam);
		//upVec.set(0,1,0);
		math::vector3d rightVec = upVec.crossProduct(beam);

		//find up and right vectors
		//Bu=(Bf x P) / |Bf x P|
		//Br=Bf x Bu
		//		math::vector3d rightVec=eye.crossProduct(upVec);
		//		upVec.Normalize();

		//		math::vector3d rightVec=frontVec.crossProduct(upVec);
		/*
		//calc matrix
		matrix4x4 mBeam;

		mBeam(0,0)=rightVec.x;
		mBeam(0,1)=rightVec.y;
		mBeam(0,2)=rightVec.z;

		mBeam(1,0)=upVec.x;
		mBeam(1,1)=upVec.y;
		mBeam(1,2)=upVec.z;

		mBeam(2,0)=frontVec.x;
		mBeam(2,1)=frontVec.y;
		mBeam(2,2)=frontVec.z;

		//create a matrix for each end point

		matrix4x4 mSPoint=mBeam;

		mSPoint(3,0)=vStartPoint.x;
		mSPoint(3,1)=vStartPoint.y;
		mSPoint(3,2)=vStartPoint.z;

		matrix4x4 mEPoint=mBeam;

		mEPoint(3,0)=vEndPoint.x;
		mEPoint(3,1)=vEndPoint.y;
		mEPoint(3,2)=vEndPoint.z;
		//mSPoint.transformVect(math::vector3d(-w,0,0),mesh->currFrame[i*2].Pos);
		//mSPoint.transformVect(math::vector3d( w,0,0),mesh->currFrame[i*2+2].Pos);

		//mEPoint.transformVect(math::vector3d(-w,0,0),mesh->currFrame[i*2+3].Pos);
		//mEPoint.transformVect(math::vector3d( w,0,0),mesh->currFrame[i*2+1].Pos);
		*/

		float t = 1 - (float)i / (float)count;

		tc[n + 0] = math::vector2d(currV, 0);
		tc[n + 1] = math::vector2d(currV, 1);
		tc[n + 2] = math::vector2d(currV + deltaV, 0);
		tc[n + 3] = math::vector2d(currV + deltaV, 1);

		currV += deltaV;

		float w = startW*(1 - t) + endW*t;
		vert[n + 0] = vStartPoint - upVec*w;
		vert[n + 1] = vStartPoint + upVec*w;
		vert[n + 2] = vEndPoint - upVec*w;
		vert[n + 3] = vEndPoint + upVec*w;

		norm[n + 0] = rightVec;
		norm[n + 1] = rightVec;
		norm[n + 2] = rightVec;
		norm[n + 3] = rightVec;
/*

		binorm[n + 0] = upVec;
		binorm[n + 1] = upVec;
		binorm[n + 2] = upVec;
		binorm[n + 3] = upVec;

		tangent[n + 0] = beam;
		tangent[n + 1] = beam;
		tangent[n + 2] = beam;
		tangent[n + 3] = beam;*/

		n += 4;
	}
	vStream->unlock();
	nStream->unlock();
// 	bStream->unlock();
// 	tStream->unlock();
	tcStream->unlock();
}

}
}