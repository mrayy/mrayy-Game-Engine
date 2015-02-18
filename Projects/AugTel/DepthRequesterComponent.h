

/********************************************************************
	created:	2014/02/17
	created:	17:2:2014   4:12
	filename: 	C:\Development\mrayEngine\Projects\AugTel\DepthRequesterComponent.h
	file path:	C:\Development\mrayEngine\Projects\AugTel
	file base:	DepthRequesterComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __DepthRequesterComponent__
#define __DepthRequesterComponent__


#include "MountableComponent.h"

namespace mray
{
namespace AugTel
{

class DepthRequesterComponent:public VT::MountableComponent
{
	DECLARE_RTTI;
protected:
public:
	DepthRequesterComponent();
	virtual~DepthRequesterComponent();
};

DECLARE_GAMECOMPONENT_FACTORY(PenComponent);
}
}


#endif
