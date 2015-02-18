#include "stdafx.h"

#include "MorphAnimator.h"
#include "ILogManager.h"
#include "IMeshBuffer.h"

namespace mray{
namespace scene{


MorphAnimator::MorphAnimator(IMeshBuffer*mesh)
	:IMorphAnimator(mesh)
{
	m_needUpdate=false;
	setMeshBuffer(mesh);
}
MorphAnimator::~MorphAnimator(){
	m_targets.clear();
}

void MorphAnimator::setMeshBuffer(IMeshBuffer*mesh)
{
	m_needUpdate=false;
	m_buffer=mesh;
	video::IHardwareStreamBuffer*s=mesh->getStream(0,video::EMST_Position);
	m_tempVector.resize(m_vertexCount);
	m_baseVector.resize(m_vertexCount);
	math::vector3d*mainVert=(math::vector3d*) s->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
	for (int v=0;v<m_baseVector.size();v++){
		m_baseVector[v]=mainVert[v];
	}
	s->unlock();
}

void MorphAnimator::update(float dt){

	m_needUpdate=false;
	for (int i=0;i<m_targets.size();++i)
	{
		if(m_targets[i].target->getPercent()!=m_targets[i].oldVal){
			m_needUpdate=true;
			break;
		}
	}
	if(!m_needUpdate)return;
	m_needUpdate=false;

	for (int i=0;i<m_tempVector.size();++i)
	{
		m_tempVector[i]=0;
	}

	float total=1;
	bool changed=0;
	for(int i=0;i<m_targets.size();++i){
		MorphTarget*t=m_targets[i].target;
		if(t->getPercent()==0)continue;
		m_targets[i].oldVal=t->getPercent();
		float p=m_targets[i].oldVal*0.01f;
		video::IHardwareStreamBuffer*stream=t->getMesh()->getStream(0,video::EMST_Position);
		math::vector3d*pV=(math::vector3d*) stream->lock(0,0,video::IHardwareBuffer::ELO_ReadOnly);
		int cnt=stream->length();
		for (int v=0;v<cnt;v++)
		{
			m_tempVector[v]+=pV[v]*p;
		}
		total-=p;
		stream->unlock();
		changed=true;
	}
	if(changed){
		video::IHardwareStreamBuffer*mainStream=m_buffer->getStream(0,video::EMST_Position);
		math::vector3d*mainVert=(math::vector3d*) mainStream->lock(0,0,video::IHardwareBuffer::ELO_Normal);
		int len=mainStream->length();
		for (int v=0;v<m_tempVector.size();v++){
			mainVert[v]=m_tempVector[v]+m_baseVector[v]*total;
		}
		mainStream->unlock();
	}

}

const MorphTargetPtr& MorphAnimator::addTarget(scene::IMeshBuffer*buff,float p){
	const MorphTargetPtr& t=IMorphAnimator::addTarget(buff,p);

	if(p!=0 && t.pointer()!=0)
		m_needUpdate=true;
	return t;
}

void MorphAnimator::setTargetPercent(int index,float v){
	if(index>=m_targets.size())return;
	MorphTarget*t=m_targets[index].target;
	v=math::clamp(v,0.0f,1.0f);
	if(t->getPercent()!=v){
		m_needUpdate=true;
		t->setPercent(v);
	}
}

IMeshAnimator* MorphAnimator::duplicate()
{
	MorphAnimator* c=new MorphAnimator(m_buffer);

	for(int i=0;i<m_targets.size();++i)
	{
		MorphTargetInfo ifo(new MorphTarget(m_targets[i].target->getMesh(),m_targets[i].target->getPercent()));
		c->m_targets.push_back(ifo);
	}

	return c;
}
}
}
