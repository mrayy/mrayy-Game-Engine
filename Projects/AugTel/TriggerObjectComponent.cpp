
#include "stdafx.h"
#include "TriggerObjectComponent.h"

#include "IMovableComponent.h"
#include "ATAppGlobal.h"
#include "OptiTrackDataSource.h"
#include "RobotSpaceComponent.h"

namespace mray
{
namespace AugTel
{
	IMPLEMENT_SETGET_PROP(TriggerObjectComponent, TargetNode, core::string, m_targetName, "", , );
	IMPLEMENT_SETGET_PROP(TriggerObjectComponent, Type, core::string, m_type, "", , );
	IMPLEMENT_SETGET_PROP(TriggerObjectComponent, GrabNode, core::string, m_grabName, "", , );
	IMPLEMENT_SETGET_PROP(TriggerObjectComponent, TriggerNode, core::string, m_triggerName, "", , );
	IMPLEMENT_SETGET_PROP(TriggerObjectComponent, Position, math::vector3d, m_position, 0, core::StringConverter::toVector3d, core::StringConverter::toString);

TriggerObjectComponent::TriggerObjectComponent(game::GameEntityManager*m)
{
	CPropertieDictionary* dic;
	if (CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeType::instance);
		dic->addPropertie(&PropertyTypeGrabNode::instance);
		dic->addPropertie(&PropertyTypeTriggerNode::instance);
	}
}

TriggerObjectComponent::~TriggerObjectComponent()
{
}

bool TriggerObjectComponent::InitComponent()
{
	if (!IGameComponent::InitComponent())
		return false;
	game::IMovableComponent* mcomp;
	mcomp = RetriveComponent<game::IMovableComponent>(m_ownerComponent, m_targetName);
	if (mcomp)
	{
		mcomp->InitComponent();
		m_targetNode = dynamic_cast<scene::ISceneNode*>(mcomp->GetMovableObject());
	}
	mcomp = RetriveComponent<game::IMovableComponent>(m_ownerComponent, m_grabName);
	if (mcomp)
	{
		mcomp->InitComponent();
		m_grabNode = mcomp->GetMovableObject();
	}
	mcomp = RetriveComponent<game::IMovableComponent>(m_ownerComponent, m_triggerName);
	if (mcomp)
	{
		mcomp->InitComponent();
		m_triggerNode = mcomp->GetMovableObject();
	}
	m_robotSpace = RetriveComponent<RobotSpaceComponent>(m_ownerComponent, "RobotSpace");
	return true;
}


void TriggerObjectComponent::Update(float dt)
{
	controllers::IKeyboardController* kb= ATAppGlobal::Instance()->inputMngr->getKeyboard();
	if (!m_isMounted)
	{
		if (m_robotSpace->IsVisible() || kb->getKeyState(KEY_G) )
		{
			math::vector3d wp=m_robotSpace->TransformToWorld(m_grabNode->getAbsolutePosition());
			float len = wp.getDist(m_position);
			
			if (len < 0.3 || true)
			{
				m_targetNode->setVisible(true);
				m_isMounted = true;
				m_grabNode->addChild(m_targetNode);
				m_targetNode->setPosition(0);
				m_targetNode->setOrintation(math::quaternion::Identity);
			}
		}

	}
	else
	{
		if (kb->getKeyState(KEY_H))
		{
			m_isMounted = false;
			m_targetNode->setVisible(false);
			m_targetNode->removeFromParent();
		}
	}
}


void TriggerObjectComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
}

void TriggerObjectComponent::OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp)
{
}

}
}

