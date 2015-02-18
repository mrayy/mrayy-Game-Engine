// VTSensorLibrary.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

#include "IPlugin.h"
#include "GameComponentCreator.h"


#include "GyroComponent.h"
#include "AccelerometerComponent.h"
#include "GPSComponent.h"
#include "MotorEncoderComponent.h"
#include "DistanceComponent.h"


namespace mray
{
	namespace VT
	{

		class VTSensorLibraryPlugin:public IPlugin
		{
		public:
			VTSensorLibraryPlugin()
			{
			}

			virtual core::string getName()
			{
				return mT("VTSensorLibrary");
			}

			virtual void install()
			{
				IPlugin::install();

				REGISTER_COMPONENT_FACTORY(GyroComponent);
				REGISTER_COMPONENT_FACTORY(AccelerometerComponent);
				REGISTER_COMPONENT_FACTORY(GPSComponent);
				REGISTER_COMPONENT_FACTORY(MotorEncoderComponent);
				REGISTER_COMPONENT_FACTORY(DistanceComponent);
			}
			virtual void uninstall(){
				IPlugin::uninstall();

				UNREGISTER_COMPONENT_FACTORY(GyroComponent);
				UNREGISTER_COMPONENT_FACTORY(AccelerometerComponent);
				UNREGISTER_COMPONENT_FACTORY(GPSComponent);
				UNREGISTER_COMPONENT_FACTORY(MotorEncoderComponent);
				UNREGISTER_COMPONENT_FACTORY(DistanceComponent);
			}
		};


	}
}

IMPLEMET_PLUGIN_FUNCTION(mray::VT::VTSensorLibraryPlugin);



BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved )
{
	return TRUE; 
}

