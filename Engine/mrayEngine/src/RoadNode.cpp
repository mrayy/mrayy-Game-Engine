#include "stdafx.h"


#include "RoadNode.h"

#include "IMeshBuffer.h"

namespace mray
{
namespace scene
{


RoadNode::RoadNode()
{
	m_width=10;
}
RoadNode::~RoadNode()
{
}

void RoadNode::SetRoadCVs(const std::vector<math::vector3d>& cvs)
{
	math::vector3d v;
	if(cvs.size()>1)
	{
		m_points.resize(cvs.size());
		m_points[0].pos=cvs[0];
		m_points[0].up=math::vector3d::YAxis;
		v=(cvs[1]-cvs[0]);
		v.Normalize();
		m_points[0].side=v.crossProduct(m_points[0].up);
	}else{
		m_points.resize(0);
		return;
	}
	for (int i=1;i<cvs.size()-1;++i)
	{
		m_points[i].pos=cvs[i];
		m_points[i].up=math::vector3d::YAxis;
		v=(cvs[i+1]-cvs[i]);
		v.Normalize();
		math::vector3d side=v.crossProduct(m_points[i].up);
		m_points[i].side=(side+m_points[i-1].side).Normalize();
	}

	m_points[cvs.size()-1].pos=cvs[cvs.size()-1];
	m_points[cvs.size()-1].up=math::vector3d::YAxis;
	m_points[cvs.size()-1].side=m_points[cvs.size()-2].side;
}
void RoadNode::Build(float w,IMeshBuffer* buffer)
{
	if(m_points.size()<2)return;
	m_width=w;
	uint vCnt=2*m_points.size();
	uint iCnt=6*(m_points.size()-1);

	video::IHardwareStreamBuffer* posStream=0;
	video::IHardwareStreamBuffer* uvStream=0;
	video::IHardwareStreamBuffer* normalStream=0;
	video::IndexData* indexData=0;

	posStream=buffer->getStream(0,video::EMST_Position);
	if(!posStream)
	{
		posStream=buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,true,false);
	}else posStream->resize(vCnt*sizeof(math::vector3d));

	normalStream=buffer->getStream(0,video::EMST_Normal);
	if(!normalStream)
	{
		normalStream=buffer->createStream(0,video::EMST_Normal,video::ESDT_Point3f,vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,true,false);
	}else normalStream->resize(vCnt*sizeof(math::vector3d));

	uvStream=buffer->getStream(0,video::EMST_Texcoord);
	if(!uvStream)
	{
		uvStream=buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point2f,vCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,true,false);
	}else uvStream->resize(vCnt*sizeof(math::vector2d));

	indexData=buffer->getIndexData();
	if(!indexData->indexBuffer)
	{
		buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,iCnt,video::IHardwareBuffer::EUT_StaticWriteOnly,true,false);
	}else indexData->indexBuffer->resize(iCnt*sizeof(ushort));
	indexData->indexCount=iCnt;

	math::vector3d*vPtr=(math::vector3d*)posStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector3d*nPtr=(math::vector3d*)normalStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector2d*uvPtr=(math::vector2d*)uvStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	ushort*iPtr=(ushort*)indexData->indexBuffer->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	float hWidth=m_width*0.5;

	math::box3d boundingBox;

	boundingBox.reset(m_points[0].pos);

	float currVTex=0;

	int currVertex=0;
	int currIndex=0;
	int currPoint=0;
	for (;currPoint<m_points.size()-1;++currPoint)
	{
		vPtr[currVertex]		=m_points[currPoint].pos-m_points[currPoint].side*hWidth;
		vPtr[currVertex+1]		=m_points[currPoint].pos+m_points[currPoint].side*hWidth;

		nPtr[currVertex]		=m_points[currPoint].up;
		nPtr[currVertex+1]		=m_points[currPoint].up;

		uvPtr[currVertex].set(0,currVTex);
		uvPtr[currVertex+1].set(1,currVTex);

		currVTex+=(m_points[currPoint].pos-m_points[currPoint+1].pos).Length()/m_width;

		boundingBox.AddPoint(vPtr[currVertex]);
		boundingBox.AddPoint(vPtr[currVertex+1]);

		iPtr[currIndex]			=currVertex;
		iPtr[currIndex+1]		=currVertex+1;
		iPtr[currIndex+2]		=currVertex+2;

		iPtr[currIndex+3]		=currVertex+1;
		iPtr[currIndex+4]		=currVertex+3;
		iPtr[currIndex+5]		=currVertex+2;

		currVertex+=2;
		currIndex +=6;

	}


	//last point
	vPtr[currVertex]		=m_points[currPoint].pos-m_points[currPoint].side*hWidth;
	vPtr[currVertex+1]		=m_points[currPoint].pos+m_points[currPoint].side*hWidth;

	nPtr[currVertex]		=m_points[currPoint].up;
	nPtr[currVertex+1]		=m_points[currPoint].up;

	posStream->unlock();
	normalStream->unlock();
	uvStream->unlock();
	indexData->indexBuffer->unlock();

	buffer->setBoundingBox(boundingBox);
}
void RoadNode::Build(SPatchTerrain*terrain)
{
}




}
}

