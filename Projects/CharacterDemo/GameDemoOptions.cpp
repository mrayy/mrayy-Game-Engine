
#include "stdafx.h"
#include "GameDemoOptions.h"

namespace mray
{


GameDemoOptions::GameDemoOptions(const core::string&confingFileName)
{

	m_confingFileName=confingFileName;
	GCPtr<OS::IStream> conf=gFileSystem.openFile(confingFileName);
	if(!conf){
	}else{
	}

	gameSoundVol=1;

	viewDistance=50;
	postEffect=false;

}
GameDemoOptions::~GameDemoOptions()
{

}

void GameDemoOptions::exportSettings(const core::string&confingFileName)
{

}


}