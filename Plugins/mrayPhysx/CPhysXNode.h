

#ifndef ___CPhysXNode___
#define ___CPhysXNode___

#include <IPhysicalNode.h>
#include "compileConfig.h"

#include <vector>

class NxActor;
class NxShape;

namespace mray
{
namespace physics
{
	class PhysXShape;

class MRAY_PHYSX_DLL CPhysXNode:public IPhysicalNode
{
protected:
	NxActor* m_actor;
	core::string m_name;

	std::vector<IPhysicalShape*> m_shapes;
	PhysXShape* _AddShape(NxShape*s);
public:

	CPhysXNode(NxActor* actor,IPhysicManager*creator);
	virtual~CPhysXNode();

	void setActor(NxActor*d);
	NxActor* getActor(){return m_actor;}


	const core::string& GetName(){return m_name;}
	bool SetName(const core::string&n); 

	int GetShapesCount(){return m_shapes.size();}
	IPhysicalShape* GetShape(int i);
	IPhysicalShape* GetShapeByName(const core::string& name);
	IPhysicalShape* CreateShape(IPhysicalShapeDesc*desc);

	void moveGlobalPosition(const math::vector3d&pos);
	void moveGlobalOrientation(const math::quaternion&ori);

	bool setGlobalPosition(const math::vector3d&pos);
	math::vector3d getGlobalPosition()const;

	bool setGlobalOrintation(const math::quaternion&or);
	math::quaternion getGlobalOrintation()const;


	int getNumberOfShapes();

	void SetBodyFlag(EBodyFlag f,bool enable);
	bool GetBodyFlag(EBodyFlag f);

	void setMaterialIndex(int matIndex,int shape=0);
	int getMaterialIndex(int shape=0);

	bool	setMass(float m);
	float	getMass()const;

	bool setLinearDamping(float d);
	float getLinearDamping()const;


	bool setAngularDamping(float d);
	float getAngularDamping()const;

	bool isDynamic()const;

	bool setLinearVelocity(const math::vector3d& v);
	bool setAngularVelocity(const math::vector3d& v);

	math::vector3d getLinearVelocity()const;
	math::vector3d getAngularVelocity()const;

	bool setMaxAngularVelocity(float m);
	float getMaxAngularVelocity()const;

	bool setLinearMomentum(const math::vector3d& v);
	math::vector3d getLinearMomentum()const;

	bool setAngularMomentum(const math::vector3d& v);
	math::vector3d getAngularMomentum()const;

	void addForce(const math::vector3d& force, EForceMode mode = EFM_Force, bool wakeup = true) ;
	void addLocalForce(const  math::vector3d& force, EForceMode mode = EFM_Force, bool wakeup = true) ;

	void addForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) ;
	void addForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) ;
	void addLocalForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) ;
	void addLocalForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) ;


	void addTorque(const math::vector3d& torque, EForceMode mode = EFM_Force, bool wakeup = true) ;
	void addLocalTorque(const math::vector3d& torque, EForceMode mode = EFM_Force, bool wakeup = true) ;

	virtual	float computeKineticEnergy() const ;


	math::vector3d getPointVelocity(const math::vector3d& point) const ;
	math::vector3d getLocalPointVelocity(const math::vector3d& point) const ;


	bool isSleeping() const ;
	bool isGroupSleeping() const ;

	bool setSleepLinearVelocity(float threshold) ;
	float getSleepLinearVelocity() const ;


	bool setSleepAngularVelocity(float threshold) ;
	float getSleepAngularVelocity() const ;

	bool setSleepEnergyThreshold(float threshold) ;
	float getSleepEnergyThreshold() const ;

	void wakeUp(float wakeCounterValue)	;
	void goToSleep();


	bool SetSolverIterationCount(uint c);
	uint GetSolverIterationCount()const;

	bool SetContactReportThreshold(float th);
	float GetContactReportThreshold()const;

	void SetCMassOffsetLocalMat(const math::matrix4x4& m);
	void SetCMassOffsetLocalPosition(const math::vector3d&pos);
	void SetCMassOffsetLocalOrientation(const math::quaternion&or);

	void SetCMassOffsetGlobalMat(const math::matrix4x4& m);
	void SetCMassOffsetGlobalPosition(const math::vector3d&pos);
	void SetCMassOffsetGlobalOrientation(const math::quaternion&or);

	void SetCMassGlobalMat(const math::matrix4x4& m);
	void SetCMassGlobalPosition(const math::vector3d&pos);
	void SetCMassGlobalOrientation(const math::quaternion&or);

	void GetCMassLocalMat(math::matrix4x4& m)const;
	void GetCMassLocalPosition(math::vector3d& p)const;
	void GetCMassLocalOrientation( math::quaternion&or)const;

	void GetCMassGlobalMat(math::matrix4x4& m)const;
	void GetCMassGlobalPosition(math::vector3d& p)const;
	void GetCMassGlobalOrientation( math::quaternion&or)const;


	void EnableContactReportFlag(EContactReportFlag flag);
	void DisableContactReportFlag(EContactReportFlag flag);

	void SaveToDesc(PhysicalNodeDesc* desc);
	bool SaveToDesc(PhysicalBodyDesc* desc);
};

}
}



#endif
