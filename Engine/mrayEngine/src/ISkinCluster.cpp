
#include "stdafx.h"
#include "ISkinCluster.h"
#include "IMeshBuffer.h"

#include "Skeleton.h"
#include "BoneNode.h"

namespace mray{
namespace scene{

ISkinCluster::ISkinCluster(Skeleton*skeleton)
{
	buildWeights(skeleton);
	//bonesWeights.sort();
}


ISkinCluster::~ISkinCluster(){
	bonesWeights.clear();
}


void ISkinCluster::buildWeights(Skeleton*skeleton){
	if( !skeleton)return;

	const BonesList& bones= skeleton->getBones();
	BonesList::const_iterator it=bones.begin();

	SBoneWeights w1;
	for(;it!=bones.end();++it){
		w1.boneIndex=(*it)->getID();
		w1.name=(*it)->getNodeName();
		bonesWeights.push_back(w1);
	}
}
void ISkinCluster::setBoneWeights(int bone,int arrSize,float*weights){
	SBoneWeights*tmp=getBoneWeights(bone);
	if(!tmp)return;
	tmp->vertices.resize(arrSize);
	for(int i=0;i<arrSize;++i){
		tmp->vertices[i]=weights[i];
	}
	std::sort(tmp->vertices.begin(),tmp->vertices.end());
	//tmp->vertices.sort();

}
ISkinCluster::SBoneWeights*ISkinCluster::getBoneWeights(int bone){
//	bonesWeights.sort();
	/*
	int l=0;
	int h=bonesWeights.size()-1;
	int m;
	while(l<=h){
		m=(l+h)/2;
		if(bonesWeights[m].boneIndex==bone)
			return &bonesWeights[m];
		else if(bonesWeights[m].boneIndex<bone){
			l=m+1;
		} 
		else h=m-1;
	}*/

	BoneWeightList::iterator it=bonesWeights.begin();
	for (;it!=bonesWeights.end();++it)
	{
		if((*it).boneIndex==bone)
			return &*it;
	}
	return 0;
}

ISkinCluster::SBoneWeights* ISkinCluster::getBoneWeights(const core::string&name)
{
	BoneWeightList::iterator it=bonesWeights.begin();
	for (;it!=bonesWeights.end();++it)
	{
		if((*it).name==name){
			SBoneWeights&res=*it;
			return &res;
		}
	}
	return 0;
}

void ISkinCluster::setVertexWeight(int bone,int vtx,float weight){
	SBoneWeights*tmp=getBoneWeights(bone);
	if(!tmp)return;
	tmp->vertices[vtx]=weight;

}
float ISkinCluster::getVertexWeight(int bone,int vtx){
	SBoneWeights*tmp=getBoneWeights(bone);
	if(!tmp)return 0;
	return tmp->vertices[vtx];
}



void ISkinCluster::calcWeights(IMeshBuffer *mesh,Skeleton*skeleton,float minR,float maxR)
{
	if( !skeleton)return;

	float minRsqr=minR*minR;
	float maxRsqr=maxR*maxR;

	float iDiff=1.0f/(maxR-minR);

	bonesWeights.clear();

	SBoneWeights w1;
	const BonesList& bones= skeleton->getBones();

	BoneNode* parent;

	video::IHardwareStreamBuffer*vS=mesh->getStream(0,video::EMST_Position);
	math::vector3d*vert=(math::vector3d*)vS->lock(0,0,video::IHardwareStreamBuffer::ELO_ReadOnly);
	int cnt	  =vS->length();
	BonesList::const_iterator it= bones.begin();

	w1.vertices.resize(cnt);
	math::line3d boneLine;

	for(;it!=bones.end();++it){
		BoneNode* bone=*it;

		parent=bone->getParentBone();
		if(parent==0){
			continue;
			//boneLine.pStart=bone->getAbsoluteBasePosition();
		}else
			boneLine.pStart=parent->getAbsoluteBasePosition();
		w1.boneIndex=parent->getID();
		boneLine.pEnd=bone->getAbsoluteBasePosition();
		float sum=0;
		for(int i=0;i<cnt;++i){
			math::vector3d p=boneLine.ClosestPoint(vert[i]);
			float dist=p.getDistSQ(vert[i]);
			if(dist>maxRsqr)
			{
				w1.vertices[i]=0;
			}else if(dist<minRsqr)
			{
				w1.vertices[i]=1;
			}else{
				dist=sqrt(dist);
				w1.vertices[i]=(maxR-dist)*iDiff;
				if(w1.vertices[i]<=0.1){
					w1.vertices[i]=0;
				}
			}
		}
		bonesWeights.push_back(w1);
	}
	vS->unlock();
	for(int v=0;v<cnt;v++){
		float sum=0;
		BoneWeightList::iterator it=bonesWeights.begin();
		for(;it!=bonesWeights.end();++it){
			sum+=math::sqr((*it).vertices[v]);
		}

		float isum;
		if(sum>0){
			//sum=sqrt(sum);
			isum=1.0f/sum;
			it=bonesWeights.begin();
			for(;it!=bonesWeights.end();++it){
				(*it).vertices[v]*=isum;
			}
		}
	}
}



}
}


