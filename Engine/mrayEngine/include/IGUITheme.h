

/********************************************************************
	created:	2010/11/04
	created:	4:11:2010   16:20
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IGUITheme.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IGUITheme
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IGUITheme_h__
#define IGUITheme_h__

#include "SColor.h"
#include "mstring.h"

#include "FontAttributes.h"

namespace mray
{

	namespace video
	{
		class RenderMaterial;
	}
namespace GUI
{
class IGUIRenderer;
class GUIProfile;

class MRAY_DLL IGUITheme
{
private:
protected:
	typedef std::map<core::string,GUIProfile*> ProfileList;
	ProfileList m_profiles;
	core::string m_name;

	video::RenderMaterial* m_material;

	FontAttributes m_hintFontAttr;
public:
	IGUITheme(const core::string&name);
	virtual ~IGUITheme();

	const core::string& GetName(){return m_name;}

	void addProfile(GUIProfile*  p);
	GUIProfile*  getProfile(const core::string&name);

	FontAttributes* getHintFontAttr();

	void setMaterial(video::RenderMaterial* m);
	video::RenderMaterial* getMaterial();


	virtual void drawSizableBox(IGUIRenderer*renderer, const math::rectf &r,int state,const core::string &profile,const video::SColor& globalColor=1,math::rectf*clip=0)=0;
	// Horizontal
	virtual void drawSizableBox3PH(IGUIRenderer*renderer, const math::rectf &rc,int state,const core::string &profile,const video::SColor& globalColor=1,math::rectf*clip=0)=0;
	// Vertical
	virtual void drawSizableBox3PV(IGUIRenderer*renderer, const math::rectf &rc,int state,const core::string &profile,const video::SColor& globalColor=1,math::rectf*clip=0)=0;
	virtual void drawBox(IGUIRenderer*renderer, const math::rectf &r,const core::string &profile,int index,const video::SColor& globalColor,math::rectf*clip=0,bool fill=true)=0;

	//returns the rect which corresponds to "rect"
	//rect : 0 --> 8
	virtual math::rectf getSizableRect(int rect,const math::rectf &r,int state,const core::string &profile)=0;
	//rect : 0 --> 2
	virtual math::rectf getRect3PH(int rect,const math::rectf &r,int state,const core::string &profile)=0;
	//rect : 0 --> 2
	virtual math::rectf getRect3PV(int rect,const math::rectf &r,int state,const core::string &profile)=0;

	virtual math::rectf getRect(int rect,const math::rectf &r,const core::string &profile)=0;
};

MakeSharedPtrType(IGUITheme)

}
}

#endif // IGUITheme_h__

