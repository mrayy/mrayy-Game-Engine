

/********************************************************************
	created:	2014/03/03
	created:	3:3:2014   15:43
	filename: 	C:\Development\mrayEngine\Engine\mrayEngine\include\CommonValueProviders.h
	file path:	C:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	CommonValueProviders
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __CommonValueProviders__
#define __CommonValueProviders__


#include "ValueController.h"


namespace mray
{

	class TimeValueProvier :public ControllerValue<float>
	{
	public:
		void SetValue(const float &v);
		float GetValue();
	};

	class FPSValueProvier :public ControllerValue<float>
	{
	public:
		void SetValue(const float &v);
		float GetValue();
	};
}


#endif
