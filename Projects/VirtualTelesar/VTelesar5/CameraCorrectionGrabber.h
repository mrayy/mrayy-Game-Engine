

/********************************************************************
	created:	2013/12/10
	created:	10:12:2013   21:02
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\CameraCorrectionGrabber.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
	file base:	CameraCorrectionGrabber
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CameraCorrectionGrabber__
#define __CameraCorrectionGrabber__


#include "CameraTextureSource.h"
#include "ParsedShaderPP.h"


namespace mray
{

class CameraCorrectionGrabber :public video::ITextureSource
{
protected:

	enum class ERotation
	{
		None,
		CW,
		CCW
	};

	GCPtr<video::CameraTextureSource> m_source;
	video::ITexturePtr m_texture;
	video::IRenderTargetPtr m_rt;
	float m_distance;
	float m_fov;
	float m_targetFov;
	ERotation m_rotation;

	math::vector2d m_cropping;
	math::vector2d m_scale;
	float m_ratio;

	math::vector2d m_target;
	float m_targetAspectRatio;
	GCPtr<video::ParsedShaderPP> m_lensCorrectionPP;
	video::ParsedShaderPP::PassValue* m_correctionValue[2];
	int m_index;

	virtual bool _Load() ;
	virtual bool _Unload() ;

	void _UpdateParams();
public:
	CameraCorrectionGrabber(int idx);
	virtual~CameraCorrectionGrabber();

	virtual const core::string& GetSourceType() ;

	void SetTargetRect(const math::vector2d& target);

	virtual void Update(float dt);


	virtual video::ITextureCRef GetTexture() ;
	virtual xml::XMLElement* LoadFromXML(xml::XMLElement* e) ;
};

}


#endif
