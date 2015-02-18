

/********************************************************************
	created:	2013/05/22
	created:	22:5:2013   14:00
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\ContactDisablerComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	ContactDisablerComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ContactDisablerComponent__
#define __ContactDisablerComponent__


#include "IVTComponent.h"

namespace mray
{
namespace VT
{

class ContactDisablerComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:
	physics::IPhysicManager* m_mngr;
	std::vector<physics::IPhysicalNode*> m_nodes;
public:
	ContactDisablerComponent(game::GameEntityManager*);
	virtual~ContactDisablerComponent();

	virtual bool InitComponent();

	void RecheckObjects();
};

DECLARE_GAMECOMPONENT_FACTORY(ContactDisablerComponent);

}
}


#endif
