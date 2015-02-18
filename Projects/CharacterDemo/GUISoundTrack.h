
/********************************************************************
	created:	2010/06/30
	created:	30:6:2010   15:52
	filename: 	i:\Programing\GameEngine\mrayEngine\CharacterDemo\GUISoundTrack.h
	file path:	i:\Programing\GameEngine\mrayEngine\CharacterDemo
	file base:	GUISoundTrack
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUISoundTrack_h__
#define GUISoundTrack_h__

#include <GUIElement.h>

namespace mray
{
namespace GUI
{

class GUISoundTrack:public GUIElement
{

protected:
	GCPtr<video::TextureUnit> m_BackGnd;
	core::string m_soundName;
	core::string m_themeName;

	float m_cooldown;
	float m_maxCooldown;
	float m_lastAlpha;
	bool m_show;


public:

	GUISoundTrack(GUIManager*mngr,float maxCooldown,math::vector2d pos,math::vector2d size);
	virtual ~GUISoundTrack();

	void show(const core::string& soundName,const core::string& themeName);

	virtual void draw(float dt);

};


}

}
#endif // GUISoundTrack_h__