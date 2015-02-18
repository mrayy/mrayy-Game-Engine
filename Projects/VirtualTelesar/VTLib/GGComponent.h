

/********************************************************************
	created:	2013/05/17
	created:	17:5:2013   20:15
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\GGComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	GGComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __GGComponent__
#define __GGComponent__



#include "IVTComponent.h"


namespace mray
{
namespace VT
{
	class GGComponentImpl;
class GGComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:
	GGComponentImpl* m_impl;
public:
	DECLARE_PROPERTY_TYPE(Port,int,);
	DECLARE_PROPERTY_TYPE(IP,core::string,);
public:
	GGComponent(game::GameEntityManager*);;
	virtual~GGComponent();

	virtual bool InitComponent();

	bool SetIP(const core::string& ip);
	const core::string& GetIP();

	bool SetPort(int p);
	int GetPort();

	void SetChannelValue(int c,float v);

	virtual void Update(float dt);

	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);


};

DECLARE_GAMECOMPONENT_FACTORY(GGComponent);

}
}


#endif
