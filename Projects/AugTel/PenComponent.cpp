
#include "stdafx.h"
#include "PenComponent.h"

#include "AppData.h"
#include "IDebugDrawManager.h"
#include "INetwork.h"

#include "ATAppGlobal.h"
#include "HeadMount.h"
#include "SceneComponent.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "FontResourceManager.h"

namespace mray
{
namespace AugTel
{

	IMPLEMENT_SETGET_PROP(PenComponent, Threshold, float, m_threshold, 0.01, core::StringConverter::toFloat, core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(PenComponent, DrawIP, core::string, m_drawIp, "", ,);
	IMPLEMENT_SETGET_PROP(PenComponent, DrawPort, int, m_drawPort, 5123, core::StringConverter::toInt, core::StringConverter::toString);
	IMPLEMENT_SETGET_PROP(PenComponent, PenTipName, core::string, m_penTipName, "", , );

	class MovingAverage
	{
		std::list<math::vector3d> m_points;
		math::vector3d m_average;

		std::vector<float> m_kernel;
		int m_cnt;
	public:
		MovingAverage()
		{
			m_cnt = 20;
			float s = 0;
			float w = 0;
			for (int i = 0; i < m_cnt; ++i)
			{
				w = powf(i + 1, 2.0f);
				m_kernel.push_back(w);
				s += w;
			}
			for (int i = 0; i<m_kernel.size(); ++i)
				m_kernel[i] /= s;
		}
		math::vector3d AddPoint(math::vector3d& pt)
		{
			m_points.push_back(pt);
			if (m_points.size() > m_cnt)
			{
				m_points.pop_front();
			}
			std::list<math::vector3d>::iterator it = m_points.begin();
			m_average = 0;
			for (int i = 0; it != m_points.end(); ++it, ++i)
			{
				m_average += *it*m_kernel[i];
			}
			//m_average /= m_points.size();
			return m_average;

		}
	};
PenComponent::PenComponent(game::GameEntityManager*m)
:MountableComponent(m)
{
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeThreshold::instance);
		dic->addPropertie(&PropertyTypeDrawIP::instance);
		dic->addPropertie(&PropertyTypeDrawPort::instance);
		dic->addPropertie(&PropertyTypePenTipName::instance);
	}
	m_threshold = 0.01;

	m_sender = 0;

	m_drawIp = "127.0.0.1";
	m_drawPort = 5123;

	m_points.clr = video::DefaultColors::Blue;
	m_points.width = 7;
	m_remove = false;
	m_penTip = 0;

