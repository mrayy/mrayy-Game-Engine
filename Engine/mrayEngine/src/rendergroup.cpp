#include "stdafx.h"

#include "RenderGroup.h"
#include "RenderManager.h"
#include "ISceneManager.h"
#include "CameraNode.h"
#include "MeshBufferData.h"
#include "ShaderSemanticTable.h"
#include "IVideoDevice.h"
#include "ITimer.h"
#include "BenchmarkItem.h"
#include "ShaderSemanticTable.h"
#include "SMesh.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "Engine.h"
#include <algorithm>


namespace mray{
namespace scene{


	struct DepthSortFunction
	{
		const CameraNode* camera;
		RenderGroup::ESortType sortType;

		DepthSortFunction(const CameraNode* cam ,RenderGroup::ESortType s)
			: camera(cam),sortType(s)
		{
		}

		bool operator()(const RenderOperation* a, const RenderOperation* b) const
		{
			if(sortType==RenderGroup::Sort_None)
				return 0;
			// Different renderables, sort by depth
			float adepth = a->node->getOwner()->getAbsolutePosition().getDistSQ(camera->getAbsolutePosition());
			float bdepth = b->node->getOwner()->getAbsolutePosition().getDistSQ(camera->getAbsolutePosition());

			return sortType==RenderGroup::Sort_ASC ? adepth < bdepth : adepth > bdepth;
		}
	};

	const RenderGroup::ESortGroupType RenderGroup::s_SortGroupCreation[ESGT_SortGroupCount]=
	{
		ESGT_None,				//None -> None
		ESGT_Material,			//Base -> Materials
		ESGT_Streams,			//Materials -> Streams
		ESGT_RenderOperation,	//Streams -> RenderOp
		ESGT_None				//RenderOp -> None
	};
	RenderGroup::ISortGroup* RenderGroup::ISortGroup::CreateSortGroup(RenderGroup::ESortGroupType type)
	{
		switch(type)
		{
		case ESGT_None:
			return 0;
		case ESGT_BaseSortGroup:
			return new SBaseSortGroup();
		case ESGT_Material:
			return new RenderPassGroup();
		case ESGT_Streams:
			return new SVBGroup();
		case ESGT_RenderOperation:
			return new SRenderOpGroup();
		}
		return 0;
	}


	RenderGroup::SRenderOpGroup::~SRenderOpGroup()
	{
		Clear();
	}
	void RenderGroup::SRenderOpGroup::addOperation(RenderOperation*op)
	{
		m_renderOperations.push_back(op);
	}
	void RenderGroup::SRenderOpGroup::render(RenderGroup*grp)
	{
		video::IVideoDevice*dev= grp->getDevice();
		PRenderOperationList::iterator it=m_renderOperations.begin();
		IRenderPass*pass=grp->getRenderManager()->getSceneManager()->getCurrnetRenderPass();

		for(;it!=m_renderOperations.end();++it){
			RenderOperation*op=*it;

			if( !pass->canRenderObject(op->node))
				continue;

// 			video::RenderTechnique* teq=op->mesh->getMaterial()->GetActiveTechnique();
// 			const video::RenderPassList& passes= teq->GetPasses();
// 
// 			for(int i=0;i<passes.size();++i)
// 			{
// 				video::RenderPass* p=passes[i];
// 				dev->useRenderPass(p);
				video::GPUShader* shader=pass->getMaterialShader(op->pass);

				math::matrix4x4&m=op->node->getOwner()->getAbsoluteTransformation();
				if(op->node->IsUseIdentityMatrix())
				{
					m=math::matrix4x4::Identity;
				}
				if(!shader || !shader->GetVertexShader())
					dev->setTransformationState(video::TS_WORLD,m);
				else{
					video::ShaderSemanticTable::getInstance().setWorldMatrix(m);
					if(op->node->getMesh()->GetWorldMatricies().size()>0)
						video::ShaderSemanticTable::getInstance().setWorldMatricies(&op->node->getMesh()->GetWorldMatricies()[0],op->node->getMesh()->GetWorldMatricies().size());
					else
						video::ShaderSemanticTable::getInstance().setWorldMatricies(0,0);
					if(!op->node->IsUseIdentityMatrix())
						video::ShaderSemanticTable::getInstance().setPrevWorldMatrix(op->node->getOwner()->getPrevAbsoluteTransformation());
					else
						video::ShaderSemanticTable::getInstance().setPrevWorldMatrix(m);
				}
				video::ShaderSemanticTable::getInstance().setSkeleton(op->node->getMesh()->getSkeleton());
				op->mesh->getShaderCallback()->setConstants(shader,video::IShaderConstantsCallback::ESUT_Object);

				IMeshBuffer* mb=op->mesh->getMeshBuffer();

				dev->drawElements( mb->getIndexData(),mb->getRenderType());

				video::ShaderSemanticTable::getInstance().setWorldMatricies(0,0);
			}
		//}
	}


