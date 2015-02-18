

/********************************************************************
	created:	2012/12/25
	created:	25:12:2012   17:38
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\ContactCollisionDebugger.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	ContactCollisionDebugger
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __ContactCollisionDebugger__
#define __ContactCollisionDebugger__




namespace mray
{
namespace VT
{

	class ContactCollisionDebuggerImpl;

class ContactCollisionDebugger
{
protected:
	ContactCollisionDebuggerImpl* m_impl;
public:
	ContactCollisionDebugger();
	virtual~ContactCollisionDebugger();

	void Init(game::GameEntityManager* mngr);

	void Draw(scene::IViewNode* cam);
	void Update(float dt);
	int GetLastContactCount();
	float GetAverageForce();
};

}
}


#endif
