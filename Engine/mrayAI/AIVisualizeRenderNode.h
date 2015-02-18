


/********************************************************************
	created:	2010/06/29
	created:	29:6:2010   23:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\AIVisualizeRenderNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	AIVisualizeRenderNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef AIVisualizeRenderNode_h__
#define AIVisualizeRenderNode_h__

#include "CompileConfig.h"
#include <IRenderable.h>
#include "IRenderDevice.h"
#include "IVideoDevice.h"
#include "IVisualizable.h"
#include <list>

namespace mray
{
namespace scene
{
	class ISceneManager;


class MRAY_AI_DLL AIVisualizeRenderNode:public IRenderable
{
	bool m_vis;
	bool m_pass;
	bool m_castShadow;
	bool m_recShadow;

	bool m_isEditorNode;

	math::matrix4x4 m_trans;
	RenderableList m_child;

	core::string m_name;
	uint m_id;

	video::RenderPass*m_mtrl;

	typedef std::list<AI::IVisualizable*> VisualizableList;
	VisualizableList m_visualizeList;

	ISceneManager* m_smngr;
	AI::IRenderDevice* m_renderDev;

public:
	AIVisualizeRenderNode(ISceneManager* m_smngr,AI::IRenderDevice* renderDev);
	virtual~AIVisualizeRenderNode();

	void AddVisualizableNode(AI::IVisualizable*node);
	void RemoveVisualizableNode(AI::IVisualizable*node);

	virtual bool preRender();
	virtual bool render(IRenderPass*pass);
	virtual void update(float dt);

	virtual const RenderableList& getChildren();
};

}
}

#endif // AIVisualizeRenderNode_h__
