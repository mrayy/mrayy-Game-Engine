

#include "stdafx.h"
#include "FingerTipComponent.h"
#include "HeadMount.h"
#include "ATAppGlobal.h"
#include "IGUIRenderer.h"
#include "FontResourceManager.h"
#if USE_OPENNI
#include "OpenNIHandler.h"
#endif
#include "GGComponent.h"

#include "ATAppGlobal.h"

namespace mray
{
namespace AugTel
{
	IMPLEMENT_SETGET_PROP(FingerTipComponent, NormalAxis, math::vector3d, m_normalAxis, math::vector3d::YAxis, core::StringConverter::toVector3d, core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(FingerTipComponent, Channel, int, m_channel, 0, core::StringConverter::toInt, core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(FingerTipComponent, GGDriverName, core::string, m_GGDriverName, "", , );
	IMPLEMENT_SETGET_PROP(FingerTipComponent, IsLeftHand, bool, m_isLeftHand, false, core::StringConverter::toBool, core::StringConverter::toString );

	IMPLEMENT_RTTI(FingerTipComponent, MountableComponent);


	const float VEL_DELTA_TIME = 1.0f / 30.0f;

	int s_fingerindex = 0;;

FingerTipComponent::FingerTipComponent(game::GameEntityManager*m)
:VT::MountableComponent(m), m_normalAxis(math::vector3d::YAxis)
{
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeNormalAxis::instance);
		dic->addPropertie(&PropertyTypeChannel::instance);
		dic->addPropertie(&PropertyTypeIsLeftHand::instance);
		dic->addPropertie(&PropertyTypeGGDriverName::instance);
	}
	m_channel= 0;
	m_isLeftHand = false;
	m_index = s_fingerindex++;
}
FingerTipComponent::~FingerTipComponent()
{

}


bool FingerTipComponent::InitComponent()
{
	if (!VT::MountableComponent::InitComponent())
		return false;

	m_ggDriver = RetriveComponent<VT::GGComponent>(GetOwnerComponent(), m_GGDriverName);
	m_time = 999;
	return true;
}

bool FingerTipComponent::_calculateForce(float dt)
{
	scene::CameraNode* camera = ATAppGlobal::Instance()->headObject->GetRightCamera();
	math::matrix4x4 mat = camera->getViewMatrix();// math::MathUtil::CreateLookAtMatrix(cpos, ATAppGlobal::Instance()->headObject->getAbsoluteOrintation());//ATAppGlobal::Instance()->sharedCamera->getViewMatrix() ;//
	mat = ATAppGlobal::Instance()->headObject->GetRightCamera()->getProjectionMatrix()* mat;
	math::vector3d pos = GetAbsolutePosition();;
	math::vector4d projPos = mat*math::vector4d(pos.x, pos.y, pos.z, 1);

	m_projPos.x = projPos.x / projPos.w;
	m_projPos.y = projPos.y / projPos.w;
	m_projPos.z = pos.getDist(camera->getAbsolutePosition());

	m_projPos.x = (+m_projPos.x)*0.5 + 0.5;
	m_projPos.y = (-m_projPos.y)*0.5 + 0.5;

	m_time += dt;

	if (m_time >= VEL_DELTA_TIME)
	{
		math::vector3d vel = (pos-m_lastPos)/m_time;
// 		m_avgVelocity.x = (GetAbsoluteOrientation()*math::vector3d::XAxis).dotProduct(vel);
// 		m_avgVelocity.y = (GetAbsoluteOrientation()*math::vector3d::YAxis).dotProduct(vel);
// 		m_avgVelocity.z = (GetAbsoluteOrientation()*math::vector3d::ZAxis).dotProduct(vel);
// 		
		math::vector3d vel2 = GetAbsoluteOrientation().inverse()*vel;
		m_avgVelocity = math::vector3d(vel2.z, vel2.x, vel2.y);
		//convert velocity to finger space
		//m_avgVelocity=GetAbsoluteOrientation()*m_avgVelocity;
		m_time = 0;
		m_lastPos = pos;// m_projPos;
		

	}
#if USE_OPENNI

	if (!ATAppGlobal::Instance()->depthProvider->ConvertToImageSpace(math::vector2d(m_projPos.x, m_projPos.y), m_depthSpace))
	{
		m_depthSpace = -1;
		return false;
	}

	float depth = ATAppGlobal::Instance()->depthProvider->GetNormalCalculator().GetDepthAtPoint(m_depthSpace.x, m_depthSpace.y);
	if (depth < m_projPos.z)
	{
		m_force.z = m_projPos.z - depth; //represents the amount of penetration with the other object
		float maxPent = 0.1f;
		if (m_force.z>maxPent)//limit the penetration to "x" cm
			m_force.z = -maxPent;
		m_force.z = m_force.z*4/maxPent;
		m_force.x = m_avgVelocity.x;
		float fx = abs(m_force.x) - 0.2f;
		if (fx<0)
			m_force.x = 0;
		else
			m_force.x -= math::sign(m_force.x)*fx;

		float maxVel = 0.2;
		if (m_force.x > maxVel)
			m_force.x = maxVel;
		m_force.x /= maxVel;
	}
	else
	{
		m_force.z = 0;
		m_force.x = 0;
	}
	_sendForceToGG();
	return true;
#else
	return false;

#endif
}

void FingerTipComponent::_sendForceToGG()
{

	if (m_ggDriver)
	{
		const bool threeType = false; // true if using shearing forces
		if (threeType)
		{
			m_ggDriver->SetChannelValue(m_channel * 2 + 0, m_force.x);//shearing force
			//m_ggDriver->SetChannelValue(m_channel * 3 + 1, 0);				//always zero
			m_ggDriver->SetChannelValue(m_channel * 2 + 1, m_force.z);//pressure force
		}
		else
		{
			m_ggDriver->SetChannelValue(m_channel, abs(m_force.z));//shearing force
		}
	}
}

void FingerTipComponent::Update(float dt)
{
	if (!m_enabled)
	{
		if (m_wasEnabled)
		{
			m_wasEnabled = false;
			m_force = 0;
			_sendForceToGG();
		}
		return;
	}
	m_wasEnabled = true;
	VT::MountableComponent::Update(dt);

	//calculate finger position in camera space
	if ( !ATAppGlobal::Instance()->headObject)
		return;
	_calculateForce(dt);
}

void FingerTipComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
// 	if (!m_enabled)
// 		return;
	IGameComponent::DebugRender(renderer);
	if (!AppData::Instance()->IsDebugging)
		return;
	math::vector3d pos;
	math::quaternion ori;
	video::SColor clr[2] = { video::DefaultColors::Red, video::DefaultColors::Green };