	void RenderGroup::SRenderOpGroup::Clear()
	{
		m_renderOperations.clear();
	}

//////////////////////////////////////////////////////////////////////////


RenderGroup::RenderPassGroup::~RenderPassGroup()
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		delete *it;
	}
	m_subGroup.clear();
}
void RenderGroup::RenderPassGroup::addOperation(RenderOperation*op)
{
	if(isNew)
	{
		isNew=false;
		mtrl=op->pass;
	}
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		if((*it)->CanBeAdded(op))
		{
			(*it)->addOperation(op);
			return;
		}
	}
	ISortGroup* g=ISortGroup::CreateSortGroup(s_SortGroupCreation[GetType()]);
	if(g)
	{
		if(m_subGroup.empty())
		{
			mtrl=op->pass;
		}
		g->addOperation(op);
		m_subGroup.push_back(g);
	}
}

void RenderGroup::RenderPassGroup::render(RenderGroup*grp)
{
	if (isNew)
		return;
	video::IShaderConstantsCallback c;
	video::IVideoDevice*dev= grp->getDevice();
	IRenderPass*pass=grp->getRenderManager()->getSceneManager()->getCurrnetRenderPass();

	if(pass->useMaterial(mtrl))
		dev->useRenderPass(mtrl);
	c.setConstants(pass->getMaterialShader(mtrl),video::IShaderConstantsCallback::ESUT_Global);

	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{

		(*it)->render(grp);
	}
}

bool RenderGroup::RenderPassGroup::CanBeAdded(RenderOperation*op)
{
	if(isNew || op->pass==mtrl)
		return true;
	return false;

}

void RenderGroup::RenderPassGroup::Clear()
{
	isNew=true;
	mtrl = 0;
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		(*it)->Clear();
	}
	//m_subGroup.clear();
}



//////////////////////////////////////////////////////////////////////////

RenderGroup::SVBGroup::~SVBGroup()
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		delete *it;
	}
	m_subGroup.clear();
}


void RenderGroup::SVBGroup::addOperation(RenderOperation*op)
{
	if(isNew)
	{
		isNew=false;
		stream=op->mesh->getMeshBuffer()->getStreamsData().pointer();
	}
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		if((*it)->CanBeAdded(op))
		{
			(*it)->addOperation(op);
			return;
		}
	}
	ISortGroup* g=ISortGroup::CreateSortGroup(s_SortGroupCreation[GetType()]);
	if(g)
	{
		if(m_subGroup.empty())
		{
			stream=op->mesh->getMeshBuffer()->getStreamsData().pointer();
		}
		g->addOperation(op);
		m_subGroup.push_back(g);
	}
}

void RenderGroup::SVBGroup::render(mray::scene::RenderGroup *grp)
{
	if (isNew)
		return;
	video::IVideoDevice*dev= grp->getDevice();
	dev->setStreamMap(stream);

	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		(*it)->render(grp);
	}
	dev->endStreams();
}


bool RenderGroup::SVBGroup::CanBeAdded(RenderOperation*op)
{
	if(isNew ||  op->mesh->getMeshBuffer() && op->mesh->getMeshBuffer()->getStreamsData().pointer()==stream)
		return true;
	return false;
}

void RenderGroup::SVBGroup::Clear()
{
	isNew=true;
	stream = 0;
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		(*it)->Clear();
	}
