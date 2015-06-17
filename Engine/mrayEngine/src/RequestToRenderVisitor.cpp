#include "stdafx.h"

#include "RequestToRenderVisitor.h"

#include "ISceneNode.h"
#include "IRenderable.h"
#include "ISceneManager.h"

#include "RenderManager.h"
#include "SMesh.h"
#include "LightNode.h"
#include "MeshBufferData.h"
#include "CameraNode.h"
#include "RenderPass.h"
#include "RenderTechnique.h"

namespace mray
{
namespace scene
{


RequestToRenderVisitor::RequestToRenderVisitor(IRenderPass*pass)
{
	m_pass=pass;
	m_smngr=0;
}

RequestToRenderVisitor::~RequestToRenderVisitor()
{
}

void RequestToRenderVisitor::_AddRenderableNode(IRenderable*r)
{
	if(!m_pass->canRenderObject(r))
		return;
	if(r->preRender(m_pass))
		return;
	RenderOperation ROP;
	ROP.node=r;
	SMesh* mesh=r->getMesh();
	if(mesh){
		for (int i=0;i<mesh->getBuffersCount();++i)
		{
			bool added=false;
			ROP.mesh=mesh->getBufferData(i);
			ROP.customRender=r->isCustomRender();
			if(!ROP.mesh->isVisible())
				continue;

			int targetLayer=0;

			if(r->HasCustomRenderGroup())
			{
				targetLayer=r->GetTargetRenderGroup();
			}
			if(ROP.mesh){
				video::RenderMaterial*mr=ROP.mesh->getMaterial();
				if(mr)
				{
					if(mr->GetActiveTechnique())
					{
						const video::RenderPassList&lst= mr->GetActiveTechnique()->GetPasses();
						for(int i=0;i<lst.size();++i)
						{
							video::RenderPass*p=lst[i];
							if (!p->IsEnabled())
								continue;
							if (!r->HasCustomRenderGroup())
							{
								if (p->getRenderState(video::RS_Blend))
									targetLayer = RGH_Transparent;
								else
									targetLayer = RGH_Solid;

							}
							ROP.pass = p;
							m_smngr->getRenderManager()->addOperation(targetLayer,ROP);
						}
					}
				}
				added=true;
			}
			if(!ROP.mesh && !ROP.customRender || added)
				continue;

			//m_smngr->getRenderManager()->addOperation(targetLayer,ROP);
		}
	}else if(r->isCustomRender())
	{

		int targetLayer=0;
		ROP.customRender=true;

		if(r->HasCustomRenderGroup())
		{
			targetLayer=r->GetTargetRenderGroup();
		}else
			targetLayer=RGH_Solid;
		m_smngr->getRenderManager()->addOperation(targetLayer,ROP);
	}
}

void RequestToRenderVisitor::Visit(ISceneManager*mngr)
{
	m_smngr=mngr;

	m_smngr->clearRenderList();


	if (!mngr->getRootNode())
		return;
	mngr->getRootNode()->OnVisit(this);

/*	const ISceneManager::SceneNodeMap& lst = m_smngr->GetChildren();
	ISceneManager::SceneNodeMap::const_iterator it=lst.begin();
	for (;it!=lst.end();++it)
	{
		it->second->OnVisit(this);
	}*/
}

void RequestToRenderVisitor::Visit(ISceneNode*node)
{
	if(!node->isVisible() || m_smngr->isNodeCulled(node))
	{
		node->SetDrown(false);
		return;
	}
	Visit((IMovable*)node);
	node->SetDrown(true);
	node->preRender();

	m_smngr->addNodeToRender(node);

	{

		const AttachNodesList& lst=node->GetAttachedNodes();
		for (int i=0;i<lst.size();++i)
		{
			lst[i]->OnVisit(this);
		}
	}
/*	for (int i=0;i<node->GetAttachedNodesCount();++i)
	{
		node->GetAttachedNode(i)->OnVisit(this);
	}*/
}

void RequestToRenderVisitor::Visit(CameraNode*c)
{
	Visit((ISceneNode*)c);
	if(!c->isVisible())
		return;
	c->clearVisibleNodes();
	c->updateView();
}

void RequestToRenderVisitor::Visit(LightNode*l)
{
	Visit((ISceneNode*)l);
	if(!l->isEnabled())
		return;

	m_smngr->addLightToRender(l);
}

void RequestToRenderVisitor::Visit(IRenderable*r)
{
	if(!r->getOwner() || !r->getOwner()->IsDrown())
		return;
	
	_AddRenderableNode(r);
}


void RequestToRenderVisitor::Visit(IMovable*node)
{
	/*
	{

		const MovableNodeList& lst=node->getChildren();
		MovableNodeList::const_iterator it=lst.begin();
		for (;it!=lst.end();++it)
		{
			(*it)->OnVisit(this);
		}
	}*/
}



}
}



