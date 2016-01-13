

#ifndef GUICommentsLayer_h__
#define GUICommentsLayer_h__


#include "GUICommentsLayerLayout.h"
#include "KMDComment.h"
#include "GUIDockedElement.h"

#include <queue>

namespace mray
{
namespace GUI
{
	class GUICommentDetailsPanel;
	
class GUICommentsLayer :public IGUIPanelElement, public GUICommentsLayerLayout,public GUIDockedElement
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
protected:

	struct CommentInfo
	{
		enum EStatus
		{
			EStart,
			EIdle,
			EDone
		};
		GUICommentDetailsPanel* ui;
		float alpha;
		EStatus status;
	};

	std::list<CommentInfo> m_comments;
	std::list<GUICommentDetailsPanel*> m_graveYard;
	std::queue<kmd::KMDComment*> m_toAdd;

	float m_timeToAdd;
	bool m_firstTime;
	void _UpdatePositions();

	void _AddComment(kmd::KMDComment* c);
public:
	GUICommentsLayer(GUI::IGUIManager* m);
	virtual ~GUICommentsLayer();

	void CommentsLoaded(const std::vector<kmd::KMDComment*>& comments);

	virtual void Draw(const math::rectf*vp);
	virtual void Update(float dt);


	void LoadXMLSettings(xml::XMLElement*e);
};
DECLARE_ELEMENT_FACTORY(GUICommentsLayer);

}
}
#endif // GUICommentsLayer_h__
