

#ifndef ___GUITheme___
#define ___GUITheme___


#include "mString.h"

#include "GCPtr.h"
#include "TextureUnit.h"
#include "IGUITheme.h"
//#include "GUIMultiQuadRenderOperation.h"


namespace mray{
namespace video{
	class IVideoDevice;
}
namespace GUI{


class MRAY_DLL GUITheme:public IGUITheme
{
protected:
	//GCPtr<GUIMultiQuadRenderOperation> m_renderOperation;
public:

	GUITheme(const core::string&name);
	virtual~GUITheme();

	virtual void drawSizableBox(IGUIRenderer*renderer, const math::rectf &rc,int state,const core::string &profile,const video::SColor& globalColor=255,math::rectf*clip=0);
	virtual void drawSizableBox3PH(IGUIRenderer*renderer, const math::rectf &rc,int state,const core::string &profile,const video::SColor& globalColor=255,math::rectf*clip=0);
	virtual void drawSizableBox3PV(IGUIRenderer*renderer, const math::rectf &rc,int state,const core::string &profile,const video::SColor& globalColor=255,math::rectf*clip=0);
	virtual void drawBox(IGUIRenderer*renderer, const math::rectf &r,const core::string &profile,int index,const video::SColor& globalColor,math::rectf*clip=0,bool fill=true);

	
	virtual math::rectf getSizableRect(int rect,const math::rectf &r,int state,const core::string &profile);
	virtual math::rectf getRect3PH(int rect,const math::rectf &r,int state,const core::string &profile);
	virtual math::rectf getRect3PV(int rect,const math::rectf &r,int state,const core::string &profile);
	virtual math::rectf getRect(int rect,const math::rectf &r,const core::string &profile);
};

}
}




#endif

