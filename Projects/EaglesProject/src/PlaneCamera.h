
/********************************************************************
	created:	2009/04/30
	created:	30:4:2009   13:35
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\src\PlaneCamera.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject\src
	file base:	PlaneCamera
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PlaneCamera___
#define ___PlaneCamera___

#include <SCameraNode.h>
#include <SceneManager.h>

namespace mray{
namespace gameMod{

class PlaneCamera:public scene::SCameraNode
{
private:
protected:

	float m_radius;
	math::vector3d m_angles;
	math::quaternion m_angleQ;
public:
	PlaneCamera(scene::SceneManager*smngr);
	virtual~PlaneCamera();

	void attachToPlane(IMovable*node);

	virtual void update(float dt);
};

}
}


#endif //___PlaneCamera___
