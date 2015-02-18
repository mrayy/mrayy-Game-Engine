#include "stdafx.h"

#include "GPUSkeletonAnimator.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "IMeshBuffer.h"
#include "Skeleton.h"
#include "ISkinCluster.h"
#include "BoneNode.h"


namespace mray{
namespace scene{


//defaultStreams .. where each attribute will go , ex : TEXCOORD1,TEXCOORD2,...
GPUSkeletonAnimator::GPUSkeletonAnimator(IMeshBuffer*buff,ISkinCluster* skinCluster,Skeleton*skeleton,
										 int weightStream,int boneStream ):m_buffer(0)
{
	m_skeleton=skeleton;
	m_skinCluster=skinCluster;
	m_weightStream=weightStream;
	m_boneStream=boneStream;
	setMeshBuffer(buff);

}
GPUSkeletonAnimator::~GPUSkeletonAnimator(){
}


void GPUSkeletonAnimator::setMeshBuffer(IMeshBuffer*buff)
{

	m_buffer=buff;

	if(!m_buffer || !m_skeleton || !m_skinCluster)
		return;

	video::StreamsData* data= m_buffer->getStreamsData();


	video::IHardwareStreamBuffer* wStream;
	video::IHardwareStreamBuffer* bStream;

	wStream=m_buffer->getStream(m_weightStream,video::EMST_Texcoord);
	if(!wStream)
		wStream=m_buffer->createStream(m_weightStream,video::EMST_Texcoord,video::ESDT_Point4f,data->count,
		video::IHardwareBuffer::EUT_Static,false);

	bStream=m_buffer->getStream(m_boneStream,video::EMST_Texcoord);
	if(!bStream)
		bStream=m_buffer->createStream(m_boneStream,video::EMST_Texcoord,video::ESDT_Point4f,data->count,
		video::IHardwareBuffer::EUT_Static,false);

	int start=data->start*sizeof(math::vector4d);
	int count=data->count*sizeof(math::vector4d);

	math::vector4d*weightsPtr=(math::vector4d*) wStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	mraySystem::memSet(weightsPtr,0,count);

	math::Point4d<float>*bonesPtr=(math::Point4d<float>*) bStream->lock(0,0,video::IHardwareBuffer::ELO_Discard);
	mraySystem::memSet(bonesPtr,0,data->count*sizeof(math::Point4d<float>));

	std::vector<uchar> bonesCnt;
	bonesCnt.resize(data->count);
	mraySystem::memSet(&bonesCnt[0],0,data->count*sizeof(uchar));

	const BonesList& bones= m_skeleton->getBones();
	BonesList::const_iterator it= bones.begin();

	uchar* bonesCntPtr=&bonesCnt[0];

	int i=0;
	int c=0;
	int bid;
	BoneNode*b;
	int cnt;
	float*w;
	ISkinCluster::SBoneWeights*weights;

	for (;it!=bones.end();++it)
	{
		b=*it;

		bid=b->getID()-1;

		weights= m_skinCluster->getBoneWeights(bid+1);
		if(!weights || weights->vertices.size()==0)continue;

		cnt=weights->vertices.size();
		w=&weights->vertices[0];


		for (i=0;i<cnt;++i)
		{
			if(bonesCntPtr[i]>=4){
				continue;
			}
			if(w[i]<=math::Epsilon)
			{
				continue;
			}

			c=bonesCntPtr[i];
			weightsPtr[i][c]=w[i];
			bonesPtr[i][c]=(float)bid;

			bonesCntPtr[i]++;
		}
	}
	wStream->unlock();
	bStream->unlock();

	m_bonesMatricies.resize(m_skeleton->getBonesCount());
}

void GPUSkeletonAnimator::update(float dt){
	if( !m_skeleton)
		return;

	const BonesList& bones= m_skeleton->getBones();
	BonesList::const_iterator it= bones.begin();
	for(int i=0;it!=bones.end() ; ++i, ++it){
		math::matrix4x4 abb;
		BoneNode*b=*it;
		int id=b->getID()-1;//bones id start at 1
		FATAL_ERROR(id<0 || id>=m_bonesMatricies.size(),mT("Bone ID out of bounds!"));
		b->getFinalBoneTransformation(m_bonesMatricies[id]);
	}
}

Skeleton*GPUSkeletonAnimator::getSkeleton(){
	return m_skeleton;
}
ISkinCluster*GPUSkeletonAnimator::getSkinCluster(){
	return m_skinCluster;
}

IMeshAnimator* GPUSkeletonAnimator::duplicate()
{
	return new GPUSkeletonAnimator(m_buffer,m_skinCluster,m_skeleton,m_weightStream,m_boneStream);
}

}
}
