
/********************************************************************
	created:	2009/04/05
	created:	5:4:2009   23:59
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\IPlaneController.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	IPlaneController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPlaneController___
#define ___IPlaneController___

#include "AirPlane.h"
#include <GCPtr.h>

namespace mray{
namespace gameMod{

class IPlaneController
{
private:
protected:
	GCPtr<AirPlane> m_plane;
public:
	IPlaneController(){}
	virtual~IPlaneController(){}


	virtual void attachToPlane(GCPtr<AirPlane> p){
		m_plane=p;
	}
	virtual GCPtr<AirPlane> getAttachedPlane(){
		return m_plane;
	}

	virtual void control()=0;	// update the plane
	virtual bool isNPC()=0;		// non player controll?
};

}
}


#endif //___IPlaneController___
