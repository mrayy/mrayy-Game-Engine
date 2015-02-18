

#include "stdafx.h"
#include "RobotModeState.h"




namespace mray
{
namespace VT
{


RobotModeState::RobotModeState(const core::string&name) :IRenderingState(name)
{
}

RobotModeState::~RobotModeState()
{
}


void RobotModeState::InitState()
{
	IRenderingState::InitState();
}

void RobotModeState::OnEvent(Event* e)
{
}

void RobotModeState::OnEnter(IRenderingState*prev)
{
}
void RobotModeState::OnExit()
{
	IRenderingState::OnExit();
}

void RobotModeState::Update(float dt)
{
}

video::IRenderTarget* RobotModeState::Render(const math::rectf& rc,TBee::ETargetEye eye)
{
	return IRenderingState::Render(rc,eye);
}


void RobotModeState::LoadFromXML(xml::XMLElement* e)
{
}


}
}
