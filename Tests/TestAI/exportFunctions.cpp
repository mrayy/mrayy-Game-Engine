
#include "exportFunctions.h"
#include <LogManager.h>
#include <stringutilities.h>



void logWarn(const char*msg){
	
	gLogManager.log(core::UToString::toString(msg),ELL_WARNING);
}
void logErr(const char*msg){
	gLogManager.log(core::UToString::toString(msg),ELL_ERROR);
}
void logSucc(const char*msg){
	gLogManager.log(core::UToString::toString(msg),ELL_SUCCESS);
}

