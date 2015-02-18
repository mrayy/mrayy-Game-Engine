
/********************************************************************
	created:	2008/12/30
	created:	30:12:2008   23:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IJoysticController.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IJoysticController
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IJoysticController___
#define ___IJoysticController___

//#include "ISingleton.h"
#include "compileconfig.h"



namespace mray{

namespace controllers{

class MRAY_DLL JoysticPov
{
public:
	JoysticPov() : direction(0) {}

	static const int Centered  = 0x00000000;
	static const int North     = 0x00000001;
	static const int South     = 0x00000010;
	static const int East      = 0x00000100;
	static const int West      = 0x00001000;
	static const int NorthEast = 0x00000101;
	static const int SouthEast = 0x00000110;
	static const int NorthWest = 0x00001001;
	static const int SouthWest = 0x00001010;

	int direction;
};

class MRAY_DLL JoysticSlider
{
public:
	JoysticSlider() : abX(0), abY(0) {};
	//! true if pushed, false otherwise
	int abX, abY;
};
class JoysticAxis {
public:
	JoysticAxis() : abs(0), rel(0), absOnly(false) {};


	//! Absoulte and Relative value components
	float abs, rel;

	//! Indicates if this Axis only supports Absoulte (ie JoyStick)
	bool absOnly;
};




class MRAY_DLL IJoysticController//:public ISingleton<IJoysticController>
{
	//! Represents all the buttons 
	std::vector<bool> m_buttons;
	//! Represents all the single axes on the device
	std::vector<JoysticAxis> m_Axis;
	//! Represents all Vector type controls the device exports
	std::vector<math::vector3d> m_Vectors;

	//! Represents the value of a POV. Maximum of 4
	JoysticPov m_Pov[4];
	//! Represent the max sliders
	JoysticSlider m_sliders[4];


public:
	IJoysticController(){};
	virtual ~IJoysticController(){};

	virtual void reset()=0;

	virtual bool getButtonState(int index)=0;
	virtual void setButtonState(int index,bool v)=0;
	virtual void copyButtonState(bool*v,int cnt)=0;

	virtual JoysticAxis getAxisState(int index)=0;
	virtual void setAxisState(int index,JoysticAxis state)=0;

	virtual math::vector3d getVectorState(int index)=0;
	virtual void setVectorState(int index,math::vector3d state)=0;

	virtual JoysticPov getPovState(int index)=0;
	virtual void setPovState(int index,JoysticPov state)=0;

	virtual JoysticSlider getSliderState(int index)=0;
	virtual void setSliderState(int index,JoysticSlider state)=0;
};
//#define gJoysticController mray::controllers::IJoysticController::getInstance()


}

}


#endif //___IJoysticController___
