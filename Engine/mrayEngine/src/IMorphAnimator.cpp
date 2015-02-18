#include "stdafx.h"

#include "IMorphAnimator.h"
#include "ILogManager.h"
#include "IMeshBuffer.h"



namespace mray{
namespace scene{

IMorphAnimator::IMorphAnimator(IMeshBuffer*mesh)
{
	m_buffer=mesh;
	video::IHardwareStreamBuffer* s=mesh->getStream(0,video::EMST_Position);
	m_vertexCount=s->length();
}
IMorphAnimator::~IMorphAnimator()
{
	m_targets.clear();
}

IMeshBuffer*IMorphAnimator::getMeshBuffer(){
	return m_buffer;
}
void IMorphAnimator::setMeshBuffer(IMeshBuffer*b)
{
	video::IHardwareStreamBuffer* s=b->getStream(0,video::EMST_Position);
	m_vertexCount=s->length();
	m_buffer=b;
}

void IMorphAnimator::clearTargets()
{
	m_targets.clear();
}

void IMorphAnimator::setTarget(int t,scene::IMeshBuffer*buff,float p)
{
	video::IHardwareStreamBuffer*s=buff->getStream(0,video::EMST_Position);
	if(!s || s->length()!=m_vertexCount){
		gLogManager.log(mT("MorphAnimator::addTarget() - target Vertex Count not matched!"),ELL_WARNING);
		return;
	}
	if(m_targets.size()<=t)
		m_targets.resize(t+1);
	if(m_targets[t].target)
		m_targets[t].target->setMesh(buff);
	else 
	{
		m_targets[t].target=new MorphTarget(buff,p);
		m_targets[t].oldVal=0;
	}

	setTargetPercent(t,p);
}

const MorphTargetPtr& IMorphAnimator::addTarget(scene::IMeshBuffer*buff,float p){
	if(!buff)
		return MorphTargetPtr::Null;
	const video::IHardwareStreamBufferPtr& s=buff->getStream(0,video::EMST_Position);
	if(!s || s->length()!=m_vertexCount){
		gLogManager.log(mT("MorphAnimator::addTarget() - target Vertex Count not matched!"),ELL_WARNING);
		return MorphTargetPtr::Null;
	}
	const MorphTargetPtr& target=new MorphTarget(buff,p);
	m_targets.push_back(MorphTargetInfo(target));

	setTargetPercent(m_targets.size()-1,p);
	return target;
}
int IMorphAnimator::getTargetCount(){
	return m_targets.size();
}
const MorphTargetPtr&IMorphAnimator::getTarget(int index){
	if(index>=m_targets.size())return MorphTargetPtr::Null;
	return m_targets[index].target;
}

void IMorphAnimator::setTargetPercent(int index,float v){
	if(index>=m_targets.size())return;
	MorphTarget*t=m_targets[index].target;
	v=math::clamp(v,0.0f,1.0f);
	if(t->getPercent()!=v){
		t->setPercent(v);
	}
}
float IMorphAnimator::getTargetPercent(int index,float v){
	if(index>=m_targets.size())return 0;
	return m_targets[index].target->getPercent();
}

}
}

