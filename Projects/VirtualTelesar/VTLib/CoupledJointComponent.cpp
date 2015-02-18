


#include "stdafx.h"
#include "CoupledJointComponent.h"
#include "PhysicalJointDOF6Component.h"


namespace mray
{
namespace VT
{

	IMPLEMENT_RTTI(CoupledJointInfo,IObject);


	IMPLEMENT_PROPERTY_TYPE_HEADER(JointName,CoupledJointInfo,core::string,mT("TargetJoint"),EPBT_Basic,mT(""),mT(""));
	IMPLEMENT_PROPERTY_TYPE_GENERIC(JointName,CoupledJointInfo,core::string,SetJointName,GetJointName,,,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Ratio,CoupledJointInfo,float,mT("Ratio"),EPBT_Basic,mT(""),1);
	IMPLEMENT_PROPERTY_TYPE_GENERIC(Ratio,CoupledJointInfo,float,SetRatio,GetRatio,core::StringConverter::toString,core::StringConverter::toFloat,false);

	IMPLEMENT_PROPERTY_TYPE_HEADER(Axis,CoupledJointInfo,ETargetControlAxis,mT("Axis"),EPBT_Basic,mT(""),Axis_Twist);

	CoupledJointInfo::PropertyTypeAxis CoupledJointInfo::PropertyTypeAxis::instance;
	ETargetControlAxis CoupledJointInfo::PropertyTypeAxis::GetValue(CPropertieSet*object)const
	{
		CoupledJointInfo* o=dynamic_cast<CoupledJointInfo*>(object);
		if(!o)
			return m_default;
		return o->GetJointAxis();
	}
	bool CoupledJointInfo::PropertyTypeAxis::SetValue(CPropertieSet*object,const ETargetControlAxis& v)
	{
		CoupledJointInfo* o=dynamic_cast<CoupledJointInfo*>(object);
		if(!o)return false;
		return o->SetJointAxis(v);
	}
	core::string CoupledJointInfo::PropertyTypeAxis::toString(CPropertieSet*object)const
	{
		ETargetControlAxis a=GetValue(object);
		switch (a)
		{
		case Axis_Twist:return mT("T");
		case Axis_Swing1:return mT("S1");
		case Axis_Swing2:return mT("S2");
		case Axis_XTranslate:return mT("X");
		case Axis_YTranslate:return mT("Y");
		case Axis_ZTranslate:return mT("Z");
		}
		return mT("");
	}
	bool CoupledJointInfo::PropertyTypeAxis::parse(CPropertieSet*reciver,const core::string&str)
	{
		ETargetControlAxis v;
		if(str==mT("T"))
			v=Axis_Twist;
		else if(str==mT("S1"))
			v=Axis_Swing1;
		else if(str==mT("S2"))
			v=Axis_Swing2;
		else if(str==mT("X"))
			v=Axis_XTranslate;
		else if(str==mT("Y"))
			v=Axis_YTranslate;
		else if(str==mT("Z"))
			v=Axis_ZTranslate;
		else 
			return false;
		return SetValue(reciver,v);
	}
	bool CoupledJointInfo::PropertyTypeAxis::isDefault(CPropertieSet*reciver)const
	{
		if(GetValue(reciver)==GetDefaultValue())
			return true;
		return false;
	}
CoupledJointInfo::CoupledJointInfo()//:m_AnglePID(20000,10,10)
{
	m_ratio=1;
	m_joint=0;
	m_jointAxis=Axis_Twist;
//	m_value=0;

	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeJointName::instance);
		dic->addPropertie(&PropertyTypeAxis::instance);
		dic->addPropertie(&PropertyTypeRatio::instance);
	}

}
CoupledJointInfo::~CoupledJointInfo()
{
}


void CoupledJointInfo::RetriveJoint(CoupledJointComponent* owner)
{
	m_joint=owner->RetriveComponent<game::IPhysicalJointComponent>(owner->GetOwnerComponent(),m_jointName);
	if(!m_joint)
	{
		gLogManager.log("CoupledJointInfo::RetriveJoint() - Warning Joint named:\""+m_jointName+"\" couldn't be found in \""+owner->GetName()+"\"",ELL_WARNING);
	}
}

void CoupledJointInfo::SetJoint(game::IPhysicalJointComponent* j)
{
	m_joint=j;
	if(m_joint)
	{
	}
}


float CoupledJointInfo::SetValue(const ControlInputValues& val,EControlSource src)
{
	float v;
	if(m_value.KinValue==val.KinValue*m_ratio && m_value.RealtimeValue==val.RealtimeValue*m_ratio)
		return (src==EControl_Kinematic)? m_value.KinValue:m_value.RealtimeValue;
	m_value=val;
	m_value.KinValue*=m_ratio;
	m_value.RealtimeValue*=m_ratio;
	if(src==EControl_Kinematic)
		v=m_value.KinValue;
	else v=m_value.RealtimeValue;
	//v*=m_ratio;

	switch (m_jointAxis)
	{
	case Axis_Twist:
		m_joint->SetTwistDrive(v);
		break;
	case Axis_Swing1:
		m_joint->SetSwing1Drive(v);
		break;
	case Axis_Swing2:
		m_joint->SetSwing2Drive(v);
		break;
	case Axis_XTranslate:
		m_joint->SetXDrive(v);
		break;
	case Axis_YTranslate:
		m_joint->SetYDrive(v);
		break;
	case Axis_ZTranslate:
		m_joint->SetZDrive(v);
		break;
	}
	return v;
}


