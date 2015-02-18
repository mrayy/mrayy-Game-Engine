#include "stdafx.h"

#include "GPUMorphAnimator.h"
#include "ILogManager.h"
#include "IMeshBuffer.h"


namespace mray{
namespace scene{

GPUMorphAnimator::GPUMorphAnimator(IMeshBuffer*buff,int posStream,int normalStream,bool useNormals):
	IMorphAnimator(buff)
{
	m_posStream=posStream;
	m_normalStream=normalStream;
	m_useNormals=useNormals;

	m_topTargets=-1;
	m_topTargets.x=0;
	m_targetsCnt=1;

	m_changed=true;
}
GPUMorphAnimator::~GPUMorphAnimator(){
}

void GPUMorphAnimator::update(float dt){
	if(m_targets.size()==0)
		return;

	if(!m_changed)return;
	m_changed=false;

	float l=m_weightVec.x+m_weightVec.y+m_weightVec.z+m_weightVec.w;
	if(l==0){
		m_weightVec[0]=1;
	}else if(math::fEqual(l,1)==0)
	{
		m_weightVec/=l;
	}

	for(int i=0;i<m_targetsCnt;i++){
		MorphTargetInfo& t=m_targets[m_topTargets[i]];
		IMeshBuffer *b=t.target->getMesh();
		if(i==0){
			m_buffer->removeStream(0,video::EMST_Position);
			m_buffer->addStream(0,video::EMST_Position,b->getStream(0,video::EMST_Position));
			if(m_useNormals){
				m_buffer->removeStream(0,video::EMST_Normal);
				m_buffer->addStream(0,video::EMST_Normal,b->getStream(0,video::EMST_Normal));
			}
		}else{
			m_buffer->removeStream(m_posStream+i-1,video::EMST_Texcoord);
			m_buffer->addStream(m_posStream+i-1,video::EMST_Texcoord,
				b->getStream(0,video::EMST_Position));

			if(m_useNormals){
				m_buffer->removeStream(m_normalStream+i-1,video::EMST_Texcoord);
				m_buffer->addStream(m_normalStream+i-1,video::EMST_Texcoord,
					b->getStream(0,video::EMST_Normal));
			}
		}
	}


}

void GPUMorphAnimator::setTargetPercent(int index,float v){
	bool found =false;
	for(int i=0;i<4;i++){
		if(m_topTargets[i]==-1){
			if(m_targetsCnt<4)
				m_targetsCnt++;

			for(int j=3;j>i;j--){
				m_weightVec[j]=m_weightVec[j-1];
				m_topTargets[j]=m_topTargets[j-1];
			}
			m_weightVec[i]=v;
			m_topTargets[i]=index;
			break;
			if(m_weightVec[i]<v){
			}
		}else if(m_topTargets[i]==index){
			m_weightVec[i]=v;
			m_topTargets[i]=index;
			found=true;
			break;
		}
	}
	if(!found)
	{
	}
	IMorphAnimator::setTargetPercent(index,v);
	m_changed=true;
}

IMeshAnimator* GPUMorphAnimator::duplicate()
{
	GPUMorphAnimator* c=new GPUMorphAnimator(m_buffer,m_posStream,m_normalStream,m_useNormals);

	for(int i=0;i<m_targets.size();++i)
	{
		MorphTargetInfo ifo(new MorphTarget(m_targets[i].target->getMesh(),m_targets[i].target->getPercent()));
		c->m_targets.push_back(ifo);
	}

	return c;
}

}
}
