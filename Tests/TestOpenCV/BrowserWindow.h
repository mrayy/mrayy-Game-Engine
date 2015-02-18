


/********************************************************************
created:	2012/05/22
created:	22:5:2012   15:21
filename: 	d:\Development\mrayEngine\Tests\TestOpenCV\BrowserWindow.h
file path:	d:\Development\mrayEngine\Tests\TestOpenCV
file base:	BrowserWindow
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __BrowserWindow__
#define __BrowserWindow__


namespace mray
{

class BrowserWindow:public Berkelium::WindowDelegate
{
protected:
	math::vector2di m_size;
	GCPtr<video::ITexture> m_texture;

	Berkelium::Window* m_window;
	mray::uchar* m_flipped;

	bool m_refresh;

public:
	BrowserWindow(int w,int h,Berkelium::Context* c);
	~BrowserWindow();
	video::ITexture* GetTexture();

	Berkelium::Window* GetWindow();

	void Resize(int w,int h);

	math::vector2di GetSize();

	virtual void onStartLoading(Berkelium::Window *win, Berkelium::URLString newURL) ;

	math::vector2d mapCoord(const math::vector2d& c,const math::vector2d& screen);

	void ExecuteScript(const core::stringw& script);

	virtual void onPaint(
		Berkelium::Window *win,
		const unsigned char *sourceBuffer,
		const Berkelium::Rect &sourceBufferRect,
		size_t numCopyRects,
		const Berkelium::Rect *copyRects,
		int dx, int dy,
		const Berkelium::Rect &scrollRect);
	virtual void onLoadError(Berkelium::Window *win, const char* error, size_t errorLength) ;

};


}

#endif
