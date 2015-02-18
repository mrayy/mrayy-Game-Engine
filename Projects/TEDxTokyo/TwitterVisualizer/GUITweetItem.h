

/********************************************************************
	created:	2014/04/11
	created:	11:4:2014   15:26
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\GUITweetItem.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	GUITweetItem
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUITweetItem__
#define __GUITweetItem__


#include "IGUIElement.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
	namespace ted
	{
		class TwitterTweet;
	}
namespace GUI
{

class GUITweetItem:public IGUIElement
{
	DECLARE_RTTI;
public: 
	static const GUID ElementType;
protected:
	ted::TwitterTweet* m_Tweet;
	GUI::FontAttributes m_fontAttrs;
public:
	GUITweetItem(IGUIManager* m);
	virtual~GUITweetItem();

	void SetTweet(ted::TwitterTweet* t);
	ted::TwitterTweet* GetTweet(){ return m_Tweet; }


	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);
};

DECLARE_ELEMENT_FACTORY(GUITweetItem);

}
}


#endif
