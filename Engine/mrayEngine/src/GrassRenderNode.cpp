#include "stdafx.h"

#include "GrassRenderNode.h"
#include "SMeshBuffer.h"

#include "SMesh.h"
#include "MeshBufferData.h"
#include "IRenderPass.h"
#include "IViewNode.h"

namespace mray
{
namespace scene
{

GrassRenderNode::GrassRenderNode()
:MeshRenderableNode(SMeshPtr::Null),m_buffer(0),m_totalIndexCount(0)
{
	//std::list<scene::StaticGeometryBatch::MeshStreamInfo> streamsToCopy;
	//streamsToCopy.push_back(video::EMST_Position);
	//streamsToCopy.push_back(video::EMST_Texcoord);
	//m_grassObjects=new scene::StaticGeometryBatch(streamsToCopy);

}
GrassRenderNode::~GrassRenderNode()
{
}


bool GrassRenderNode::preRender(IRenderPass*pass)
{
	//choose the amount of indexes based on the distance of the view node

	if(m_buffer && pass->getViewNode())
	{
		math::vector3d diff=pass->getViewNode()->getViewPos()-m_buffer->getBoundingBox().getCenter();
		float lenSQ=diff.LengthSQ();
		float minDistSQ=m_buffer->getBoundingBox().getExtent().LengthSQ();
		lenSQ-=minDistSQ;
		if(lenSQ>0)
		{
			float len=sqrt(lenSQ);
			int amountToRemove=m_totalIndexCount*len*0.005;
			if(amountToRemove>m_totalIndexCount*0.75)
				amountToRemove=m_totalIndexCount*0.75;
			m_buffer->getIndexData()->indexCount=m_totalIndexCount-amountToRemove;
		}else
			m_buffer->getIndexData()->indexCount=m_totalIndexCount;
	}else
		m_buffer->getIndexData()->indexCount=m_totalIndexCount;

	return MeshRenderableNode::preRender(pass);
}

void GrassRenderNode::AddUVRect(const math::rectf& uv,float prob)
{
	m_uvRects.push_back(UVRect(uv,prob));
}
int GrassRenderNode::GetUVRectCount()
{
	return m_uvRects.size();
}
math::rectf GrassRenderNode::GetUVRect(int idx)
{
	if(idx>=m_uvRects.size())
		return math::rectf(1,0);
	return m_uvRects[idx].rc;
}
const math::rectf& GrassRenderNode::GetRandomUVRect()
{
	float r=math::Randomizer::rand01();
	float s=0;
	for(int i=0;i<m_uvRects.size();++i)
	{
		s+=m_uvRects[i].prob;
		if(s>r)
			return m_uvRects[i].rc;
	}
	return m_uvRects[0].rc;
}

void GrassRenderNode::AddObject(const math::vector2d& texPos,const math::vector3d&pos,float scale,float shadow)
{
	GrassData d;
	d.texPos=texPos;
	d.pos=pos;
	d.scale=scale;
	d.shadow=shadow;
	m_grassData.push_back(d);
}

GrassRenderNode::GrassData* GrassRenderNode::GetGrassData(int i)
{
	if(i>=m_grassData.size())
		return 0;
	return &m_grassData[i];
}

void GrassRenderNode::ShuffleGrasses()
{
	int cnt=GetGrassCount();
	GrassData t;
	for(int i=0;i<cnt;i+=2)
	{
		int randIdx=math::Randomizer::rand(cnt);
		t=m_grassData[i];
		m_grassData[i]=m_grassData[randIdx];
		m_grassData[randIdx]=t;
	}
}

void GrassRenderNode::Build(int spans)
{

	//normalize probabilities
	{
		float maxProb=0;
		for(int i=0;i<m_uvRects.size();++i)
		{
			if(m_uvRects[i].prob>maxProb)
				maxProb=m_uvRects[i].prob;
		}
		if(maxProb==0)
			maxProb=1;
		float invProb=1.0f/(maxProb*m_uvRects.size());
		for(int i=0;i<m_uvRects.size();++i)
			m_uvRects[i].prob*=invProb;
		for(int i=0;i<m_uvRects.size();++i)
		{
			for(int j=i+1;j<m_uvRects.size();++j)
			{
				if(m_uvRects[i].prob<m_uvRects[j].prob)
					math::Swap(m_uvRects[i],m_uvRects[j]);
			}
		}
	}


	m_mesh->clear();
	int vtxCount=4*spans*m_grassData.size();//total number of verticies
	m_totalIndexCount=4*spans*m_grassData.size();//total number of indicies
	int currIndex=0;

	m_buffer= m_mesh->addNewBuffer()->getMeshBuffer();

	m_buffer->setRenderType(video::MR_QUADS);
	video::IHardwareStreamBuffer* vStream= m_buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vtxCount,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
	video::IHardwareStreamBuffer* dStream= m_buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point4f,vtxCount,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);
	video::IHardwareStreamBuffer* d2Stream= m_buffer->createStream(1,video::EMST_Texcoord,video::ESDT_Point4f,vtxCount,video::IHardwareBuffer::EUT_StaticWriteOnly,0,0);

