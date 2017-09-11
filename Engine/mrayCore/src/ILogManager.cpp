#include "stdafx.h"

#include "ILogManager.h"


namespace mray{

	mchar*  ELogLevelStr[]={
		mT("Info"),
		mT("Success"),
		mT("Warning"),
		mT("Error")
	};

	const mchar* ILogManager::getLogLevelStr(ELogLevel l){
		return ELogLevelStr[l];
	}

}