//	m_subGroup.clear();
}

//////////////////////////////////////////////////////////////////////////

RenderGroup::SBaseSortGroup::~SBaseSortGroup()
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		delete *it;
	}
	m_subGroup.clear();
}


void RenderGroup::SBaseSortGroup::addOperation(RenderOperation*op)
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		if((*it)->CanBeAdded(op))
		{
			(*it)->addOperation(op);
			return;
		}
	}
	ISortGroup* g=ISortGroup::CreateSortGroup(s_SortGroupCreation[GetType()]);
	if(g)
	{
		g->addOperation(op);
		m_subGroup.push_back(g);
	}
}

void RenderGroup::SBaseSortGroup::render(mray::scene::RenderGroup *grp)
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		(*it)->render(grp);
	}
}

void RenderGroup::SBaseSortGroup::Clear()
{
	SortGroupList::iterator it=m_subGroup.begin();
	for(;it!=m_subGroup.end();++it)
	{
		(*it)->Clear();
	}
}

bool RenderGroup::SBaseSortGroup::CanBeAdded(mray::scene::RenderOperation *op)
{
	return true;
}

//////////////////////////////////////////////////////////////////////////

/*
void RenderGroup::SShaderGroup::addOperation(RenderOperation*op){

	video::StreamsData*data=op->mesh->getStreamsData();
	VBGroupMap::iterator it=vbGroup.find(data);

	if(it==vbGroup.end()){
		SVBGroup grp;
		grp.stream=data;
		grp.addOperation(op);
		vbGroup.insert(VBGroupMap::value_type(grp.stream,grp));
	}else{
		it->second.addOperation(op);
	}

}
void RenderGroup::SShaderGroup::render(RenderGroup*grp){

	VBGroupMap::iterator it=vbGroup.begin();
	video::StreamsData* streams;
	video::IVideoDevice*dev= grp->getDevice();
	for(;it!=vbGroup.end();++it){

		streams=it->first;
		dev->setStreamMap(streams);

		it->second.render(grp);

		dev->endStreams();
	}

}*/




//////////////////////////////////////////////////////////////////////////
RenderGroup::RenderGroup(RenderManager*mgr,int idx)
{
	m_renderManager=mgr;
	m_sortType=Sort_None;
	m_index=idx;

	m_benchmarkItem=new BenchmarkItem(mT("RenderGroup")+core::StringConverter::toString(idx));
	m_sortBI=m_benchmarkItem->addSubItem(mT("Sort"));
	m_renderBI=m_benchmarkItem->addSubItem(mT("Render"));

	Engine::getInstance().getRootBenchmarking()->addSubItem(m_benchmarkItem);
}
RenderGroup::~RenderGroup()
{
	Engine::getInstance().getRootBenchmarking()->removeSubItem(m_benchmarkItem);
	delete m_benchmarkItem;
}

void RenderGroup::setSortType(RenderGroup::ESortType s){
	m_sortType=s;
}
RenderGroup::ESortType RenderGroup::getSortType(){
	return m_sortType;
}

video::IVideoDevice* RenderGroup::getDevice(){
	return m_renderManager->getSceneManager()->getDevice();
}
void RenderGroup::addRenderOperation(const RenderOperation& op){
	if(op.customRender==false)
		m_unsortedOperations.push_back(op);
	else
		m_customRenderOP.push_back(op);
}

void RenderGroup::clear()
{
	m_unsortedOperations.clear();
	m_sortedOperations.clear();
	m_customRenderOP.clear();
	m_sortGroup.Clear();
}


