#include "stdafx.h"
 

#include "SMesh.h"
#include "SMeshBuffer.h"
#include "MeshAnimatorManager.h"
#include "MeshBufferData.h"
#include "IJobPiece.h"
#include "ThreadJobManager.h"

namespace mray{
namespace scene{


	class MeshUpdateJobPiece:public IJobPiece
	{
		SMesh* m_mesh;
	public:
		float dt;

		MeshUpdateJobPiece(SMesh* mesh):m_mesh(mesh),dt(0)
		{
		}
		virtual bool Execute()
		{
			m_mesh->update(dt);
			return true;
		}
	};

SMesh::SMesh():lastCycle(-1),lastFrame(-1)
{
	m_jobPiece=new MeshUpdateJobPiece(this);
};

SMesh::~SMesh()
{
	clear();
	delete m_jobPiece;
}
void SMesh::clear()
{
	for(int i=0;i<m_meshData.size();++i)
	{
		delete m_meshData[i];
	}
	m_meshData.clear();
}

void SMesh::setSkeleton(const SkeletonPtr& s)
{
	m_skeleton=s;
}
const SkeletonPtr& SMesh::getSkeleton()
{
	return m_skeleton;
}

#ifdef OLD_MESH_CODE
void SMesh::getStartEndFrames(int CycleIndex,float &frame,int &sout,int &eout)
{
	if(CycleIndex>=Cycles->totalCycles ){
		sout=eout=0;
		return;
	}
	if(CycleIndex<0){
		sout=eout=-1;
		return;
	}
	int frameStart;
	int frameEnd;
	int framePos;
	int i=CycleIndex;
	float outFrame=0;
	frame=fmod(frame,100);
	if(frame<0)frame+=100;
	int sframeIdx=Cycles->CycleProp[i].cycleIndex[0];
	int eframeIdx=Cycles->CycleProp[i].cycleIndex[Cycles->CycleProp[i].count-1];
	frameStart=0;
	frameEnd=Cycles->CycleProp[i].count-1;
	outFrame  =sframeIdx+(float)(eframeIdx-sframeIdx)*(frame/100.0f);
	
	for(int j=1;j<Cycles->CycleProp[i].count;j++)
	{
		if(Cycles->CycleProp[i].cycleIndex[j]>=outFrame)
		{
			outFrame-=frameStart;
			frameStart=j-1 + Cycles->CycleProp[i].start;
			frameEnd=j+ Cycles->CycleProp[i].start;
			frame=(float)(outFrame-Cycles->CycleProp[i].cycleIndex[j-1])*100.0f/
				(float)(Cycles->CycleProp[i].cycleIndex[j]-
				Cycles->CycleProp[i].cycleIndex[j-1]);
			break;
		}
	}
	sout=frameStart;
	eout=frameEnd;
}

void SMesh::animate(float frame,bool allowSkeleton){
	if(allowSkeleton && m_skeleton){
		animateSkeleton(frame);
	}
	else
		animateMesh(frame);
}
void SMesh::animate(int CycleIndex,float frame,bool allowSkeleton){
	if(allowSkeleton && m_skeleton){
		animateSkeleton(CycleIndex,frame);
	}
	else
		animateMesh(CycleIndex,frame);
}

void SMesh::animateSkeleton(int CycleIndex,float frame){
	if(CycleIndex<0)return;
	
	if(frame==lastFrame && CycleIndex==lastCycle)return;
	lastFrame=frame;
	lastCycle=CycleIndex;

	animateSkeleton(m_skeleton->Skeleton->Name().c_str(),CycleIndex,frame);

}
void SMesh::animateSkeleton(const  mchar*jointName,int CycleIndex,float frame){


	if(CycleIndex<0)return;
	skeleton->animate(jointName,CycleIndex,frame);
	calcCurrBoundingBox();

}
void SMesh::animateSkeleton(float frame){

	animateSkeleton(Cycles->CurrCycle,frame);

}


void SMesh::animateMesh(int CycleIndex,float frame)
{
	if(CycleIndex<0)return;
	Cycles->CurrCycle=CycleIndex;
	Cycles->CurrFrame=frame;
	Cycles->cycleType=CT_ANIM;
	int frameStart,frameEnd;
	getStartEndFrames(CycleIndex,frame,frameStart,frameEnd);
	Cycles->startFrame=frameStart;
	Cycles->endFrame=frameEnd;
	
	animateMesh(frameStart,frameEnd,frame);
		
	recalcLocatorsPos(frame);
}

void SMesh::animateMesh(float frame)
{
	animateMesh(Cycles->CurrCycle,frame);
}
void SMesh::animateMesh(const mchar*meshName,int cycleIndex,float frame)
{
	GCPtr<SMeshBuffer>mesh=getBufferByName(meshName);
	if(!mesh)return;
	
	if(cycleIndex<0)return;
	Cycles->cycleType=CT_ANIM;
	int frameStart,frameEnd;
	frame=fmod(frame,100);
	getStartEndFrames(cycleIndex,frame,frameStart,frameEnd);
	Cycles->startFrame=frameStart;
	Cycles->endFrame=frameEnd;
	if(frameStart<0 || frameEnd<0)return;
	mesh->updateMeshCurrFrame(frameStart,frameEnd,frame);
	calcCurrBoundingBox();
	recalcLocatorsPos(frame);
}

void SMesh::animateMesh(int startFrame,int endFrame,float frame)
{
	for(uint i=0;i<m_meshData.size();++i)
	{
		m_meshData[i]->updateMeshCurrFrame(startFrame,endFrame,frame);
	}
	calcCurrBoundingBox();
}

void SMesh::blendAnimation(int frame,float blend)
{
	for(uint i=0;i<m_meshData.size();++i)
	{
		m_meshData[i]->blend(frame,blend);
	}
	calcCurrBoundingBox();
}

void SMesh::blendAnimation(int c1SFrame,int c1EFrame,float c1frame,int frame,float blend)
{
	for(uint i=0;i<m_meshData.size();++i)
	{
		m_meshData[i]->cycleBlend(c1SFrame,c1EFrame,c1frame,frame,blend);
	}
	calcCurrBoundingBox();
}

void SMesh::blendAnimation(int blendFrame,float currFrame,float blend)
{
	if(blendFrame<0 || currFrame<0)return;
	Cycles->cycleType=CT_BLEND;
	blendFrame=Cycles->CycleProp[blendFrame].start;
	blend=fmod(blend,100);
	
	currFrame=fmod(currFrame,100);
	int frameStart,frameEnd;
	getStartEndFrames(Cycles->CurrCycle,currFrame,frameStart,frameEnd);
	Cycles->startFrame=frameStart;
	Cycles->endFrame=frameEnd;
	Cycles->blendFrame=blendFrame;
	Cycles->blendAmount=blend;

	blendAnimation(frameStart,frameEnd,currFrame,blendFrame,blend);
	recalcLocatorsPos(currFrame);

}

#endif

void SMesh::update(float dt){
	if(m_skeleton)
	{
		m_skeleton->update(dt);
		m_worldMatricies= m_skeleton->GetBonesMatricies();
	}
	for(int i=0;i<m_meshData.size();++i)
	{
		m_meshData[i]->update(dt);
	}
}
void SMesh::AddUpdateJobs(ThreadJobManager* mngr,float dt)
{
	((MeshUpdateJobPiece*)m_jobPiece)->dt=dt;
	mngr->AddPiece(m_jobPiece);
}

int SMesh::getBuffersCount()
{
	return m_meshData.size();
}

MeshBufferData*SMesh::addNewBuffer(){
	IMeshBufferPtr b=new SMeshBuffer();
	MeshBufferData* bd=new MeshBufferData();
	bd->setMeshBuffer(b);
	m_meshData.push_back(bd);
	return m_meshData[m_meshData.size()-1];
}



const IMeshBufferPtr&SMesh::getBuffer(int index){
	if(index<0 || index>=m_meshData.size())
		return IMeshBufferPtr::Null;
	return m_meshData[index]->getMeshBuffer();
}
MeshBufferData*SMesh::getBufferData(int index){
	if(index<0 || index>=m_meshData.size())
		return 0;
	return m_meshData[index];
}
MeshBufferData* SMesh::addBuffer(const IMeshBufferPtr&b){
	if(!b)return MeshBufferDataPtr::Null;
	MeshBufferData* bd=new MeshBufferData();
	bd->setMeshBuffer(b);
	if(m_meshData.size()==0)
		boundingBox=b->getBoundingBox();
	else boundingBox.AddBox(b->getBoundingBox());
	m_meshData.push_back(bd);
	return m_meshData[m_meshData.size()-1];
}
int SMesh::getBufferIndex(const mchar* name){
	core::string sname=name;
	for(uint i=0;i<m_meshData.size();++i)
	{
		if(m_meshData[i]->getMeshBuffer()->getMeshName().equals_ignore_case(sname))
			return i;
	}
	return -1;
}

void SMesh::removeBuffer(int index)
{
	if(index<0 || index>=m_meshData.size())
		return;
	std::vector<MeshBufferData*>::iterator it=m_meshData.begin();
	std::advance(it,index);
	m_meshData.erase(it);
}

const IMeshBufferPtr&SMesh::getBufferByName(const mchar*name)
{
	int i=getBufferIndex(name);
	if(i==-1)
		return IMeshBufferPtr::Null;
	return m_meshData[i]->getMeshBuffer();
}

math::box3d SMesh::calcCurrBoundingBox()
{
	boundingBox.reset(0);
	if(m_meshData.size()>0){
		//m_meshData[0]->getMeshBuffer()->recalculateBoundingBox();
		boundingBox=m_meshData[0]->getMeshBuffer()->getBoundingBox();
	}
	for(uint i=1;i<m_meshData.size();++i)
	{
		//m_meshData[i]->getMeshBuffer()->recalculateBoundingBox();
		boundingBox.AddBox(m_meshData[i]->getMeshBuffer()->getBoundingBox());
	}
	return boundingBox;
}


/*
math::vector3d SMesh::getVertexPos(int meshIndex,int vertex)
{
	if(meshIndex>=m_meshData.size())return 0;
	return m_meshData[meshIndex]->getVertexPos(vertex);
}

math::vector3d SMesh::getVertexPos(const mchar* meshName,int vertex)
{
	for(uint i=0;i<m_meshData.size();++i)
		if(m_meshData[i]->Name.equals_ignore_case(meshName))
			return m_meshData[i]->getVertexPos(vertex);
	return 0;
}*/

#ifdef OLD_MESH_CODE
void SMesh::recalcLocatorsPos(float frame){

	for(int i=0;i<Cycles->locators.size();++i){
		if(Cycles->cycleType==CT_ANIM){
			Cycles->locators[i]->setCurrFrame(frame,Cycles->startFrame,Cycles->endFrame);
		}else{
			Cycles->locators[i]->setCurrFrame(frame,Cycles->startFrame,Cycles->endFrame,
				Cycles->blendAmount,Cycles->blendFrame);
		}
	}
}



bool SMesh::getLocatorPos(int index,math::vector3d&out){
	if(index<0 || index>=Cycles->locators.size())return 0;
	float frame=Cycles->CurrFrame;
	int frameStart=0,frameEnd=0;
	bool flag=0;
	getStartEndFrames(Cycles->CurrCycle,frame,frameStart,frameEnd);

	if(Cycles->cycleType==CT_ANIM){
		if(Cycles->locators[index]->getFramePos(frame,frameStart,frameEnd,out))
			flag=1;
	}else
		if(Cycles->cycleType==CT_BLEND){
			if(Cycles->locators[index]->getFramePos(frame,frameStart,frameEnd,
				Cycles->blendAmount,Cycles->blendFrame,out))
				flag=1;
		}
	return flag;
	return 0;
}
bool SMesh::getLocatorOrintation(int index,math::quaternion&out){
	if(index<0 || index>=Cycles->locators.size())return 0;
	math::vector3d p=0;
	bool flag=0;
	float frame=Cycles->CurrFrame;
	int frameStart=0,frameEnd=0;
	getStartEndFrames(Cycles->CurrCycle,frame,frameStart,frameEnd);
	if(Cycles->cycleType==CT_ANIM){
		if(Cycles->locators[index]->getFrameOrintation(frame,frameStart,frameEnd,out))
			flag=1;
	}else
		if(Cycles->cycleType==CT_BLEND){
			if(Cycles->locators[index]->getFrameOrintation(frame,frameStart,frameEnd,
				Cycles->blendAmount,Cycles->blendFrame,out))
				flag=1;
		}
	return flag;
}

bool SMesh::getLocatorScale(int index,math::vector3d&out){
	
	if(index<0 || index>=Cycles->locators.size())return 0;
	bool flag=0;
	float frame=Cycles->CurrFrame;
	int frameStart=0,frameEnd=0;
	getStartEndFrames(Cycles->CurrCycle,frame,frameStart,frameEnd);
	if(Cycles->cycleType==CT_ANIM){
		if(Cycles->locators[index]->getFramePos(frame,frameStart,frameEnd,out))
			flag=1;
	}else
		if(Cycles->cycleType==CT_BLEND){
			if(Cycles->locators[index]->getFramePos(frame,frameStart,frameEnd,
				Cycles->blendAmount,Cycles->blendFrame,out))
				flag=1;
		}
	return flag;
}


void SMesh::setSkeleton(SAnimationJoint*other){
	if(!other)return;
	bool flag=0;
	if(!skeleton){
		skeleton=new SAnimationJoint(other->animName.c_str());
		skeleton->Skeleton=new SJointNode(other->Skeleton->Name().c_str(),0);
		copySkeleton(other->Skeleton,skeleton->Skeleton);
		flag=1;
	}
	skeleton->Skeleton->copySkeleton(other->Skeleton,flag);
	skeleton->Cycles=other->Cycles;
}
	
void SMesh::copySkeleton(GCPtr<SJointNode>j1,GCPtr<SJointNode>j2){
	if(!j1 || !j2)return;

	j2->getFrames(j1->frames);
	std::list<GCPtr<ISceneNode> >::iterator it=j1->getChildren()->begin();
	for(;it!=j1->getChildren()->end();++it){
		GCPtr<scene::SJointNode>joint=new scene::SJointNode((*it)->Name().c_str(),j2);
		copySkeleton((GCPtr<SJointNode>)(*it),joint);
		j2->addChild(joint);
	}
}

#endif
scene::SMesh* SMesh::getDuplicate(){
	scene::SMesh*mesh=new SMesh();
	mesh->setResourceName(getResourceName());
	//TODO :duplicate "skeleton"
	mesh->boundingBox=boundingBox;

	mesh->m_meshData.resize(m_meshData.size());

	for(int i=0;i<mesh->m_meshData.size();++i){
		mesh->m_meshData[i]=m_meshData[i]->getDuplicate();
	}
	if(m_skeleton)
		mesh->setSkeleton(m_skeleton->getDuplicate());

#ifdef OLD_MESH_CODE
	if(Cycles)
		mesh->Cycles=Cycles->getDuplicate();
#endif

	mesh->setupShaderAndAnimators();
	return mesh;
}

scene::SMesh* SMesh::duplicateContextOnly(){
	scene::SMesh* mesh=new SMesh();
	mesh->setResourceName(getResourceName());
	//TODO :duplicate "skeleton"
	mesh->boundingBox=boundingBox;

	mesh->m_meshData.resize(m_meshData.size());

	if(m_skeleton)
		mesh->setSkeleton(m_skeleton->getDuplicate());

	for(int i=0;i<mesh->m_meshData.size();++i){
		mesh->m_meshData[i]=m_meshData[i]->duplicateContext();
	}

	mesh->setupShaderAndAnimators();

	return mesh;
}

uint SMesh::calcSizeInternal(){
	uint size=0;
	for(int i=0;i<m_meshData.size();++i){
		size+=m_meshData[i]->calcSizeInternal();
	}
	return size;
}


void SMesh::setupShaderAndAnimators()
{
	for(int i=0;i<getBuffersCount();++i)
	{
		scene::MeshBufferData*bd=getBufferData(i);
/*
		if(m_skeleton && scene::MeshAnimatorManager::getInstance().IsGPUBasedAnimators())
		{
			bd->setShaderCallback(scene::MeshAnimatorManager::getInstance().CreateSkeletonShaderCallback(anim));
		}
		
		if(bd->getMeshBuffer()->getSkinCluster()){
			GCPtr<scene::ISkeletonAnimator> anim=scene::MeshAnimatorManager::getInstance().CreateSkeletonAnimator(bd->getMeshBuffer(),bd->getMeshBuffer()->getSkinCluster()
				,getSkeleton());
			bd->setAnimator(anim);
			if(scene::MeshAnimatorManager::getInstance().IsGPUBasedAnimators())
			{
				bd->setShaderCallback(scene::MeshAnimatorManager::getInstance().CreateSkeletonShaderCallback(anim));
			}
		}else
		{
			if(scene::MeshAnimatorManager::getInstance().IsGPUBasedAnimators())
			{
				bd->setShaderCallback(scene::MeshAnimatorManager::getInstance().CreateMorphShaderCallback(bd->getMorphAnimator()));
			}
		}*/
	}
}

}
}
