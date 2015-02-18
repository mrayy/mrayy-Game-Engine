
#include "IDManager.h"


namespace mray{

IDManager::IDManager(){
	m_id=1;
}
IDManager::~IDManager(){

}

uint IDManager::getID(){
	return m_id++;
}
void IDManager::freeID(uint id){
}

}

