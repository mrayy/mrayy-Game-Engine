
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   0:04
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\ControlMap.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	ControlMap
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ControlMap___
#define ___ControlMap___

#include <KeyCode.h>
#include "EPlaneAction.h"

namespace mray{
namespace gameMod{

class ControlMap
{
private:
	struct SKeyMap{
		SKeyMap(){}
		SKeyMap(EPlaneAction action,EKEY_CODE key){
			this->action=action;
			this->key=key;
		}
		EPlaneAction action;
		EKEY_CODE key;
	};
	core::array<SKeyMap> keys;
public:
	ControlMap();
	virtual ~ControlMap();

	void clearKeys();

	bool getKey(EPlaneAction action,EKEY_CODE&code);
	EPlaneAction getAction(EKEY_CODE key);

	void setKey(EPlaneAction action,EKEY_CODE key);
	void addKey(EPlaneAction action,EKEY_CODE key);

};

}
}


#endif //___ControlMap___