void RenderGroup::sort()
{

	_AUTO_BENCHMARK(m_sortBI);

	m_sortedOperations.clear();
	m_sortedOperations.reserve(m_unsortedOperations.size());
	for(int i=0;i<m_unsortedOperations.size();++i){
		m_sortedOperations.push_back(&m_unsortedOperations[i]);
	}
	if(m_sortType==Sort_None){
		PRenderOperationList::iterator it=m_sortedOperations.begin();
		PRenderOperationList::iterator end=m_sortedOperations.end();
		video::StreamsData*data=0;
		for(;it!=end;++it){
			RenderOperation*op=*it;
			m_sortGroup.addOperation(op);
		}
	}else{
		std::sort(m_sortedOperations.begin(),m_sortedOperations.end(),DepthSortFunction(m_renderManager->getSceneManager()->getActiveCamera(),m_sortType));
/*
		PRenderOperationList::iterator it=m_sortedOperations.begin();
		PRenderOperationList::iterator end=m_sortedOperations.end();

		//GCPtr<video::GPUShader> shader,lastShader;

		video::StreamsData*data=0,*lastData=0;

		SVBGroup currGrp;
		currGrp.stream=lastData;

		for(;it!=end;++it){
			RenderOperation*op=*it;
			data=op->mesh->getStreamsData();
			if(lastData==data){
				currGrp.addOperation(op);
			}else{
				if(!currGrp.materialGroup.empty()){
					m_vbGroup.insert(vbGroup::value_type(lastData,currGrp));
				}
				currGrp.stream=data;
				currGrp.materialGroup.clear();
				currGrp.addOperation(op);
			}
			lastData=data;
		}
		if(!currGrp.materialGroup.empty()){
			m_vbGroup.insert(vbGroup::value_type(lastData,currGrp));
		}*/
	}
}
void RenderGroup::performRender(IRenderPass*pass)
{
	_AUTO_BENCHMARK(m_renderBI);
	video::IVideoDevice*dev= getDevice();
	if(m_sortType==Sort_None)
	{
		m_sortGroup.render(this);
	}
	else
	{
		video::StreamsData*sdata=0;
		video::GPUShader* shader=0;
		video::RenderPass* mtrl=0;
		video::IShaderConstantsCallback c;
		PRenderOperationList::iterator it=m_sortedOperations.begin();
		
		for(;it!=m_sortedOperations.end();++it){
			RenderOperation*op=*it;

			if(!pass->canRenderObject(op->node))
				continue;


			IMeshBuffer* mb=op->mesh->getMeshBuffer();

			if(mb->getStreamsData().pointer()!=sdata){
				sdata=mb->getStreamsData();
				dev->setStreamMap(sdata);
			}
			if(op->pass!=mtrl){
				mtrl=op->pass;
				shader=mtrl->getRenderShader();
				if(pass->useMaterial(mtrl))
					dev->useRenderPass(mtrl);
				c.setConstants(pass->getMaterialShader(mtrl),video::IShaderConstantsCallback::ESUT_Global);
			}

			math::matrix4x4&m=op->node->getOwner()->getAbsoluteTransformation();
			if(op->node->IsUseIdentityMatrix())
			{
				m=math::matrix4x4::Identity;
			}
			if(!shader || !shader->GetVertexShader())
				dev->setTransformationState(video::TS_WORLD,m);
			else{
				video::ShaderSemanticTable::getInstance().setWorldMatrix(m);
				if(op->node->getMesh()->GetWorldMatricies().size()>0)
					video::ShaderSemanticTable::getInstance().setWorldMatricies(&op->node->getMesh()->GetWorldMatricies()[0],op->node->getMesh()->GetWorldMatricies().size());
				else
					video::ShaderSemanticTable::getInstance().setWorldMatricies(0,0);
				if(!op->node->IsUseIdentityMatrix())
					video::ShaderSemanticTable::getInstance().setPrevWorldMatrix(op->node->getOwner()->getPrevAbsoluteTransformation());
				else
					video::ShaderSemanticTable::getInstance().setPrevWorldMatrix(m);
				op->mesh->getShaderCallback()->setConstants(shader,video::IShaderConstantsCallback::ESUT_Object);
			}
			video::ShaderSemanticTable::getInstance().setSkeleton(op->node->getMesh()->getSkeleton());

			dev->drawElements(mb->getIndexData(),mb->getRenderType());
		}
		dev->endStreams();
	}
	//render custom Render operations
	RenderOperationList::iterator cit=m_customRenderOP.begin();
	for(;cit!=m_customRenderOP.end();++cit)
	{
		RenderOperation&op=*cit;
		if(!op.node)
			continue;
		if(!pass->canRenderObject(op.node))
			continue;

		op.node->render(pass);
	}
}



}
}

