

#include "stdafx.h"
#include "WiiboardInputController.h"

#include "WiimoteManager.h"
#include "CWiimote.h"
#include "AppData.h"
#include "InputManager.h"
#include "IKeyboardController.h"

namespace mray
{
namespace TBee
{

#define INITIAL_WEIGHT 10
WiiboardInputController::WiiboardInputController()
{
	for (int i = 0; i < 4; ++i)
		m_vals[i] = INITIAL_WEIGHT;
}

WiiboardInputController::~WiiboardInputController()
{
}



bool WiiboardInputController::IsButtonPressed(EInputButton button)
{
	return false;
}

math::vector2d WiiboardInputController::_CalcSpeedVector()
{
#define LimitNormValue(idx,v)m_vals[idx].x=math::Min(m_vals[idx].x,v);m_vals[idx].y=math::Max(m_vals[idx].y,v);v=(v-m_vals[idx].x)/(m_vals[idx].y-m_vals[idx].x);
	controllers::CWiimote* mote = controllers::WiimoteManager::getInstance().GetByID(0);
	if (!mote)
		return 0;
	if (!mote->IsBalanceBoard())
		return 0;
	float tl=mote->BalanceBoard.Lb.TopL;
	float tr = mote->BalanceBoard.Lb.TopR;
	float bl = mote->BalanceBoard.Lb.BottomL*0.5f;
	float br = mote->BalanceBoard.Lb.BottomR*0.5f;

	LimitNormValue(0, tl);
	LimitNormValue(1, tr);
	LimitNormValue(2, bl);
	LimitNormValue(3, br);

	float t = (tl * tr);
	float b = (bl * br);
	float l = (br * tr);
	float r = (bl * tl);

	math::vector2d vec;
	vec.x=(r - l);
	vec.y=(t - b);

	float sx = math::sign(vec.x);
	float sy = math::sign(vec.y);

//  	vec.x = pow(abs(vec.x), 0.7)*sx;
//  	vec.y = pow(abs(vec.y), 0.7)*sy;

	vec.x = -vec.x;
	vec.y = -vec.y;
	return vec*2;
}

math::vector2d WiiboardInputController::GetSpeed()
{
	math::vector2d sv = _CalcSpeedVector();
	return math::vector2d(sv.y, 0);
}

float WiiboardInputController::GetRotation()
{
	math::vector2d sv = _CalcSpeedVector();
	return sv.x;
}

}
}
