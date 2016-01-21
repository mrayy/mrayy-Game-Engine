



/********************************************************************
	created:	2014/05/07
	created:	7:5:2014   17:30
	filename: 	F:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\SessionRenderer.h
	file path:	F:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	SessionRenderer
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __SessionRenderer__
#define __SessionRenderer__

#include "IMutex.h"

#include "AppData.h"
#include "DataTypes.h"
#include "PhManager.h"

namespace mray
{
	namespace kmd
	{
		class KMDComment;
		class SessionContainer;
		class SessionDetails;
		class CSubProject;
	}
namespace scene
{
	class IKMDNode;
	class ProjectNode;
	class CommentNode;
	class NodeRenderer;
	class SceneCamera;
	class PointerNode;


class SessionRenderer :public kmd::ISubProjectChangeListener,public kmd::ICommentsListener
{
protected:
	NodeRenderer* m_nodeRenderer;
	kmd::SessionContainer*m_sessions;

	std::list<IKMDNode*> m_renderNodes;

	typedef std::map<kmd::IDType, CommentNode*> CommentMap;
	typedef std::map<kmd::SessionDetails*, ProjectNode*> ProjectMap;


	float m_speakerDistance;
	float m_CommentsDistance;

	float m_alpha;

	bool m_enabled;

	std::vector<ProjectNode*> m_projectsSeq;
	ProjectMap m_projects;
	CommentMap m_Comments;

	//msa::physics::World2D* m_physics;
	physics::PhManager* m_physics;
	OS::IMutex* m_dataMutex;
	IKMDNode* m_hoverItem;
	IKMDNode* m_selectedItem;
	SceneCamera* m_camera;
	ProjectNode* m_activeSpeaker;

	void _RenderConnections();
	void _RenderSpeakers();
	void _RenderComments();

	math::rectf CalcAllBox();
	void _AddCommentsNodes(const std::vector<CommentNode*> &nodes);
	void _AddCommentNode(kmd::KMDComment* t, ProjectNode*speaker);


public:
	SessionRenderer();
	virtual ~SessionRenderer();

	void OnPointerMoved(const math::vector2d& pos);

	void SetRenderingVP(const math::rectf& vp);

	void SetSessions(kmd::SessionContainer*sessions);

	virtual void _OnSubProjectChange(kmd::CSubProject* sp);

	ProjectNode* GetCurrentSpeaker(){ return m_activeSpeaker; }

	void AddComments(const std::vector<kmd::KMDComment*> &Comments);

	IKMDNode* GetNodeFromPosition(const math::vector2d& pos);

	void SetHoverdItem(IKMDNode* node);
	IKMDNode* GetHoverdItem(){ return m_hoverItem; }
	void SetSelectedItem(IKMDNode* node);
	IKMDNode* GetSelectedItem(){ return m_selectedItem; }

	void Draw(float alpha);

	void SetAlpha(float a){ m_alpha = a; }

	void Update(float dt);
	virtual void OnCommentsLoaded(const std::vector<kmd::KMDComment*>& comments){ AddComments(comments); }


};

}
}

#endif
