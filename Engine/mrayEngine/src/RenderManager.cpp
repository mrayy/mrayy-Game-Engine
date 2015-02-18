#include "stdafx.h"

#include "RenderManager.h"
#include "RenderGroup.h"

#include "ThreadBarrier.h"
#include "JobPool.h"
#include "IMutex.h"
#include "IThreadManager.h"
#include "MutexLocks.h"
#include "ThreadJobManager.h"
#include "Engine.h"
#include "IVideoDevice.h"

namespace mray{
namespace scene{



	class RenderManagerSortJob:public IJobPiece//JobOrder
	{
		RenderManager* m_manager;
	public:
		RenderManagerSortJob(RenderManager*mngr):m_manager(mngr)
		{
		}
		bool Execute()
		{
			m_manager->_SortNodes();
			return true;
		}
	};
RenderManager::RenderManager(ISceneManager* sceneManager){
	m_sceneManager=sceneManager;

	m_sortNodesJob=new RenderManagerSortJob(this);
//	m_sortNodesCompleted= newClassDelegate2<RenderManager,void,JobOrder*,bool>("",this,&RenderManager::OnSortNodesCompleted);
	m_sortNodesJobBarrier=new OS::ThreadBarrier();
	//m_sortNodesJob->SetTargetQueue(EDefaultJobQ_Scene);

	m_threadManager=new ThreadJobManager();
	m_threadManager->AddPiece(m_sortNodesJob);

	getRenderGroup(RGH_Solid)->setSortType(IRenderGroup::Sort_None);
	getRenderGroup(RGH_Transparent)->setSortType(IRenderGroup::Sort_DES);
}
RenderManager::~RenderManager(){
	{
		RenderGroupMap::iterator it=m_renderGroups.begin();
		for (;it!=m_renderGroups.end();++it)
		{
			delete it->second;
		}
	}
	m_renderGroups.clear();

	delete m_threadManager;

	delete m_sortNodesJobBarrier;
//	delete m_sortNodesCompleted;
	delete m_sortNodesJob;
	m_sortNodesJob=0;
}

IRenderGroup *RenderManager::getRenderGroup(int idx){

	IRenderGroup*grp=0;
	RenderGroupMap::iterator it=m_renderGroups.find(idx);
	if(it==m_renderGroups.end()){
		grp=new RenderGroup(this,idx);
		m_renderGroups.insert(RenderGroupMap::value_type(idx,grp));
	}else{
		grp=it->second;
	}

	return grp;
}

void RenderManager::startSorting()
{
	
//	m_sortNodesJobBarrier->Lock();
//	JobPool::getInstance().AddRequest(m_sortNodesJob,m_sortNodesCompleted);

	m_threadManager->BeginProcessing();
	
/*
	_SortNodes();*/
}
void RenderManager::addOperation(int group,RenderOperation &op)
{
	IRenderGroup*grp=getRenderGroup(group);
	grp->addRenderOperation(op);
}

void RenderManager::endSorting()
{/*
	if(m_sortNodesJobBarrier->GetLocksCount()>0)
	m_sortNodesJobBarrier->Wait();//wait until the sorting complete*/
	//m_sortNodesJobBarrier->Block(2);
	m_threadManager->EndProcessing();
}


void RenderManager::renderAll(IRenderPass*pass)
{
	//endSorting();//make sure that the sorting is done
	RenderGroupMap::iterator it=m_renderGroups.begin();
	int i=0;
	for(;it!=m_renderGroups.end();++it){
		m_currGroup=it->first;
		//Engine::getInstance().getDevice()->clearBuffer(video::EDB_DEPTH);
		it->second->performRender(pass);
		++i;
	}
}

void RenderManager::clear(){

	RenderGroupMap::iterator it=m_renderGroups.begin();
	RenderGroupMap::iterator end=m_renderGroups.end();
	for(;it!=end;++it){
		it->second->clear();
	}
}

int RenderManager::getCurrGroup(){
	return m_currGroup;
}

void RenderManager::_SortNodes(){

	RenderGroupMap::iterator it=m_renderGroups.begin();
	for(;it!=m_renderGroups.end();++it){
		it->second->sort();
	}
//	m_sortNodesJobBarrier->Block(2);
}
/*void RenderManager::OnSortNodesCompleted(JobOrder*const &o,const bool&s)
{
//	m_sortNodesJobBarrier->Unlock();
}*/

}
}