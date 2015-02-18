
/********************************************************************
	created:	2010/04/03
	created:	3:4:2010   19:12
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayAI\IAIPhysics.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayAI
	file base:	IAIPhysics
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	 
*********************************************************************/

#ifndef ___IAIPhysics___
#define ___IAIPhysics___

#include "compileConfig.h"
#include "IAIComponent.h"
#include <quaternion.h>
#include <Point3d.h>

namespace mray{
namespace AI{

	class IRenderDevice;

class MRAY_AI_DLL IAIPhysics:public IAIComponent
{
	DECLARE_RTTI
private:
protected:
public:
	IAIPhysics();
	virtual~IAIPhysics();


	virtual void Visualize(IRenderDevice*dev);

	virtual float	GetMass()=0;

	virtual void SetGlobalPosition(const math::vector3d&pos)=0;
	virtual math::vector3d GetGlobalPosition()=0;

	virtual void SetGlobalOrintation(const math::quaternion&or)=0;
	virtual math::quaternion GetGlobalOrintation()=0;

	virtual void SetLinearDamping(float d)=0;
	virtual float GetLinearDamping()=0;

	virtual void SetLinearVelocity(const math::vector3d&v)=0;
	virtual math::vector3d GetLinearVelocity()=0;

	virtual void SetAngularVelocity(const math::vector3d& v)=0;
	virtual math::vector3d GetAngularVelocity()=0;

	virtual void SetMaxAngularVelocity(float m)=0;
	virtual float GetMaxAngularVelocity()const=0;

	virtual void SetMaxSpeed(float s)=0;
	virtual float GetMaxSpeed()const=0;

	virtual math::vector3d GetUp()
	{
		return GetGlobalOrintation()*math::vector3d::YAxis;
	}
	virtual math::vector3d GetHeading()
	{
		return GetGlobalOrintation()*math::vector3d::ZAxis;
	}
};

}
}


#endif //___IAIPhysics___
