

/********************************************************************
	created:	2012/10/10
	created:	10:10:2012   0:57
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\VideoGrabberTexture.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VideoGrabberTexture
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___VideoGrabberTexture___
#define ___VideoGrabberTexture___

#include "IVideoGrabber.h"
#include "ITexture.h"

namespace mray
{
namespace video
{

class MRAY_DLL VideoGrabberTexture
{
protected:

	GCPtr<IVideoGrabber> m_grabber;
	GCPtr<ITexture> m_texture;

public:
	VideoGrabberTexture();
	virtual~VideoGrabberTexture();
	
	void Set(const GCPtr<IVideoGrabber>& grabber,ITextureCRef tex);

	const GCPtr<IVideoGrabber>& GetGrabber(){return m_grabber;}
	ITextureCRef GetTexture(){return m_texture;}

	bool Blit();
	
};

}
}

#endif
