


/********************************************************************
created:	2012/10/29
created:	29:10:2012   18:37
filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5\NCAppGlobals.h
file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTelesar5
file base:	NCAppGlobals
file ext:	h
author:		MHD YAMEN SARAIJI

purpose:
*********************************************************************/
#ifndef ___NCAppGlobals___
#define ___NCAppGlobals___


#include "AppData.h"
#include "CameraConfigurationManager.h"
#include "InputKeyMap.h"
#include "OptiTrackDataSource.h"
#include "ValueGroup.h"


namespace mray
{
namespace NCam
{
class Application;

class NCAppGlobals :public TBee::AppData
{
protected:

public:
	NCAppGlobals() :AppControlledValues("AppValues")
	{
		App = 0;
	}
	virtual ~NCAppGlobals()
	{
		delete optiDataSource;
	}
	controllers::InputKeyMap keyMap;

	Application* App;
	math::vector2d GetStereoScaleRatio(){ return 1; }

	ValueGroup AppControlledValues;

	static NCAppGlobals* Instance(){ return (NCAppGlobals*)s_instance; }
};

}
}

#endif
