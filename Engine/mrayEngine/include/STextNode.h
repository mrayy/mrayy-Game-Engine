

#ifndef ____STextNode_____
#define ____STextNode_____

#include "SMesh.h"
#include "IRenderable.h"

#include "ITexture.h"
#include "IFont.h"
#include "FontAttributes.h"

namespace mray{
namespace scene{


class MRAY_DLL STextNode:public IRenderable
{
	GUI::IFont*font;
	core::string text;
	math::box3d bBox;

	bool m_autoResize;

	GUI::FontAttributes m_fontAttrs;
public:
	STextNode();
	STextNode(GUI::IFont*font);
	virtual ~STextNode();

	void setText(const  core::string&text);
	void setFont(GUI::IFont*font);

	virtual bool preRender();
	virtual bool render();
	virtual void update(float dt);

	GUI::FontAttributes* getAttrs(){return &m_fontAttrs;}

	void setAutoResize(bool a){m_autoResize=a;}
	bool getAutoResize(){return m_autoResize;}
	
};

}
}


#endif



