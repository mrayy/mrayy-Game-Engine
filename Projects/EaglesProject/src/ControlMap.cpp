
#include "ControlMap.h"

namespace mray{
namespace gameMod{

ControlMap::ControlMap(){
	keys.push_back(SKeyMap(EPA_PitchUp		,KEY_DOWN));
	keys.push_back(SKeyMap(EPA_PitchDown	,KEY_UP));
	keys.push_back(SKeyMap(EPA_RollLeft		,KEY_LEFT));
	keys.push_back(SKeyMap(EPA_RollRight	,KEY_RIGHT));
	keys.push_back(SKeyMap(EPA_YawLeft		,KEY_Q));
	keys.push_back(SKeyMap(EPA_YawRight		,KEY_E));
	keys.push_back(SKeyMap(EPA_Turbo		,KEY_W));
	keys.push_back(SKeyMap(EPA_Brake		,KEY_S));
	keys.push_back(SKeyMap(EPA_Missile		,KEY_SPACE));
}

ControlMap::~ControlMap(){
	keys.clear();
}

void ControlMap::clearKeys(){
	keys.clear();
}

bool ControlMap::getKey(EPlaneAction action,EKEY_CODE&code){
	for(int i=0;i<keys.size();++i){
		if(keys[i].action==action){
			code=keys[i].key;
			return true;
		}
	}

	return false;
}

EPlaneAction ControlMap::getAction(EKEY_CODE key){
	for(int i=0;i<keys.size();++i){
		if(keys[i].key==key)
			return keys[i].action;
	}
	return EPA_NONE;
}

void ControlMap::setKey(EPlaneAction action,EKEY_CODE key){
	for(int i=0;i<keys.size();++i){
		if(keys[i].action==action)
			keys[i].key=key;
	}
}
void ControlMap::addKey(EPlaneAction action,EKEY_CODE key){
	for(int i=0;i<keys.size();++i){
		if(keys[i].action==action){
			keys[i].key=key;
			return;
		}
	}
	keys.push_back(SKeyMap(action,key));
}

}
}
