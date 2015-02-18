#include "stdafx.h"

#include "SkeletonAnimator.h"
#include "mraySystem.h"
#include "IHardwareStreamBuffer.h"
#include "IMeshBuffer.h"
#include "Skeleton.h"
#include "ISkinCluster.h"
#include "BoneNode.h"

namespace mray{
namespace scene{

#define POSITION_INDEX 0
#define NORMAL_INDEX 1
#define BINORMAL_INDEX 2
#define TANGENT_INDEX 3

SkeletonAnimator::SkeletonAnimator(IMeshBuffer*buff,ISkinCluster* skinCluster,
								   Skeleton*skeleton,int affectedAttrs){

	m_skeleton=skeleton;
	m_skinCluster=skinCluster;
	m_affectedAttrs=affectedAttrs;

	setMeshBuffer(buff);

}
SkeletonAnimator::~SkeletonAnimator(){

}

void SkeletonAnimator::setMeshBuffer(IMeshBuffer*buff)
{
	m_buffer=buff;

	video::StreamsData* data= m_buffer->getStreamsData();

	int start=data->start*sizeof(math::vector3d);
	int count=data->count*sizeof(math::vector3d);

	if(m_affectedAttrs & EAA_Position){
		video::IHardwareStreamBuffer*s=buff->getStream(0,video::EMST_Position);
		if(s)
		{
			m_baseVectors[POSITION_INDEX].resize(data->count);
			math::vector3d*v=(math::vector3d*) s->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
			mraySystem::memCopy(&m_baseVectors[POSITION_INDEX][0],v,count);
			s->unlock();
		}else
			m_affectedAttrs=m_affectedAttrs & (~EAA_Position);
	}
	if(m_affectedAttrs & EAA_Normal){
		video::IHardwareStreamBuffer*s=buff->getStream(0,video::EMST_Normal);
		if(s)
		{
			m_baseVectors[NORMAL_INDEX].resize(data->count);
			math::vector3d*v=(math::vector3d*) s->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
			mraySystem::memCopy(&m_baseVectors[NORMAL_INDEX][0],v,count);
			s->unlock();
		}else
			m_affectedAttrs=m_affectedAttrs & (~EAA_Normal);
	}
	if(m_affectedAttrs & EAA_Tangent){
		video::IHardwareStreamBuffer*s=buff->getStream(0,video::EMST_Tangent);
		if(s)
		{
			m_baseVectors[TANGENT_INDEX].resize(data->count);
			math::vector3d*v=(math::vector3d*) s->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
			mraySystem::memCopy(&m_baseVectors[TANGENT_INDEX][0],v,count);
			s->unlock();
		}else
			m_affectedAttrs=m_affectedAttrs & (~EAA_Tangent);
	}
	if(m_affectedAttrs & EAA_Binormal){
		video::IHardwareStreamBuffer*s=buff->getStream(0,video::EMST_Binormal);
		if(s)
		{
			m_baseVectors[BINORMAL_INDEX].resize(data->count);
			math::vector3d*v=(math::vector3d*) s->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
			mraySystem::memCopy(&m_baseVectors[BINORMAL_INDEX][0],v,count);
			s->unlock();
		}else
		{
			m_affectedAttrs=m_affectedAttrs & (~EAA_Binormal);
		}
	}
}

void SkeletonAnimator::update(float dt){

	if(m_affectedAttrs==0)
		return;

	const BonesList& bones= m_skeleton->getBones();
	BonesList::const_iterator it= bones.begin();

	video::StreamsData* data= m_buffer->getStreamsData();

	video::IHardwareStreamBuffer*mainStream=m_buffer->getStream(0,video::EMST_Position);
	video::IHardwareStreamBuffer*mainNormal=m_buffer->getStream(0,video::EMST_Normal);
	video::IHardwareStreamBuffer*mainTangent=m_buffer->getStream(0,video::EMST_Tangent);
	video::IHardwareStreamBuffer*mainBinormal=m_buffer->getStream(0,video::EMST_Binormal);

	math::vector3d*mainVert=0;
	math::vector3d*mainNorm=0;
	math::vector3d*mainBinorm=0;
	math::vector3d*mainTang=0;

	int start=data->start*sizeof(math::vector3d);
	int count=data->count*sizeof(math::vector3d);

	if(m_affectedAttrs & EAA_Position && mainStream){
		mainVert=(math::vector3d*) mainStream->lock(start,count,video::IHardwareBuffer::ELO_Discard);
		mraySystem::memSet(mainVert,0,count);
	}
	if(m_affectedAttrs & EAA_Normal && mainNormal){
		mainNorm=(math::vector3d*) mainNormal->lock(start,count,video::IHardwareBuffer::ELO_Discard);
		mraySystem::memSet(mainNorm,0,count);
	}
	if(m_affectedAttrs & EAA_Tangent && mainTangent){
		mainTang=(math::vector3d*) mainTangent->lock(start,count,video::IHardwareBuffer::ELO_Discard);
		mraySystem::memSet(mainTang,0,count);
	}
	if(m_affectedAttrs & EAA_Binormal && mainBinormal){
		mainBinorm=(math::vector3d*) mainBinormal->lock(start,count,video::IHardwareBuffer::ELO_Discard);
		mraySystem::memSet(mainBinorm,0,count);
	}


	for (;it!=bones.end();++it)
	{
		BoneNode*b=*it;
		ISkinCluster::SBoneWeights*weights= m_skinCluster->getBoneWeights(b->getID());
		if(!weights || weights->vertices.size()==0)continue;
		
		math::matrix4x4&absTrans = b->getAbsoluteTransformation();
		/*
		const math::matrix4x4&relTrans = b->getRelativeTransformation();
		math::matrix4x4 transMat;

		if(b->getParent()){
			transMat=b->getParent()->getAbsoluteTransformation()*transMat;
		}
		relTrans.getInverse(transMat);
		//transMat=transMat*absTrans;
*/
		math::vector3d basePos	 = b->getAbsoluteBasePosition();

		int cnt=weights->vertices.size();
		float*w=&weights->vertices[0];


		for (int i=0;i<cnt;++i)
		{
			if(w[i]<=math::Epsilon)continue;

				//transMat.transformVectPre(newPos);

			if(m_affectedAttrs & EAA_Position){
				math::vector3d newPos(m_baseVectors[POSITION_INDEX][i]-basePos);
				newPos=absTrans*(newPos);
				mainVert[i]+=(newPos)*w[i];
			}
			if(m_affectedAttrs & EAA_Normal){
				math::vector3d newNorm;
				newNorm=absTrans.transformNormal(m_baseVectors[NORMAL_INDEX][i]);
				mainNorm[i]+=newNorm*w[i];
			}
			if(m_affectedAttrs & EAA_Tangent){
				math::vector3d newTang(m_baseVectors[TANGENT_INDEX][i]);
				absTrans.transformNormal(newTang);
				mainTang[i]+=newTang*w[i];
			}
			if(m_affectedAttrs & EAA_Binormal){
				math::vector3d newBinorm(m_baseVectors[BINORMAL_INDEX][i]);
				absTrans.transformNormal(newBinorm);
				mainBinorm[i]+=newBinorm*w[i];
			}
		}
	}

	if(m_affectedAttrs & EAA_Position){
		mainStream->unlock();
	}
	if(m_affectedAttrs & EAA_Normal){
		mainNormal->unlock();
	}
	if(m_affectedAttrs & EAA_Binormal){
		mainBinormal->unlock();
	}
	if(m_affectedAttrs & EAA_Tangent){
		mainTangent->unlock();
	}
}

Skeleton*SkeletonAnimator::getSkeleton(){
	return m_skeleton;
}

ISkinCluster*SkeletonAnimator::getSkinCluster(){
	return m_skinCluster;
}

IMeshAnimator* SkeletonAnimator::duplicate()
{
	return new SkeletonAnimator(m_buffer,m_skinCluster,m_skeleton,m_affectedAttrs);
}

}
}

