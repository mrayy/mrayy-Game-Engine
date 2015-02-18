
#include "HumanController.h"
#include <IKeyboardController.h>
#include <IMouseController.h>


namespace mray{
namespace gameMod{

HumanController::HumanController(){
	m_controllMap=new ControlMap();
}

HumanController::~HumanController(){
}

void HumanController::control(){
	if(!m_plane)
		return;

	
	for(int i=0;i<EPA_ActionsCount;++i){
		EKEY_CODE key;
		if(m_controllMap->getKey((EPlaneAction)i,key)){
			if(gKeyboardController.getKeyState(key)){
				m_plane->doAction((EPlaneAction)i,true);
			}else{
				m_plane->doAction((EPlaneAction)i,false);
			}
		}
	}

	if(gMouseController.isPressed(controllers::EMB_Left)){
		m_plane->doAction(EPA_Missile,true);
	}
	if(gMouseController.getDX()>0){
		m_plane->doAction(EPA_RollRight,true);
	}else if(gMouseController.getDX()<0){
		m_plane->doAction(EPA_RollLeft,true);
	}
	if(gMouseController.getDY()>0){
		m_plane->doAction(EPA_PitchDown,true);
	}else if(gMouseController.getDY()<0){
		m_plane->doAction(EPA_PitchUp,true);
	}
}

}
}
