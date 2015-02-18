
#include "GameOptionManager.h"

#include <IFileSystem.h>
#include <EnumManager.h>
#include <XMLWriter.h>

namespace mray{
namespace gameMod{

GameOptionManager::GameOptionManager(const core::string&confingFileName){

	m_confingFileName=confingFileName;
	GCPtr<OS::IStream> conf=gFileSystem.openFile(confingFileName);
	if(!conf){
	}else{
	}

	gameSoundVol=1;

	textureQuality=ETQ_Low;
	viewDistance=50;
	postEffect=false;

	StartRegisterEnum(ETextureQuality);
	RegisterEnumValue(ETQ_Low);
	RegisterEnumValue(ETQ_High);
	EndRegisterEnum();

}
GameOptionManager::~GameOptionManager(){
}

void GameOptionManager::exportSettings(const core::string&confingFileName){

/*	GCPtr<OS::IStream> conf=gFileSystem.openFile(confingFileName,OS::BIN_WRITE);
	if(!conf)
		return;
	GCPtr<xml::XMLWriter> writer=new xml::XMLWriter(conf);

	xml::XMLElement*elem=new xml::XMLElement(mT("config"));
	elem->addAttribute(mT("texQuality"),core::StringConverter::toString((int)textureQuality,mT("ETextureQuality")));
	elem->addAttribute(mT("viewDist"),core::StringConverter::toString(viewDistance));
	elem->addAttribute(mT("postEffect"),core::StringConverter::toString(postEffect));
*/
}

}
}


