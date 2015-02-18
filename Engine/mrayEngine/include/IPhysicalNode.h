
/********************************************************************
	created:	2009/01/10
	created:	10:1:2009   23:44
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicalNode.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicalNode
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicalNode___
#define ___IPhysicalNode___

#include "GCPtr.h"

#include "IObject.h"
#include "TypedProperty.h"

#include "IPhysical3DDef.h"

namespace mray{
namespace scene{
	class IMovable;
}
namespace physics{
	class IPhysicUserContactReport;

	class IPhysicalShape;
	class IPhysicManager;


class MRAY_DLL IPhysicalNode:public IObject
{
	DECLARE_RTTI;
protected:
	IObject* m_userData;
	IPhysicUserContactReport* m_report;
	IPhysicManager* m_creator;
	void fillProperties();
public:
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(Mass,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(LinearDamping,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(GlobalPosition,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(GlobalOrintation,math::quaternion,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(LinearVelocity,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AngularVelocity,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(MaxAngularVelocity,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(LinearMomentum,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(AngularMomentum,math::vector3d,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SleepLinearVelocity,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SleepAngularVelocity,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SleepEnergyThreshold,float,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(SolverIterationCount,uint,MRAY_DLL);
	DECLARE_PROPERTY_TYPE(ContactReportThreshold,float,MRAY_DLL);

public:

	IPhysicalNode(IPhysicManager*creator):m_userData(0),m_report(0),m_creator(creator)
	{
		fillProperties();
	}
	virtual~IPhysicalNode(){}

// 	virtual void attachNode(GCPtr<scene::IMovable> node)=0;
// 	virtual GCPtr<scene::IMovable> getAttachedNode()=0;

	IPhysicManager* GetCreator()const{return m_creator;}

	virtual bool SetName(const core::string& name)=0;
	virtual const core::string&  GetName()=0;

	virtual int GetShapesCount()=0;
	virtual IPhysicalShape* GetShape(int i)=0;
	virtual IPhysicalShape* GetShapeByName(const core::string& name)=0;
	virtual IPhysicalShape* CreateShape(IPhysicalShapeDesc*desc)=0;

	virtual int getNumberOfShapes()=0;

	virtual void SetBodyFlag(EBodyFlag f,bool enable)=0;
	virtual bool GetBodyFlag(EBodyFlag f)=0;

	virtual void setMaterialIndex(int matIndex,int shape=0)=0;
	virtual int getMaterialIndex(int shape=0)=0;

	virtual bool	setMass(float m)=0;
	virtual float	getMass()const=0;

	virtual bool setLinearDamping(float d)=0;
	virtual float getLinearDamping()const=0;


	virtual bool setAngularDamping(float d)=0;
	virtual float getAngularDamping()const=0;

	virtual bool isDynamic()const=0;

	virtual void moveGlobalPosition(const math::vector3d&pos)=0;
	virtual void moveGlobalOrientation(const math::quaternion&ori)=0;

	virtual bool setGlobalPosition(const math::vector3d&pos)=0;
	virtual math::vector3d getGlobalPosition()const=0;

	virtual bool setGlobalOrintation(const math::quaternion&or)=0;
	virtual math::quaternion getGlobalOrintation()const=0;

	virtual bool setLinearVelocity(const math::vector3d& v)=0;
	virtual bool setAngularVelocity(const math::vector3d& v)=0;

	virtual math::vector3d getLinearVelocity()const=0;
	virtual math::vector3d getAngularVelocity()const=0;

	virtual bool setMaxAngularVelocity(float m)=0;
	virtual float getMaxAngularVelocity()const=0;

	virtual bool setLinearMomentum(const math::vector3d& v)=0;
	virtual math::vector3d getLinearMomentum()const=0;

	virtual bool setAngularMomentum(const math::vector3d& v)=0;
	virtual math::vector3d getAngularMomentum()const=0;

	virtual void addForce(const math::vector3d& force, EForceMode mode = EFM_Force, bool wakeup = true) = 0;
	virtual void addLocalForce(const  math::vector3d& force, EForceMode mode = EFM_Force, bool wakeup = true) = 0;

	virtual void addForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) = 0;
	virtual void addForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) = 0;
	virtual void addLocalForceAtPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) = 0;
	virtual void addLocalForceAtLocalPos(const math::vector3d& force, const math::vector3d& pos, EForceMode mode = EFM_Force, bool wakeup = true) = 0;


	virtual void addTorque(const math::vector3d& torque, EForceMode mode = EFM_Force, bool wakeup = true) = 0;
	virtual void addLocalTorque(const math::vector3d& torque, EForceMode mode = EFM_Force, bool wakeup = true) = 0;

	virtual	float computeKineticEnergy() const = 0;

	virtual math::vector3d getPointVelocity(const math::vector3d& point) const = 0;
	virtual math::vector3d getLocalPointVelocity(const math::vector3d& point) const = 0;

	virtual bool isSleeping() const = 0;
	virtual bool isGroupSleeping() const = 0;

	virtual bool setSleepLinearVelocity(float threshold) = 0;
	virtual float getSleepLinearVelocity() const = 0;


	virtual bool setSleepAngularVelocity(float threshold) = 0;
	virtual float getSleepAngularVelocity() const = 0;

	virtual bool setSleepEnergyThreshold(float threshold) = 0;
	virtual float getSleepEnergyThreshold() const = 0;

	virtual void wakeUp(float wakeCounterValue)	= 0;
	virtual void goToSleep()	= 0;

	virtual bool SetSolverIterationCount(uint c)=0;
	virtual uint GetSolverIterationCount()const=0;

	virtual bool SetContactReportThreshold(float th)=0;
	virtual float GetContactReportThreshold()const=0;

	virtual void EnableContactReportFlag(EContactReportFlag flag)=0;
	virtual void DisableContactReportFlag(EContactReportFlag flag)=0;

	void SetUserData(IObject*d){m_userData=d;}
	IObject* GetUserData(){return m_userData;}


	virtual void SetCMassOffsetLocalMat(const math::matrix4x4& m)=0;
	virtual void SetCMassOffsetLocalPosition(const math::vector3d&pos)=0;
	virtual void SetCMassOffsetLocalOrientation(const math::quaternion&or)=0;

	virtual void SetCMassOffsetGlobalMat(const math::matrix4x4& m)=0;
	virtual void SetCMassOffsetGlobalPosition(const math::vector3d&pos)=0;
	virtual void SetCMassOffsetGlobalOrientation(const math::quaternion&or)=0;

	virtual void SetCMassGlobalMat(const math::matrix4x4& m)=0;
	virtual void SetCMassGlobalPosition(const math::vector3d&pos)=0;
	virtual void SetCMassGlobalOrientation(const math::quaternion&or)=0;

	virtual void GetCMassLocalMat(math::matrix4x4& m)const=0;
	virtual void GetCMassLocalPosition(math::vector3d& p)const=0;
	virtual void GetCMassLocalOrientation(math::quaternion&or)const=0;

	virtual void GetCMassGlobalMat(math::matrix4x4& m)const=0;
	virtual void GetCMassGlobalPosition(math::vector3d& p)const=0;
	virtual void GetCMassGlobalOrientation(math::quaternion&or)const=0;

	void SetContactReport(IPhysicUserContactReport*r){m_report=r;}
	IPhysicUserContactReport* GetContactReport(){return m_report;}

	virtual void SaveToDesc(PhysicalNodeDesc* desc)=0;
	virtual bool SaveToDesc(PhysicalBodyDesc* desc)=0;
};

MakeSharedPtrType(IPhysicalNode);

}
}


#endif //___IPhysicalNode___

