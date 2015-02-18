

#ifndef ___CJoysticController___
#define ___CJoysticController___


#include "IJoysticController.h"


namespace mray{
	class mrayWin;
namespace controllers{


class MRAY_DLL CJoysticController:public IJoysticController
{
public:
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

	CJoysticController();
	virtual ~CJoysticController();

	void reset();

	bool getButtonState(int index);
	void setButtonState(int index,bool v);
	void copyButtonState(bool*v,int cnt);

	JoysticAxis getAxisState(int index);
	void setAxisState(int index,JoysticAxis state);
	
	math::vector3d getVectorState(int index);
	void setVectorState(int index,math::vector3d state);
	
	JoysticPov getPovState(int index);
	void setPovState(int index,JoysticPov state);
	
	JoysticSlider getSliderState(int index);
	void setSliderState(int index,JoysticSlider state);
};



}
}




#endif



