#include "stdafx.h"


#ifdef __TODO__
#include "MeshParticleEmitter.h"
#include <SceneManager.h>
#include <Randomizer.h>
#include <CPropertie.h>


namespace mray{
namespace scene{


MeshParticleEmitter::MeshParticleEmitter()
{
	m_lastMesh=0;
	m_lastVertex=0;
	m_node=0;
}
MeshParticleEmitter::~MeshParticleEmitter(){
	m_meshs.clear();
}

void MeshParticleEmitter::setNode(ISceneNode*node){
	m_node=node;
	if(m_node){
		addMesh(m_node->getMesh());
	}
}

void MeshParticleEmitter::setupParticlePosition(GCPtr<IParticle>particle)
{
	if(m_meshs.size()==0)return;
	if(m_lastMesh>=m_meshs.size())m_lastMesh=0;
	particle->position=m_meshs[m_lastMesh]->getVertexPos(m_lastVertex);

	if(m_node){
		m_node->getAbsoluteTransformation().transformVectPre(particle->position);
	}

	m_lastVertex++;
	GCPtr<video::IHardwareStreamBuffer>s=m_meshs[m_lastMesh]->getStream(0,video::EMST_Position);
	if(m_lastVertex>=s->length()){
		m_lastVertex=0;
		m_lastMesh++;
	}
}



void MeshParticleEmitter::addMesh(GCPtr<SMeshBuffer>mesh){
	m_meshs.push_back(mesh);
}
void MeshParticleEmitter::addMesh(GCPtr<SMesh>mesh){
	for(int i=0;i<mesh->getBuffersCount();++i){
		addMesh(mesh->getBuffer(i));
	}
}

int MeshParticleEmitter::getMeshCount(){
	return m_meshs.size();
}
GCPtr<SMeshBuffer> MeshParticleEmitter::getMesh(int index){
	if(index<0 || index>=m_meshs.size())return 0;
	return m_meshs[index];
}


//////////////////////////////////////////////////////////

const core::string s_psType=mT("MeshPS");

const mchar* SMeshPSFactory::static_getType(){
	return s_psType.c_str();
}
const mchar* SMeshPSFactory::getType(){
	return s_psType.c_str();
}
bool SMeshPSFactory::canCreate(const mchar*type){
	return s_psType.equals_ignore_case(type);
}

SParticleSystem*SMeshPSFactory::createParticleSystem(){
	return new MeshParticleEmitter();
}

}
}








#endif