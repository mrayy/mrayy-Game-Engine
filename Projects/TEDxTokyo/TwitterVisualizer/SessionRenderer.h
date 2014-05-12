



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

#include "DataTypes.h"

namespace mray
{
	namespace ted
	{
		class SessionContainer;
		class CSpeaker;
	}
namespace scene
{
	class ITedNode;
	class SpeakerNode;
	class TweetNode;
	class NodeRenderer;
class SessionRenderer
{
protected:
	NodeRenderer* m_nodeRenderer;
	ted::SessionContainer*m_sessions;

	std::list<ITedNode*> m_renderNodes;

	typedef std::map<ted::IDType, TweetNode*> TweetMap;
	typedef std::map<core::string, SpeakerNode*> SpeakerMap;


	SpeakerMap m_speakers;
	TweetMap m_tweets;

	msa::physics::World2D* m_physics;
	OS::IMutex* m_dataMutex;
	ITedNode* m_hoverItem;

	math::vector2d m_translation;
	math::vector2d m_scale;

	math::matrix4x4 m_transformation;

	void _RenderConnections();
	void _RenderSpeakers();
	void _RenderTweets();

public:
	SessionRenderer();
	virtual ~SessionRenderer();

	void SetSessions(ted::SessionContainer*sessions);

	void _OnSpeakerChanged(ted::CSpeaker*s);

	void AddTweetsNodes(const std::vector<TweetNode*> &nodes);

	ITedNode* GetNodeFromPosition(const math::vector2d& pos);

	void SetHoverdItem(ITedNode* node);
	ITedNode* GetHoverdItem(){ return m_hoverItem; }

	void Draw();

	void Update(float dt);

	void SetTransformation(const math::vector2d& pos, float angle, const math::vector2d& scale);

	math::vector2d ConvertToWorldSpace(const math::vector2d& pos);
	math::vector2d ConvertToScreenSpace(const math::vector2d& pos);
};

}
}

#endif