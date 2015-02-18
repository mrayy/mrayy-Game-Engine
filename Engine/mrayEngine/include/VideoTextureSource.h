

/********************************************************************
	created:	2013/02/10
	created:	10:2:2013   0:20
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\VideoTextureSource.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	VideoTextureSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __VideoTextureSource__
#define __VideoTextureSource__


#include "ITextureSource.h"
#include "IVideoSource.h"
#include "VideoGrabberTexture.h"

namespace mray
{
namespace video
{

class MRAY_DLL VideoTextureSource:public ITextureSource
{
	DECLARE_RTTI;
protected:
	core::string m_videoPath;
	IVideoSourcePtr m_video;
	bool m_loop;
	bool m_autoStart;
	math::vector2d m_targetResoulution;

	VideoGrabberTexture m_videoGrabber;

	bool _Load();
	bool _Unload();
public:
	static const core::string FactorySourceType;
public:
	VideoTextureSource();
	virtual~VideoTextureSource();

	virtual const core::string& GetSourceType(){return FactorySourceType;}


	void Update(float dt);

	IVideoSourcePtr GetVideoSource(){return m_video;}

	ITextureCRef GetTexture();
	xml::XMLElement* LoadFromXML(xml::XMLElement* e);
};

DECLARE_TEXTURE_FACTORY(VideoTextureSource);

}
}


#endif

