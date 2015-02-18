

/********************************************************************
	created:	2012/08/15
	created:	15:8:2012   15:37
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IControllableComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IControllableComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IControllableComponent___
#define ___IControllableComponent___

#include "ListenerContainer.h"
#include "CommunicationData.h"

namespace mray
{
namespace VT
{
	enum EControlSource
	{
		EControl_Kinematic,
		EControl_Realtime
	};
	class IControllableComponent;

	class IControllableComponentListener
	{
	public:
		virtual void OnSetControlValue(IControllableComponent* comp,const std::vector<ControlInputValues>& v,EControlSource src)=0;//{}
	};

class IControllableComponent:public ListenerContainer<IControllableComponentListener*>
{
//	DECLARE_RTTI;
//protected:

	DECLARE_FIRE_METHOD(OnSetControlValue,(IControllableComponent* comp,const std::vector<ControlInputValues>& v,EControlSource src),(comp,v,src));
public:
//	DECLARE_PROPERTY_TYPE(ControlValue,std::vector<float>,);
	
public:
	IControllableComponent();
	virtual~IControllableComponent(){}
	
	virtual const core::string& GetControllableName()=0;

	virtual bool SetControlValue(const std::vector<ControlInputValues>& v,EControlSource src)
	{
		FIRE_LISTENR_METHOD(OnSetControlValue,(this,v,src));
		return true;
	}
	//virtual const std::vector<InputValues>&  GetControlValue()=0;

	// this will calculate the current value regardless of the control value
	// (control value and current value can be different, like in joint drive)
	virtual std::vector<float> GetCurrentValue()=0;
};

}
}

#endif
