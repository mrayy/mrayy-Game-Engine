#include "stdafx.h"

#include "AIVisualizeRenderNode.h"
#include "RenderOperation.h"

#include "RenderMaterial.h"
#include "RenderManager.h"
#include "ISceneManager.h"
#include "RenderPass.h"

namespace mray
{
namespace scene
{


AIVisualizeRenderNode::AIVisualizeRenderNode(ISceneManager* smngr,AI::IRenderDevice* renderDev):m_smngr(smngr),m_renderDev(renderDev)
{
	m_mtrl=new video::RenderPass(0);
	m_mtrl->setRenderState(video::RS_Lighting,video::ES_DontUse);
	m_mtrl->setRenderState(video::RS_Blend,video::ES_Use);
}

AIVisualizeRenderNode::~AIVisualizeRenderNode()
{
	delete m_mtrl;
}

bool AIVisualizeRenderNode::preRender()
{
	RenderOperation rop;
	rop.customRender=true;
	rop.node=this;

	m_smngr->getRenderManager()->addOperation(RGH_Transparent,rop);
	return true;
}
bool AIVisualizeRenderNode::render(IRenderPass*pass)
{
	
	m_smngr->getDevice()->setTransformationState(video::TS_WORLD,math::matrix4x4::Identity);
	m_smngr->getDevice()->useRenderPass(m_mtrl);
	VisualizableList::iterator it= m_visualizeList.begin();
	for (;it!=m_visualizeList.end();++it)
	{
		if((*it)->IsEnableVisualize())
			(*it)->Visualize(m_renderDev);
	}
	return true;
}
void AIVisualizeRenderNode::update(float dt)
{
}

void AIVisualizeRenderNode::AddVisualizableNode(AI::IVisualizable*node)
{
	m_visualizeList.push_back(node);
}

void AIVisualizeRenderNode::RemoveVisualizableNode(AI::IVisualizable*node)
{
	m_visualizeList.remove(node);
}



const RenderableList&  AIVisualizeRenderNode::getChildren()
{
	return m_child;
}
}
}

