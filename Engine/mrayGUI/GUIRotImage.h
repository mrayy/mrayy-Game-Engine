#ifndef ___GUIRotImage___
#define ___GUIRotImage___

#include "GUIImage.h"

namespace mray{
namespace GUI{

class MRAY_DLL GUIRotImage:public GUIImage
{
protected:

	float m_rotationSpeed;
	const float& getRotationSpeed()  { return m_rotationSpeed; }
	void setRotationSpeed(const float &val) { m_rotationSpeed = val; }

	virtual void fillProperties();
public:
	DECLARE_RTTI;

	rwProperty<GUIRotImage,float> RotationSpeed;

	GUIRotImage();
	GUIRotImage(GUIManager* manager,math::vector2d pos,math::vector2d size,GCPtr<GUIElement> parent=0,int id=0);
	virtual ~GUIRotImage();

	virtual void draw(float dt);
	virtual void onEvent(const EventPtr& event);


};

}
}


#endif



