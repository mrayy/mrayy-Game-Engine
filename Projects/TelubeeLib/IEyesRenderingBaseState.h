

/********************************************************************
created:	2013/12/04
created:	4:12:2013   11:27
filename: 	C:\Development\mrayEngine\Projects\TELUBee\IEyesRenderingBaseState.h
file path:	C:\Development\mrayEngine\Projects\TELUBee
file base:	IEyesRenderingBaseState
file ext:	h
author:		MHD Yamen Saraiji

purpose:
*********************************************************************/

#ifndef __IEyesRenderingBaseState__
#define __IEyesRenderingBaseState__

#include "IRenderingState.h"
#include "ParsedShaderPP.h"



namespace mray
{

namespace TBee
{
	class IRobotCommunicator;
	class CRobotConnector;
	class ICameraVideoSource;
	class TelubeeCameraConfiguration;


class IEyesRenderingBaseState :public IRenderingState
{
protected:

	struct EyeInfo
	{
		EyeInfo() :ratio(1)
		{
		}

		math::vector2d cropping;
		math::vector2d scale;
		float ratio;
	};
	EyeInfo m_eyes[2];

	float m_targetAspectRatio;
	float m_hmdFov;
	math::vector2d m_hmdSize;
	float m_panningScale;//used to give some freedom to look around using Oculus in a scaled up mode
	float m_headPan;
	float m_headTilt;
	bool m_enablePanning;

	bool m_useLensCorrection;
	TelubeeCameraConfiguration *m_cameraConfiguration;
	bool m_camConfigDirty;

	GUI::IGUIRenderer* m_guiRenderer;

	GCPtr<video::ParsedShaderPP> m_lensCorrectionPP;
	GCPtr<video::ParsedShaderPP> m_I420ToRGB;
	ICameraVideoSource* m_videoSource;

	math::vector2d m_contentsPos,m_contentsOrigin;
	float m_contentsRotation;

	void _UpdateCameraParams();

	virtual void _RenderUI(const math::rectf& rc, math::vector2d& pos, ETargetEye eye);

public:
	IEyesRenderingBaseState(const core::string& name);
	virtual~IEyesRenderingBaseState();

	virtual void InitState();
	void SetHMDParameters(float targetAspectRatio, float hmdFov);
	void SetContentsOrigin(float x, float y){ m_contentsOrigin.set(x, y); }
	void SetContentsPosition(float x, float y){ m_contentsPos.set(x, y); }
	void SetContentsRotation(float r){ m_contentsRotation = r; }

	void SetVideoSource(ICameraVideoSource* src){ m_videoSource = src; }
	ICameraVideoSource* GetVideoSource(){ return m_videoSource; }

	virtual bool OnEvent(Event* e, const math::rectf& rc);
	virtual void OnEnter(IRenderingState*prev);
	virtual void OnExit();
	virtual void Update(float dt);
	virtual video::IRenderTarget* Render(const math::rectf& rc, ETargetEye eye);

	virtual void LoadFromXML(xml::XMLElement* e);
	


};

}
}


#endif
