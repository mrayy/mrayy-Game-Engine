

#include "stdafx.h"
#include "BaseRenderState.h"
#include "IRenderingState.h"


namespace mray
{
namespace TBee
{
BaseRenderState::BaseRenderState(const core::string&name) :IState(name)
{
}
BaseRenderState::~BaseRenderState()
{
}


void BaseRenderState::onEnter(IState*prev)
{
	IRenderingState*p = dynamic_cast<IRenderingState*>(prev);
	OnEnter(p);
}
void BaseRenderState::onUpdate()
{
}
void BaseRenderState::onExit()
{
}

}
}
