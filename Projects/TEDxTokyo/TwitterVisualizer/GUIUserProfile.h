

/********************************************************************
	created:	2014/04/11
	created:	11:4:2014   2:21
	filename: 	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer\GUIUserProfile.h
	file path:	C:\Development\mrayEngine\Projects\TEDxTokyo\TwitterVisualizer
	file base:	GUIUserProfile
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GUIUserProfile__
#define __GUIUserProfile__


#include "IGUIElement.h"
#include "GUIElementFactoryDef.h"

namespace mray
{
	namespace ted
	{
		class TwitterUserProfile;
	}
namespace GUI
{

class GUIUserProfile:public IGUIElement
{
	DECLARE_RTTI;
public:
	static const GUID ElementType;
protected:
	ted::TwitterUserProfile* m_user;
	video::ITexturePtr m_profilePic;
	video::IGPUShaderProgramPtr m_maskingShader;

	GUI::FontAttributes m_fontAttrs;

	float m_openAnimation;
public:
	GUIUserProfile(GUI::IGUIManager* creator);
	virtual~GUIUserProfile();

	void SetUser(ted::TwitterUserProfile* u);
	ted::TwitterUserProfile* GetUser();


	virtual void Update(float dt);
	virtual void Draw(const math::rectf*vp);
};


DECLARE_ELEMENT_FACTORY(GUIUserProfile);

}
}


#endif
