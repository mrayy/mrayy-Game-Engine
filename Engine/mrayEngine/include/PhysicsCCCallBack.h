
/********************************************************************
	created:	2009/07/14
	created:	14:7:2009   0:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\PhysicsCCCallBack.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	PhysicsCCCallBack
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___PhysicsCCCallBack___
#define ___PhysicsCCCallBack___


#include "mTypes.h"

namespace mray{
namespace physics{

	class ICharacterController;

	struct CControllerShapeHit
	{
		ICharacterController*	controller;		//!< Current controller
		//NxShape*		shape;			//!< Touched shape
		math::vector3d	worldPos;		//!< Contact position in world space
		math::vector3d	worldNormal;	//!< Contact normal in world space
		uint			id;				//!< Reserved for future use
		math::vector3d	dir;			//!< Motion direction
		float			length;			//!< Motion length
	};
	struct CControllersHit
	{
		ICharacterController*	controller;		//!< Current controller
		ICharacterController*	other;			//!< Touched controller
	};
	enum ECControllerAction
	{
		ECCA_None,				//!< Don't apply forces to touched actor
		ECCA_Push,				//!< Automatically compute & apply forces to touched actor (push)
	};
class PhysicsCCCallBack
{
private:
protected:
public:

	virtual ECControllerAction onShapeHit(const CControllerShapeHit&h)=0;
	virtual ECControllerAction onControllerHit(const CControllersHit&h)=0;
};

}
}


#endif //___PhysicsCCCallBack___
