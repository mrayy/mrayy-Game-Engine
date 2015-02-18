

/********************************************************************
	created:	2012/08/07
	created:	7:8:2012   14:41
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IFeedbackControl.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IFeedbackControl
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IFeedbackControl___
#define ___IFeedbackControl___

#include "IValue.h"
#include "IVTComponent.h"

namespace mray
{
namespace VT
{
	class IFeedbackControl;
	class IVTComponent;
	//this will contain a feedback result from sensors components
	class IFeedbackResult
	{
	public:
		IFeedbackResult():owner(0)
		{
		}
		//name of the feedback result
		core::string name;
		
		//value of this feedback
		virtual void* GetData()=0;
		virtual core::string ToString()=0;
		
		//sensor who created this
		IFeedbackControl* owner;
	};
	template<class T>
	class TFeedbackResult:public IFeedbackResult
	{
	public:
		T value;
		void* GetData()
		{
			return &value;
		}
		core::string ToString()
		{
			return core::StringConverter::toString(value);
		}
	};

	typedef TFeedbackResult<float> FeedbackResultF;
	typedef TFeedbackResult<math::vector3d> FeedbackResultV3;

	class IFeedbackControlListener
	{
	public:
		virtual void OnFeedbackValue(IFeedbackControl*p,IFeedbackResult* r){}
	};

class IFeedbackControl:public ListenerContainer<IFeedbackControlListener*>
{
protected:

	DECLARE_FIRE_METHOD(OnFeedbackValue,(IFeedbackControl*p,IFeedbackResult* r),(p,r));
public:
	IFeedbackControl(){}
	virtual~IFeedbackControl(){}

};




}
}

#endif