	pos = GetAbsolutePosition();
	renderer->AddCross(pos, 0.01, 1);

	renderer->AddArrow(pos, pos + GetAbsoluteOrientation()* math::vector3d(m_force.x, 0, 0), video::DefaultColors::Red);
	renderer->AddArrow(pos, pos + GetAbsoluteOrientation()* math::vector3d(0, 0, m_force.z), video::DefaultColors::Blue);
// 	renderer->AddArrow(pos, pos + GetAbsoluteOrientation()* math::vector3d(m_avgVelocity.x,0,0), video::DefaultColors::Red);
// 	renderer->AddArrow(pos, pos + GetAbsoluteOrientation()* math::vector3d(0, m_avgVelocity.y, 0), video::DefaultColors::Green);
// 	renderer->AddArrow(pos, pos + GetAbsoluteOrientation()* math::vector3d(0, 0, m_avgVelocity.z), video::DefaultColors::Blue);
	//renderer->AddArrow(pos, pos +  m_avgVelocity.z, video::DefaultColors::LightPink,2);
}
void FingerTipComponent::OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp)
{
	if (!m_enabled)
		return;
	VT::MountableComponent::OnGUIRender(renderer,vp);
	math::vector2d p;
	p.x = vp.ULPoint.x+m_projPos.x*vp.getWidth();
	p.y = vp.ULPoint.y + m_projPos.y*vp.getHeight();
	renderer->AddQuad(0, math::rectf(p.x - 5, p.y - 5, p.x + 5, p.y + 5), math::rectf(0, 0, 1, 1), 1);

	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
	AppData* app = AppData::Instance();
	if (font)
	{
		attr.fontColor.Set(1, 0, 0, 1);
		attr.fontAligment = GUI::EFA_TopLeft;
		attr.fontSize = 18;
		attr.hasShadow = true;
		attr.shadowColor.Set(0, 0, 0, 1);
		attr.shadowOffset = math::vector2d(2);
		attr.spacing = 2;
		attr.wrap = 0;
		attr.RightToLeft = 0;

		math::rectf r = vp;
		r.ULPoint.y += 30*m_index;

		core::string msg = mT("Finger: ") + m_name + " - Proj=" + core::StringConverter::toString(m_depthSpace) + " - Force=" + core::StringConverter::toString(m_force);
		font->print(r, &attr, 0, msg, renderer);
		r.ULPoint.y += attr.fontSize + 5;
	}

}

IMPLEMENT_GAMECOMPONENT_FACTORY(FingerTipComponent);

}
}
