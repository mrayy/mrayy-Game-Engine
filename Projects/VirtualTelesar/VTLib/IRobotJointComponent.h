
#if 0

/********************************************************************
	created:	2013/04/15
	created:	15:4:2013   17:25
	filename: 	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IRobotJointComponent.h
	file path:	C:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IRobotJointComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __IRobotJointComponent__
#define __IRobotJointComponent__


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

class IRobotJointComponent;
/*

class ICoupledJointInfo:public IObject
{
	DECLARE_RTTI;
protected:
	core::string m_jointName;
public:
	DECLARE_PROPERTY_TYPE(JointName,core::string,);
	DECLARE_PROPERTY_TYPE(Ratio,float,);
	DECLARE_PROPERTY_TYPE(Axis,ETargetControlAxis,);
public:

	ICoupledJointInfo();
	virtual~ICoupledJointInfo(){}

	virtual bool SetJointName(const core::string&n){m_jointName=n;return true;}
	const core::string& GetJointName(){return m_jointName;}

	virtual float SetValue(float v)=0;
	virtual float GetValue()=0;

	virtual float GetCurrentValue()=0;

	virtual bool SetRatio(float v)=0;
	virtual float GetRatio()=0;

	virtual bool SetJointAxis(ETargetControlAxis v)=0;
	virtual ETargetControlAxis GetJointAxis()=0;

	virtual void Update(float dt)=0;

};*/


class ICoupledJointInfo
{
	DECLARE_RTTI
protected:
	float m_value;
	game::IPhysicalJointComponent* m_joint;
	float m_ratio;
	ETargetControlAxis m_jointAxis;

//	PIDControllerF m_AnglePID;
public:
	ICoupledJointInfo();
	virtual~ICoupledJointInfo();


	void SetJoint(game::IPhysicalJointComponent* j);
	game::IPhysicalJointComponent* GetJoint(){return m_joint;}

	float SetValue(float v);
	float GetValue(){return m_value;}

	float GetCurrentValue();

	bool SetRatio(float v){m_ratio=v;return true;}
	float GetRatio(){return m_ratio;}

	bool SetJointAxis(ETargetControlAxis v){m_jointAxis=v;return true;}
	ETargetControlAxis GetJointAxis(){return m_jointAxis;}

	void Update(float dt);

};
class IRobotJointComponent:public IVTComponent, public IControllableComponent
{
protected:
public:

	IRobotJointComponent(){}
	virtual~IRobotJointComponent(){}

	const core::string& GetControllableName(){return GetName();}


	virtual const std::vector<ICoupledJointInfo*>& GetAttachedJoints()=0;

	virtual bool InitComponent()=0;

	virtual bool SetEnabled(bool enabled)=0;


	virtual bool SetControlValue(const std::vector<float>&  v)=0;
	virtual const std::vector<InputValues>&  GetControlValue()=0;

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem)=0;

	virtual void Update(float dt)=0;

	virtual std::vector<float> GetCurrentValue()=0;
};

}
}


#endif
#endif