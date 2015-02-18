

#include "stdafx.h"
#include "CommonValueProviders.h"
#include "ITimer.h"
#include "FPSCalc.h"
#include "Engine.h"


namespace mray
{
	void TimeValueProvier::SetValue(const float &v)
	{
	}
	float TimeValueProvier::GetValue()
	{
		return gEngine.getTimer()-> getSeconds();
	}


	void FPSValueProvier::SetValue(const float &v)
	{
	}
	float FPSValueProvier::GetValue()
	{
		return gEngine.getFPS()->dt();
	}
}