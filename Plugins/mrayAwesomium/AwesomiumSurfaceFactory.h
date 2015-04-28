

#ifndef __AwesomiumSURFACEFACTORY__
#define __AwesomiumSURFACEFACTORY__

#include "Awesomium/Surface.h"
#include "ITexture.h"

namespace mray
{
namespace web
{
	
class AwesomiumSurfaceFactory :public Awesomium::SurfaceFactory
{
protected:

public:
	AwesomiumSurfaceFactory(){}
	virtual ~AwesomiumSurfaceFactory(){}

	virtual Awesomium::Surface* CreateSurface(Awesomium::WebView* view,
		int width,
		int height) ;

	virtual void DestroySurface(Awesomium::Surface* surface);
};

class AwesomiumSurface:public Awesomium::Surface 
{
protected:

	video::ITexturePtr m_texture;
	Awesomium::WebView* m_view;
public:
	AwesomiumSurface(Awesomium::WebView* view, int w, int h);

	virtual ~AwesomiumSurface() ;

	Awesomium::WebView* GetView(){ return m_view; }

	video::ITexturePtr GetTexture(){ return m_texture; }

	virtual void Paint(unsigned char* src_buffer,
		int src_row_span,
		const Awesomium::Rect& src_rect,
		const Awesomium::Rect& dest_rect) ;

	virtual void Scroll(int dx,
		int dy,
		const Awesomium::Rect& clip_rect);
};


}
}


#endif