void CoupledJointInfo::Update(float dt)
{
	return;
	/*
	if(m_joint && m_joint->GetJoint())
	{
		//physics::IDOF6Joint3D* j= m_joint->GetDOF6Joint();
		float force=m_AnglePID.Update(dt,GetCurrentValue(),m_value)*dt;
		force=math::clamp<float>(force,-1,1);
		//force*=0;
		//force=math::clamp<float>(force,j->getfor);
		switch (m_jointAxis)
		{
			case Axis_Twist:
				j->setDriveAngularVelocity(math::vector3d(force,0,0));
				break;
			case Axis_Swing1:
				j->setDriveAngularVelocity(math::vector3d(0,force,0));
				break;
			case Axis_Swing2:
				j->setDriveAngularVelocity(math::vector3d(0,0,force));
				break;
			case Axis_XTranslate:
				break;
			case Axis_YTranslate:
				break;
			case Axis_ZTranslate:
				break;
		}
	}*/
}

float CoupledJointInfo::GetCurrentValue()
{
	float v=0;
	if(m_joint)
	{
		if(m_jointAxis&Axis_Revolute)
		{

			switch (m_jointAxis)
			{
			case Axis_Twist:
				v=m_joint->GetCurrentAngle(0);
				break;
			case Axis_Swing1:
				v=m_joint->GetCurrentAngle(1);
				break;
			case Axis_Swing2:
				v=m_joint->GetCurrentAngle(2);
				break;
			}
		}
		else 
		if(m_jointAxis&Axis_Prismatic)
		{
			switch (m_jointAxis)
			{
			case Axis_XTranslate:
				v=m_joint->GetCurrentLinearDistance(0);
				break;
			case Axis_YTranslate:
				v=m_joint->GetCurrentLinearDistance(1);
				break;
			case Axis_ZTranslate:
				v=m_joint->GetCurrentLinearDistance(2);
				break;
			}
		}
	}
	return v;
}

CoupledJointComponent::CoupledJointComponent(game::GameEntityManager*mngr)
{
}
CoupledJointComponent::CoupledJointComponent()
{
}
CoupledJointComponent::~CoupledJointComponent()
{
	for (int i=0;i<m_attachedJoints.size();++i)
	{
		delete m_attachedJoints[i];
	}
	m_attachedJoints.clear();
}




bool CoupledJointComponent::InitComponent()
{
	if(!IVTComponent::InitComponent())
		return false;
	for(int i=0;i<m_attachedJoints.size();++i)
	{
		((CoupledJointInfo*)m_attachedJoints[i])->RetriveJoint(this);
	}
	return true;
}

bool CoupledJointComponent::SetEnabled(bool enabled)
{
	return true;
}

bool CoupledJointComponent::SetControlValue(const std::vector<ControlInputValues>&  v,EControlSource src)
{
	if(v.size()<0)
		return false;
	IControllableComponent::SetControlValue(v,src);
	//m_targetValue=v;
	//m_fedValues.resize(m_attachedJoints.size());
	for(int i=0;i<m_attachedJoints.size();++i)
	{
		m_attachedJoints[i]->SetValue(v[0],src);

// 		m_fedValues[i].KinValue		=v[0]*m_attachedJoints[i]->GetRatio();
// 		m_fedValues[i].RealtimeValue=v[1]*m_attachedJoints[i]->GetRatio();
	}
	return true;
}
bool CoupledJointComponent::SetControlValue(const ControlInputValues&  v, EControlSource src)
{
	if (m_attachedJoints.size() == 0)
		return false;

	m_attachedJoints[0]->SetValue(v, src);

	return true;

}

int CoupledJointComponent::GetJoints(std::vector<game::IPhysicalJointComponent*>& joints)
{
	for(int i=0;i<m_attachedJoints.size();++i)
		joints.push_back(((CoupledJointInfo*)m_attachedJoints[i])->GetJoint());
	return m_attachedJoints.size();
}

void CoupledJointComponent::Update(float dt)
{
	IVTComponent::Update(dt);

	for(int i=0;i<m_attachedJoints.size();++i)
		m_attachedJoints[i]->Update(dt);
}

xml::XMLElement* CoupledJointComponent::loadXMLSettings(xml::XMLElement* elem)
{
	xml::XMLElement* e=IVTComponent::loadXMLSettings(elem);
	xml::XMLElement* je=e->getSubElement(mT("Joint"));
	while(je)
	{
		CoupledJointInfo* ifo=new CoupledJointInfo();
		ifo->loadFromXML(je);
		m_attachedJoints.push_back(ifo);
		je=je->nextSiblingElement(mT("Joint"));
	}
	return e;
}
std::vector<float> CoupledJointComponent::GetCurrentValue()
{
	std::vector<float>  vec;
	for(int i=0;i<m_attachedJoints.size();++i)
	{
		vec.push_back(m_attachedJoints[i]->GetCurrentValue());
	}
	return vec;
}

}
}

