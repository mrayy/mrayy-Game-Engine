

/********************************************************************
	created:	2013/06/10
	created:	10:6:2013   15:42
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\CameraTextureSource.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	CameraTextureSource
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CameraTextureSource__
#define __CameraTextureSource__


#include "ITextureSource.h"
#include "VideoGrabberTexture.h"



namespace mray
{
namespace video
{

class CameraTextureSource:public ITextureSource
{
	DECLARE_RTTI
protected:
	struct CameraInfo
	{
		CameraInfo():id(0),width(320),height(240),fps(25)
		{
		}
		int id;
		int width;
		int height;
		int fps;
	};

	CameraInfo m_cameraSource;
	video::VideoGrabberTexture* m_texGrabber;

	virtual bool _Load();
	virtual bool _Unload();
public:
	static const core::string FactorySourceType;
public:
	CameraTextureSource();
	virtual~CameraTextureSource();

	video::VideoGrabberTexture* GetGrabber(){ return m_texGrabber; }


	void SetCameraInfo(int id,int w,int h,int fps);

	virtual const core::string& GetSourceType(){return FactorySourceType;}

	virtual void Update(float dt);

	virtual ITextureCRef GetTexture();
	virtual xml::XMLElement* LoadFromXML(xml::XMLElement* e);
};

DECLARE_TEXTURE_FACTORY(CameraTextureSource);


}
}


#endif