	m_movingAvg = new MovingAverage;
}


PenComponent::~PenComponent()
{
	delete m_movingAvg;
}


int PenComponent::_writeLine(OS::StreamWriter& wrtr, const PenComponent::LineInfo* ret)
{
	int len = 0;
	len += wrtr.binWriteFloat(ret->width);
	len += wrtr.binWriteFloat(ret->clr.R);
	len += wrtr.binWriteFloat(ret->clr.G);
	len += wrtr.binWriteFloat(ret->clr.B);
	len += wrtr.binWriteFloat(ret->clr.A);
	len += wrtr.binWriteInt(ret->points.size());
	for (int i = 0; i < ret->points.size(); ++i)
	{
		math::vector2d pt=(ret->points[i]-math::vector2d(0,0.25))*math::vector2d(1,2);
		len += wrtr.binWriteFloat(pt.x);
		len += wrtr.binWriteFloat(pt.y);
	}
	return len;
}
void PenComponent::_Send(EDrawMessages msg, const PenComponent::LineInfo* line)
{
	const uint Size = 6000;
	byte buffer[Size];
	int len = 0;
	OS::CMemoryStream stream("", buffer, Size, false,OS::BIN_WRITE);
	OS::StreamWriter wrt(&stream);
	len += wrt.binWriteInt((int)msg);
	if (msg == EDrawMessages::AddLine)
	{
		len += _writeLine(wrt, line);
	}
	else
	if (msg == EDrawMessages::RemoveLastLine)
	{
	}
	else
	if (msg == EDrawMessages::CurrentPos)
	{
		len += stream.write(&m_currentProjPos, sizeof(m_currentProjPos));
	}
	else
	if (msg == EDrawMessages::CurrentLine)
	{
		len += _writeLine(wrt, line);
	}

	m_sender->SendTo(&m_addr, (const char*)buffer, len);
}

bool PenComponent::InitComponent()
{
	if (!MountableComponent::InitComponent())
		return false;
	m_writing = false;
	m_sender = network::INetwork::getInstance().createUDPClient();
	m_sender->Open();
	m_addr = network::NetAddress(m_drawIp, m_drawPort);
	m_sender->Connect(m_addr);
	m_penTipMtrl = 0;
	game::SceneComponent* comp = IGameComponent::RetriveComponent<game::SceneComponent>(m_ownerComponent, m_penTipName);
	if (comp)
	{
		comp->InitComponent();
		m_penTip = comp->GetSceneNode();

		scene::IRenderable* r= m_penTip->GetAttachedNode(0);
		if (r)
		{
			 video::RenderMaterial* mtrl= r->getMaterial(0);
			 if (mtrl)
			 {
				 m_penTipMtrl= mtrl->GetTechniqueAt(0)->GetPassAt(0);
			 }
		}
	}
	return true;
}

void PenComponent::Update(float dt)
{
	MountableComponent::Update(dt);
	if (!m_node)return;

	math::vector3d pos = GetAbsolutePosition();;
	pos = m_movingAvg->AddPoint(pos);

	if (TBee::AppData::Instance()->inputMngr->getKeyboard()->getKeyState(KEY_RETURN))
	{
		if (!m_calibrated)
		{
			if (!m_calibPointInsert)
			{
				m_calibPointInsert = true;
				m_calibPoints[m_currentCalibPoint].x = pos.x;
				m_calibPoints[m_currentCalibPoint].y = pos.y;
				m_currentCalibPoint++;

				if (m_currentCalibPoint == 4)
				{
					m_calibrated = true;
					m_wrapper.setSource(m_calibPoints[0], m_calibPoints[1], m_calibPoints[2], m_calibPoints[3]);
				}
			}
		}
		else
		{
			m_writing = true;
		}
			if (m_penTipMtrl)
			{
				m_penTipMtrl->SetDiffuse(video::SColor(0.1, 0.1, 1, 1));
				m_penTip->setScale(2);
			}
	}
	else
	{
		m_calibPointInsert = false;
		if (m_calibrated)
		{
			if (m_writing)
			{
				if (m_points.points.size() > 0)
					_Send(EDrawMessages::AddLine, &m_points);
				m_points.points.clear();
			}
			m_writing = false;
		}
		if (m_penTipMtrl)
			m_penTipMtrl->SetDiffuse(video::SColor(0, 0, 0, 1));
		m_penTip->setScale(1);

	}

	if (TBee::AppData::Instance()->inputMngr->getKeyboard()->getKeyState(KEY_ESCAPE))
	{
		if (!m_remove)
			_Send(EDrawMessages::RemoveLastLine,0);
		m_remove = true;
	}
	else
	{
		m_remove = false;
	}


	scene::CameraNode* camera = ATAppGlobal::Instance()->headObject->GetRightCamera();

	//math::vector2d proj = m_wrapper.Wrap(math::vector2d(pos.x, pos.y));

	
	math::matrix4x4 mat = camera->getProjectionMatrix()*camera->getViewMatrix();
	math::vector4d projPos = mat*math::vector4d(pos.x, pos.y, pos.z, 1);
	math::vector2d proj;
	proj.x = projPos.x / projPos.w;
	proj.y = -projPos.y/projPos.w;
	proj.x = (proj.x + 1)*0.5;
	proj.y = (proj.y + 1)*0.5;
	/*math::vector3d projPos = ATAppGlobal::Instance()->headObject->GetRightCamera()->WorldToScreen(pos);
	proj.x = projPos.x ;
	proj.y = projPos.y ;*/

	//proj = proj*math::vector2d(ATAppGlobal::Instance()->App->GetRenderWindow()->GetSize());

	float dp = pos.getDist(m_lastPos);
	if (dp > m_threshold && m_writing)
	{
		m_lastPos = pos;
		m_points.points.push_back(proj);
		_Send(EDrawMessages::CurrentLine, &m_points);
	}
	m_currentProjPos = proj;
	_Send(EDrawMessages::CurrentPos, 0);

}


void PenComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	MountableComponent::DebugRender(renderer);
}

void PenComponent::OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp)
{
	MountableComponent::OnGUIRender(renderer, vp);
	video::IVideoDevice* device= Engine::getInstance().getDevice();
	if (m_points.points.size() > 1)
	{
		device->setLineWidth(m_points.width);
		std::vector<math::vector2d> pts;
		pts.resize(m_points.points.size());
		for (int i = 0; i < m_points.points.size(); ++i)
		{
			pts[i] = m_points.points[i] * vp.getSize();
		}
		device->draw2DLine(&pts[0], pts.size(), m_points.clr);
	}


	GUI::IFont* font = gFontResourceManager.getDefaultFont();
	GUI::FontAttributes attr;
	video::IVideoDevice* dev = Engine::getInstance().getDevice();
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
		r.ULPoint.y += 200;

		core::string msg = mT("Pos: ") + core::StringConverter::toString(m_currentProjPos);
		font->print(r, &attr, 0, msg, renderer);
		r.ULPoint.y += attr.fontSize + 5;
	}
}

}
}