	bool idx16Bit=true;

	if(vtxCount>65535)
		idx16Bit=false;

	video::IHardwareIndexBuffer*indBuff=m_buffer->createIndexBuffer(idx16Bit?video::IHardwareIndexBuffer::EIT_16Bit
		:video::IHardwareIndexBuffer::EIT_32Bit,m_totalIndexCount,video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false,false);

	math::vector3d *vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector4d *data=(math::vector4d*)dStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector4d *data2=(math::vector4d*)d2Stream->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	uchar *indexData=(uchar*)indBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);

	math::box3d bb;

	for (int i=0;i<m_grassData.size();++i)
	{
		GrassData& grassData=m_grassData[i];
		//GCPtr<scene::IMeshBuffer> buff=new scene::SMeshBuffer();
		_GenerateGrassNode(grassData,spans,vert,data,data2,indexData,currIndex,bb,idx16Bit);
		/*
		math::matrix4x4 trans;
		math::quaternion q(math::Randomizer::rand01()*360,math::vector3d::YAxis);
		q.toMatrix(trans);
		trans.setTranslation(grassData.pos);
		if(grassData.scale!=1)
		{
			math::matrix4x4 scale;
			scale.setScale(grassData.scale);
			trans=scale*trans;
		}*/
		//m_grassObjects->addNode(buff,0,trans);
	}
	vStream->unlock();
	dStream->unlock();
	d2Stream->unlock();
	indBuff->unlock();

	m_buffer->setBoundingBox(bb);
	m_mesh->setBoundingBox(bb);

//	m_grassObjects->build();
}

void GrassRenderNode::ClearGrasses()
{
	m_grassData.clear();
}

void GrassRenderNode::_GenerateGrassNode(GrassData&g,int spans,math::vector3d*&vert,math::vector4d *&data,math::vector4d *&data2,
									  uchar *&indexData,int &currIndex,math::box3d&bb,bool index16bit)
{
	int vCount=4*spans;
/*
	buffer->setRenderType(video::MR_TRIANGLES);
	video::IHardwareStreamBuffer* vStream= buffer->createStream(0,video::EMST_Position,video::ESDT_Point3f,vCount,
		video::IHardwareStreamBuffer::EUT_Static,false,true);
	video::IHardwareStreamBuffer* dStream= buffer->createStream(0,video::EMST_Texcoord,video::ESDT_Point4f,vCount,
		video::IHardwareStreamBuffer::EUT_Static,false,true);

	video::IHardwareIndexBuffer*indBuff=buffer->createIndexBuffer(video::IHardwareIndexBuffer::EIT_16Bit,6*spans,
		video::IHardwareIndexBuffer::EUT_StaticWriteOnly,false,true);

	math::vector3d *vert=(math::vector3d*)vStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	math::vector4d *data=(math::vector4d*)dStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	ushort *indexData=(ushort*)indBuff->lock(0,0,video::IHardwareBuffer::ELO_Discard);
*/

	math::matrix4x4 trans;
	trans.setRotationRadians(math::vector3d(0,math::Randomizer::rand01()*math::PI32,0));
	/*
	math::quaternion q(math::Randomizer::rand01()*360,math::vector3d::YAxis);
	q.toMatrix(trans);*/
	if(g.scale!=1)
	{
		math::matrix4x4 scale;
		scale.setScale(g.scale);
		trans=scale*trans;
	}
	trans.setTranslation(g.pos);

	if(currIndex==0)
		bb.reset(g.pos);

	int idxOffset=2;
	if(!index16bit)
		idxOffset=4;

	

	float step=180.0f/(float)spans;
	float angle=0;
	math::rectf rc=GetRandomUVRect();
	float timeOffset=math::Randomizer::rand01()*math::PI32;
	for (int i=0;i<spans;++i)
	{
		math::quaternion q(angle,math::vector3d::YAxis);
		math::vector3d XAxis=q*math::vector3d::XAxis;
		vert[0]=-XAxis*0.5f;
		vert[1]=XAxis*0.5f;
		vert[2]=XAxis*0.5f+math::vector3d::YAxis*2;
		vert[3]=-XAxis*0.5f+math::vector3d::YAxis*2;

		vert[0]=trans*vert[0];
		vert[1]=trans*vert[1];
		vert[2]=trans*vert[2];
		vert[3]=trans*vert[3];

		bb.AddPoint(vert[0]);
		bb.AddPoint(vert[1]);
		bb.AddPoint(vert[2]);
		bb.AddPoint(vert[3]);

		data[0].set(rc.BRPoint.x,rc.BRPoint.y,0,0);
		data[1].set(rc.ULPoint.x,rc.BRPoint.y,0,0);
		data[2].set(rc.ULPoint.x,rc.ULPoint.y,1,timeOffset+math::Randomizer::rand01()*0.2);
		data[3].set(rc.BRPoint.x,rc.ULPoint.y,1,timeOffset+math::Randomizer::rand01()*0.2);

		data2[0].set(g.texPos.x,g.texPos.y,g.shadow,0);
		data2[1].set(g.texPos.x,g.texPos.y,g.shadow,0);
		data2[2].set(g.texPos.x,g.texPos.y,0,0);
		data2[3].set(g.texPos.x,g.texPos.y,0,0);

		if(index16bit){
			((ushort*)indexData)[0]=currIndex+0;
			((ushort*)indexData)[1]=currIndex+1;
			((ushort*)indexData)[2]=currIndex+2;
			((ushort*)indexData)[3]=currIndex+3;
			/*
			((ushort*)indexData)[3]=currIndex+0;
			((ushort*)indexData)[4]=currIndex+2;
			((ushort*)indexData)[5]=currIndex+3;*/
		}else
		{
			((uint*)indexData)[0]=currIndex+0;
			((uint*)indexData)[1]=currIndex+1;
			((uint*)indexData)[2]=currIndex+2;
			((uint*)indexData)[3]=currIndex+3;

/*
			((uint*)indexData)[3]=currIndex+0;
			((uint*)indexData)[4]=currIndex+2;
			((uint*)indexData)[5]=currIndex+3;*/

		}

		angle+=step;
		vert+=4;
		data+=4;
		data2+=4;
		indexData+=4*idxOffset;
		currIndex+=4;
	}
	/*
	vStream->unlock();
	dStream->unlock();
	indBuff->unlock();*/

}


