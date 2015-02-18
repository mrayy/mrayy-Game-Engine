


/********************************************************************
	created:	2012/01/26
	created:	26:1:2012   11:55
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IGUISliderBar.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IGUISliderBar
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGUISliderBar__
#define __IGUISliderBar__

#include "IGUIElement.h"

namespace mray
{
namespace GUI
{

	enum ESliderBarDirection
	{
		ESBDir_Horizontal,
		ESBDir_Vertical
	};
class MRAY_DLL IGUISliderBar:public IGUIElement
{
private:
protected:
	virtual void fillProperties();
public:
	static const GUID ElementType;

	ObjectEvent OnChanged;//args=value

	//DECLARE_PROPERTY_TYPE(Direction, ESliderBarDirection, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Value, float, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(MinValue, float, MRAY_DLL);
	DECLARE_PROPERTY_TYPE(MaxValue, float, MRAY_DLL);

public:
	IGUISliderBar(IGUIManager* creator);
	virtual~IGUISliderBar();

	virtual bool SetDirection(ESliderBarDirection val)=0;
	virtual ESliderBarDirection GetDirection()const=0;

	virtual bool SetValue(float val)=0;
	virtual float GetValue()const=0;

	virtual bool SetMaxValue(float val)=0;
	virtual float GetMaxValue()const=0;

	virtual bool SetMinValue(float val)=0;
	virtual float GetMinValue()const=0;

};

}
}

#endif

