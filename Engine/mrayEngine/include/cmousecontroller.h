

#ifndef ___CMouseController___
#define ___CMouseController___




#include "IMouseController.h"


namespace mray{
namespace controllers{

class MRAY_DLL CMouseController:public IMouseController
{
	EMouseButton m_button;
	bool m_pressed[EMB_Buttons_Count];

	math::vector3di m_pos;
	
	math::vector3df m_dpos;

public:
	CMouseController();
	virtual ~CMouseController();

	EMouseButton getButton();
	bool isPressed(EMouseButton button);

	void setPos(math::vector3d p);
	void setDPos(math::vector3d p);

	math::vector3d getPos();
	math::vector3d getDPos();

	int getX();
	int getY();
	int getZ();
	
	float getDX();
	float getDY();
	float getDZ();


	/// set Methods

	void setButton(EMouseButton b);
	void setPressed(EMouseButton button,bool p);

	void setX(int v);
	void setY(int v);
	void setZ(int v);

	void setDX(float v);
	void setDY(float v);
	void setDZ(float v);
};


}
}

#endif