//////////////////////////////////////////////////////////////////////////
std::vector<GrassRenderNode*> GrassRenderNode::GenerateFromPixels(video::IHardwarePixelBuffer*tex,const math::vector2d&worldSize,float grassPerUnit,
															int patchesPerEdge,float scale)
{
	std::vector<GrassRenderNode*> result;
	if(tex->getFormat()!=video::EPixel_LUMINANCE8)
		return result;
	math::vector2d texSize(tex->getWidth(),tex->getHeight());
	math::vector2d PixelSize=worldSize/texSize;
	math::vector2di EdgeLength=(texSize/patchesPerEdge)+1;

	math::vector2d PixelsIntensity=PixelSize*grassPerUnit;
	if(PixelsIntensity.x<1)PixelsIntensity.x=1;
	if(PixelsIntensity.y<1)PixelsIntensity.y=1;

	video::LockedPixelBox pixels= tex->lock(math::box3d(0,0,0,texSize.x,texSize.y,1),video::IHardwareBuffer::ELO_ReadOnly);

	result.resize(patchesPerEdge*patchesPerEdge);
	for(int i=0;i<result.size();++i)
		result[i]=new GrassRenderNode();

	uchar*ptr=(uchar*)pixels.data;
	
	for(int i=0;i<texSize.x;++i)
	{
		int baseIdx=(i/EdgeLength.x)*patchesPerEdge;
		for(int j=0;j<texSize.y;++j)
		{
			float intensity=(float)(*ptr)*math::i255;
			int yIdx=(j/EdgeLength.y);
			math::vector2d pixelPos(i*PixelSize.x,j*PixelSize.y);
			int idx=(baseIdx+yIdx);
			math::vector2di GrassesCount=PixelsIntensity*intensity;
			int cnt=GrassesCount.x*GrassesCount.y;
			for(int k=0;k<cnt;++k)
			{
				math::vector3d pos((math::Randomizer::rand01()-0.5)*PixelSize.x+pixelPos.x,0,(math::Randomizer::rand01()-0.5)*PixelSize.y+pixelPos.y);
				result[idx]->AddObject(math::vector2d(pos.x,pos.z)/worldSize,pos,scale,intensity);
			}
			++ptr;
		}
	}

	//shuffle the indexes in the batches
	for(int i=0;i<result.size();++i)
	{
		result[i]->ShuffleGrasses();
	}


	tex->unlock();
	return result;
}

}
}