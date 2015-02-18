
/********************************************************************
	created:	2012/07/14
	created:	14:7:2012   17:06
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\CoupledJointComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar
	file base:	CoupledJointComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___CoupledJointComponent___
#define ___CoupledJointComponent___

//#include "IRobotJointComponent.h"
//#include "PIDController.h"
#include "IVTComponent.h"
#include "IControllableComponent.h"

namespace mray
{
	namespace game
	{
		class IPhysicalJointComponent;
	}
namespace VT
{
	class CoupledJointComponent;

	enum ETargetControlAxis
	{
		Axis_Twist=BIT(0),
		Axis_Swing1=BIT(1),
		Axis_Swing2=BIT(2),
		Axis_XTranslate=BIT(3),
		Axis_YTranslate=BIT(4),
		Axis_ZTranslate=BIT(5),

		Axis_Revolute=Axis_Twist|Axis_Swing1|Axis_Swing2,
		Axis_Prismatic=Axis_XTranslate|Axis_YTranslate|Axis_ZTranslate
	};

	class CoupledJointInfo:public IObject
	{
		DECLARE_RTTI;
	protected:
		core::string m_jointName;
		ControlInputValues m_value;
		game::IPhysicalJointComponent* m_joint;
		float m_ratio;
		ETargetControlAxis m_jointAxis;

		//PIDControllerF m_AnglePID;
	public:
		DECLARE_PROPERTY_TYPE(JointName,core::string,);
		DECLARE_PROPERTY_TYPE(Ratio,float,);
		DECLARE_PROPERTY_TYPE(Axis,ETargetControlAxis,);
	public:
		CoupledJointInfo();
		virtual~CoupledJointInfo();

		void RetriveJoint(CoupledJointComponent* owner);

		virtual bool SetJointName(const core::string&n){m_jointName=n;return true;}
		const core::string& GetJointName(){return m_jointName;}

		void SetJoint(game::IPhysicalJointComponent* j);
		game::IPhysicalJointComponent* GetJoint(){return m_joint;}

		float SetValue(const ControlInputValues& v,EControlSource src);
		const ControlInputValues& GetValue(){return m_value;}

		float GetCurrentValue();

		bool SetRatio(float v){m_ratio=v;return true;}
		float GetRatio(){return m_ratio;}

		bool SetJointAxis(ETargetControlAxis v){m_jointAxis=v;return true;}
		ETargetControlAxis GetJointAxis(){return m_jointAxis;}

		void Update(float dt);

	};

class CoupledJointComponent:public IVTComponent, public IControllableComponent
{
	DECLARE_RTTI;
protected:
public:
	std::vector<CoupledJointInfo*> m_attachedJoints;
	//std::vector<float> m_targetValue;
	//std::vector<ControlInputValues> m_fedValues;

public:
	CoupledJointComponent(game::GameEntityManager*mngr);
	CoupledJointComponent();
	virtual~CoupledJointComponent();

	const core::string& GetControllableName(){return GetName();}

	const std::vector<CoupledJointInfo*>& GetAttachedJoints(){return  m_attachedJoints;}

	virtual bool InitComponent();

	virtual bool SetEnabled(bool enabled);


	virtual bool SetControlValue(const std::vector<ControlInputValues>&  v, EControlSource src);
	virtual bool SetControlValue(const ControlInputValues&  v, EControlSource src);
	//virtual const std::vector<ControlInputValues>&  GetControlValue(){return m_fedValues;}

	virtual int GetJoints(std::vector<game::IPhysicalJointComponent*>& joints);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

	virtual void Update(float dt);

	virtual std::vector<float> GetCurrentValue();
};
DECLARE_GAMECOMPONENT_FACTORY(CoupledJointComponent);

}
}

#endif
