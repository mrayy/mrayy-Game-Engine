#include "stdafx.h"

#include "FreeImageCodec.h"

#include <StringConverter.h>
#include <LogManager.h>
#include <FreeImage.h>

namespace mray
{
namespace loaders
{

int FreeImageCodec::s_FreeImageRefCtr=0;


void FreeImageErrorHandler(FREE_IMAGE_FORMAT fif, const char *message) 
{
	// Callback method as required by FreeImage to report problems
	const char* typeName = FreeImage_GetFormatFromFIF(fif);
	if (typeName)
	{
		core::string msg=mT("FreeImage Error: '");
		msg+=core::StringConverter::toString(message);
		msg+=mT("' While Processing Format");
		msg+=core::StringConverter::toString(typeName);
		gLogManager.log(msg,ELL_WARNING);
	}
	else
	{
		core::string msg=mT("FreeImage Error: '");
		msg+=core::StringConverter::toString(message);
		msg+=mT("'");
		gLogManager.log(msg,ELL_WARNING);
	}

}


void FreeImageCodec::IncreaseRef()
{
	if(s_FreeImageRefCtr==0)
	{

		gLogManager.startSection(mT("FreeImage"));
		FreeImage_Initialise(false);

		gLogManager.log(mT("Version: ")+ core::StringConverter::toString(FreeImage_GetVersion()),ELL_INFO);
		gLogManager.log(mT("Copyright: ")+ core::StringConverter::toString(FreeImage_GetCopyrightMessage()),ELL_INFO);

		core::string formats=mT("Avaliable Formats: ");
		for(int i=0;i<FreeImage_GetFIFCount();++i){
			core::string ext=core::StringConverter::toString(FreeImage_GetFIFExtensionList((FREE_IMAGE_FORMAT)i));

			formats += ext;
			formats +=mT(",");
		}
		gLogManager.log(formats,ELL_INFO);

		FreeImage_SetOutputMessage(FreeImageErrorHandler);
		gLogManager.endSection(true);

	}
	++s_FreeImageRefCtr;
}

void FreeImageCodec::DecreaseRef()
{
	--s_FreeImageRefCtr;
	if(s_FreeImageRefCtr==0)
	{
		FreeImage_DeInitialise();
	}
}


}
}


