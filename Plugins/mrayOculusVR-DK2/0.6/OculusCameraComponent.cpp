

#include "stdafx.h"
#include "OculusCameraComponent.h"
#include "OculusDevice.h"
#include "CameraNode.h"
#include "ViewPort.h"
#include "ParsedShaderPP.h"


namespace mray
{
namespace game
{


IMPLEMENT_RTTI(OculusCameraComponent,IGameComponent);

OculusCameraComponent::OculusCameraComponent(game::GameEntityManager*mngr)
{
	m_shader=0;
	m_device=0;
	m_vp=0;
	m_isDirty=false;
	m_perspOffset=0;
}
OculusCameraComponent::~OculusCameraComponent()
{
}

bool OculusCameraComponent::InitComponent()
{
	return game::IGameComponent::InitComponent();

}
void OculusCameraComponent::Update(float dt)
{
	IGameComponent::Update(dt);
	//if(!m_isDirty || !m_device || !m_vp || !m_shader)
	//	return;
	if (!m_device || !m_device->IsConnected() || !m_vp)
		return;

	const video::OculusDeviceData& data = m_device->GetDeviceInfo();
	math::vector2d vpSize = math::vector2d(m_vp->GetSize().x, m_vp->GetSize().y);
	math::vector2d scale, offset;
	m_device->GetRenderScaleAndOffset(data.eyeFov[m_target], vpSize, math::recti(0, vpSize), scale, offset);


	video::ParsedShaderPP::PassValue*lensCenter;
	video::ParsedShaderPP::PassValue*perspOff;
	video::ParsedShaderPP::PassValue*Scale;
	video::ParsedShaderPP::PassValue*ScaleIn;
	video::ParsedShaderPP::PassValue*HMDWrapParams;

	Scale = m_shader->GetValue("final.Scale");
	if (Scale)
	{
		Scale->floatParam[0] = scale.x;
		Scale->floatParam[1] = scale.y;

	}

	/*float fov = m_device->GetVerticalFOV(vpSize);

	float distOffset;
	float perspOffset;
	float distScale = 1.0f / m_device->GetDistortionScale(vpSize);
	float aspectRatio = m_device->GetAspectRatio();
	math::vector2d normalizedSize = 1;
	float offset = 0;

	offset = data.deviceInfo.InterpupillaryDistance*0.5;
	if (m_target == LeftCamera)
	{
		distOffset = 0.5 + data.lensOffset.x*0.5;
		perspOffset = data.lensOffset.x;
		offset = -offset;
	}
	else
	{
		distOffset = 0.5 + data.lensOffset.y*0.5;
		perspOffset = data.lensOffset.y;
	}

	m_perspOffset = perspOffset;

	if (m_vp->getCamera())
	{
		//	m_vp->getCamera()->setFovY(fov);
		m_vp->getCamera()->setAspect(aspectRatio);
		m_vp->getCamera()->setAutoUpdateAspect(false);
		const math::matrix4x4& pm = m_vp->getCamera()->getProjectionMatrix();
		math::matrix4x4 m;
		m.f14 = perspOffset;
		m_vp->getCamera()->setProjectionMatrix(m*pm);
		//	m_vp->getCamera()->setPosition(math::vector3d(offset,0,0));
	}

	m_infoString.clear();


	m_infoString.push_back(core::string("FoV: ") + core::StringConverter::toString(fov));
	m_infoString.push_back(core::string("Aspect: ") + core::StringConverter::toString(aspectRatio));

	perspOff = m_shader->GetValue("final.xoffset");
	if (perspOff)
	{
		perspOff->floatParam[0] = perspOffset;
		m_infoString.push_back(core::string("perspOffset: ") + core::StringConverter::toString(perspOffset));
	}

	lensCenter = m_shader->GetValue("final.lensCenter");
	if (lensCenter)
	{
		lensCenter->floatParam[0] = distOffset;
		m_infoString.push_back(core::string("Center: ") + core::StringConverter::toString(distOffset));
	}

	Scale = m_shader->GetValue("final.Scale");
	if (Scale)
	{
		Scale->floatParam[0] = (normalizedSize.x / 2.0f)*distScale;
		Scale->floatParam[1] = (normalizedSize.y / 2.0f)*distScale*aspectRatio;

		m_infoString.push_back(core::string("Scale: ") + core::StringConverter::toString(math::vector2d(Scale->floatParam[0], Scale->floatParam[1])));
	}
	ScaleIn = m_shader->GetValue("final.ScaleIn");
	if (ScaleIn)
	{
		ScaleIn->floatParam[0] = 2.0f / normalizedSize.x;
		ScaleIn->floatParam[1] = (2.0f / normalizedSize.y) / aspectRatio;

		m_infoString.push_back(core::string("ScaleIn: ") + core::StringConverter::toString(math::vector2d(ScaleIn->floatParam[0], ScaleIn->floatParam[1])));
	}
	HMDWrapParams = m_shader->GetValue("final.HMDWrapParams");
	if (HMDWrapParams)
	{
		HMDWrapParams->floatParam[0] = data.deviceInfo.DistortionK[0];
		HMDWrapParams->floatParam[1] = data.deviceInfo.DistortionK[1];
		HMDWrapParams->floatParam[2] = data.deviceInfo.DistortionK[2];
		HMDWrapParams->floatParam[3] = data.deviceInfo.DistortionK[3];
	}*/

	m_isDirty = false;
	

}
void OculusCameraComponent::InitCamera(video::OculusDevice* device,scene::ViewPort* vp,ETargetCamera target,video::ParsedShaderPP* shader)
{
	m_device=device;
	m_vp=vp;
	m_target=target;
	m_shader=shader;
	m_isDirty=true;
}


IMPLEMENT_GAMECOMPONENT_FACTORY(OculusCameraComponent);


}
}
