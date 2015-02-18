
#include "stdafx.h"
#include "SpringTouchSensorComponent.h"
#include "PhysicsComponent.h"
#include "PhysicContactPair.h"
#include "IPhysicContactStreamIterator.h"
#include "IPhysicUserContactReport.h"
#include "GameEntityManager.h"
#include "IDebugDrawManager.h"
#include "DebugRenderSettings.h"
#include "IPhysicalShape.h"


#include "SceneComponent.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "GGDriver.h"

#include "IVTDebugInterface.h"
#include "SensorCurve.h"
#include "XMLTree.h"
#include "IGUIPanelElement.h"

#include "SpringTouchSensorImpl.h"
#include "GGComponent.h"

namespace mray
{
namespace VT
{

	class SpringTouchSensorDebugObj:public IVTDebugObject
	{
	protected:
		SpringTouchSensorComponent* m_owner;
	public:
		SpringTouchSensorDebugObj(SpringTouchSensorComponent* c)
		{
			m_owner=c;
		}

		virtual core::string GetName()
		{
			return m_owner->GetName();
		}
		virtual core::string GetDebugString()
		{
			return m_owner->GetDebugString();
		}
	};

class SpringTouchSenseorContactReporter:public physics::IPhysicUserContactReport
{
protected:
	SpringTouchSensorComponent* m_owner;
public:
	SpringTouchSenseorContactReporter(SpringTouchSensorComponent* o)
	{
		m_owner=o;
	}

	physics::IPhysicalNode* GetOtherNode(physics::PhysicContactPair* pair)
	{
		game::PhysicsComponent* c=m_owner->GetTargetNode();
		if(!c)
			return 0;
		if (pair->nodes[0]==c->GetPhysicalNode())
			return pair->nodes[1];
		else if( pair->nodes[1]==c->GetPhysicalNode())
			return pair->nodes[0];
		return 0;
	}
	void OnPairStartTouchForceThreshold(physics::PhysicContactPair* pair)
	{
		physics::IPhysicalNode* n=GetOtherNode(pair);
		if(!n)
			return;
		m_owner->_OnStartTouch(pair,n);
	}
	void OnPairStartTouch(physics::PhysicContactPair* pair)
	{
		physics::IPhysicalNode* n=GetOtherNode(pair);
		if(!n)
			return;
		m_owner->_OnStartTouch(pair,n);
	}
	void OnPairTouchForceThreshold(physics::PhysicContactPair* pair)
	{
		physics::IPhysicalNode* n=GetOtherNode(pair);
		if(!n)
			return;
		m_owner->_OnTouch(pair,n);
	}
	void OnPairSlide(physics::PhysicContactPair* pair)
	{
		physics::IPhysicalNode* n=GetOtherNode(pair);
		if(!n)
			return;
		m_owner->_OnSlide(pair,n);
	}
	void OnPairEndTouchForceThreshold(physics::PhysicContactPair* pair)
	{
		physics::IPhysicalNode* n=GetOtherNode(pair);
		if(!n)
			return;
		m_owner->_OnEndTouch(pair,n);
	}
};
IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode,SpringTouchSensorComponent,core::string,mT("TargetNode"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode,SpringTouchSensorComponent,core::string,SetTargetNodeName,GetTargetNodeName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetShape,SpringTouchSensorComponent,core::string,mT("TargetShape"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetShape,SpringTouchSensorComponent,core::string,SetTargetShapeName,GetTargetShapeName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(ModulatorPath,SpringTouchSensorComponent,core::string,mT("ModulatorPath"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(ModulatorPath,SpringTouchSensorComponent,core::string,SetModulatorPath,GetModulatorPath,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetGG,SpringTouchSensorComponent,core::string,mT("TargetGG"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetGG,SpringTouchSensorComponent,core::string,SetTargetGG,GetTargetGG,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(Channel,SpringTouchSensorComponent,int,mT("Channel"),EPBT_Basic,mT(""),0);
IMPLEMENT_PROPERTY_TYPE_GENERIC(Channel,SpringTouchSensorComponent,int,SetChannel,GetChannel,core::StringConverter::toString,core::StringConverter::toInt,false);



SpringTouchSensorComponent::SpringTouchSensorComponent(game::GameEntityManager* m)
{
	m_data=new SpringTouchSensorComponentImpl();
	m_data->mngr=m;
	m_data->shape=0;
	m_data->modulator=0;
	m_data->targetNode=0;
	m_data->reporter=new SpringTouchSenseorContactReporter(this);
	m_data->guiElement=0;
	if(DebugRenderSettings::DebugInterface)
	{
		m_data->debugObj=new SpringTouchSensorDebugObj(this);
		DebugRenderSettings::DebugInterface->AddDebugObject(m_data->debugObj);
	}
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeTargetShape::instance);
		dic->addPropertie(&PropertyTypeModulatorPath::instance);
		dic->addPropertie(&PropertyTypeTargetGG::instance);
		//dic->addPropertie(&PropertyTypeIP::instance);
		//dic->addPropertie(&PropertyTypePort::instance);
		dic->addPropertie(&PropertyTypeChannel::instance);
	}
}
SpringTouchSensorComponent::~SpringTouchSensorComponent()
{
	if(DebugRenderSettings::DebugInterface)
	{
		DebugRenderSettings::DebugInterface->RemoveDebugObject(m_data->debugObj);
	}
	if(DebugRenderSettings::guiManager)
	{
		DebugRenderSettings::CreatedElements.erase(GetName());
		GUI::IGUIPanelElement* p= m_data->guiElement->GetParent();
		if(p)
			p->RemoveElement(m_data->guiElement);
	}
	delete m_data->debugObj;
	delete m_data->modulator;
	
	delete m_data;
}

void SpringTouchSensorComponent::_ChangeStatus(physics::IPhysicalNode* node,bool on)
{

	game::PhysicsComponent* comp=((game::PhysicsComponent*) node->GetUserData());
	const std::list<game::PhysicalComponentAttachment*>& nodes=comp->GetAttachedNodes();
	std::list<game::PhysicalComponentAttachment*>::const_iterator it=nodes.begin();
	for (;it!=nodes.end();++it)
	{
		game::PhysicalComponentAttachment* c=*it;
		c->GetTargetNode()->GetSceneNode()->GetAttachedNode(0)->getMaterial(0)->GetActiveTechnique()->GetPassAt(0)->SetAmbient(on?video::SColor(1,0,0,1):video::SColor(1,1,1,1));
	}

}
bool SpringTouchSensorComponentImpl::AddPoint(const SpringTouchSensorComponentImpl::SpringContactPoint& pt)
{
	const float thresholdSqr=0.01f*0.01f;
	const float minForce=0.01;
	if(pt.force<minForce)
		return false;
	int lowID=-1;
	float lowForce=9999999;
	int found=-1;
	for (int i=0;i<contactPoints.size();++i)
	{
		if(contactPoints[i].erased )
		{
			if(found==-1)
				found=i;	
			continue;;
		}
		if(contactPoints[i].position.getDistSQ(pt.position)<thresholdSqr )
		{
			contactPoints[i].force=pt.force;
			contactPoints[i].time=0;
			return false;
		}
		if(contactPoints[i].force<lowForce)
		{
			lowForce=contactPoints[i].force;
			lowID=i;
		}
	} 
	SpringContactPoint toAdd=pt;
	toAdd.erased=false;
	if(found!=-1)
	{
		contactPoints[found]=toAdd;
	}
	else
	{
		if(contactPoints.size()>=10 )
		{
			if(lowID!=-1)
				contactPoints[lowID]=toAdd;
		}else
			contactPoints.push_back(toAdd);
	}
	return true;
}

bool SpringTouchSensorComponent::InitComponent()
{
	if(!IVTComponent::InitComponent())
		return false;
	m_data->shape=0;
	m_data->targetNode=RetriveComponent<game::PhysicsComponent>(GetOwnerComponent(),m_data->targetNodeName);
	if(m_data->targetNode)
	{
		m_data->targetNode->InitComponent();
		if(m_data->targetNode->GetPhysicalNode())
		{
			m_data->shape= m_data->targetNode->GetPhysicalNode()->GetShapeByName(m_data->targetShapeName);
			
			m_data->targetNode->GetPhysicalNode()->SetContactReport(m_data->reporter);
				m_data->targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_StartTouch);
			m_data->targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_StartTouchForceThreshold);
			m_data->targetNode->GetPhysicalNode()->SetContactReportThreshold(0);
			//	m_data->targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_EndTouch);
			//	m_data->targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_Slide);
			//	m_data->targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_NotifyForces);
		}
	}

	m_data->ggComponent=RetriveComponent<GGComponent>(GetOwnerComponent(),m_data->targetGG);
	
	//load modulator
	xml::XMLTree tree;
	if(tree.load(m_data->modulatorPath))
	{
		m_data->modulator=new SensorCurve();
		m_data->modulator->LoadFromXML(tree.getSubElement("Modulator"));
	}
	if(m_data->modulator && DebugRenderSettings::guiManager)
	{
		m_data->guiElement=new GUI::GUISpringTouchElement(DebugRenderSettings::guiManager);
		m_data->guiElement->SetElement(m_data);
		DebugRenderSettings::CreatedElements[GetName()]=m_data->guiElement;
		if(DebugRenderSettings::rootElement)
			DebugRenderSettings::rootElement->AddElement(m_data->guiElement);
		m_data->guiElement->SetSize(200);
	}
	return true;
}

bool SpringTouchSensorComponent::SetTargetNodeName(const core::string& name)
{
	m_data->targetNodeName=name;
	return true;
}
const core::string& SpringTouchSensorComponent::GetTargetNodeName()
{
	return m_data->targetNodeName;
}
bool SpringTouchSensorComponent::SetTargetShapeName(const core::string& name)
{
	m_data->targetShapeName=name;
	return true;
}
const core::string& SpringTouchSensorComponent::GetTargetShapeName()
{
	return m_data->targetShapeName;
}

bool SpringTouchSensorComponent::SetTargetGG(const core::string& name)
{
	m_data->targetGG=name;
	return true;
}
const core::string& SpringTouchSensorComponent::GetTargetGG()
{
	return m_data->targetGG;
}
bool SpringTouchSensorComponent::SetChannel(int p)
{
	m_data->channel=p;
	return true;
}
int SpringTouchSensorComponent::GetChannel()
{
	return m_data->channel;
}

game::PhysicsComponent* SpringTouchSensorComponent::GetTargetNode()
{
	return m_data->targetNode;
}

bool SpringTouchSensorComponent::SetModulatorPath(const core::string& name)
{
	m_data->modulatorPath=name;
	return true;
}
const core::string& SpringTouchSensorComponent::GetModulatorPath()
{
	return m_data->modulatorPath;
}
void SpringTouchSensorComponent::PreUpdate()
{
}

void SpringTouchSensorComponent::Update(float dt)
{
	float amount=0;

	for(int i=0;i<m_data->contactPoints.size();++i)
	{
		amount+=m_data->contactPoints[i].affect*m_data->contactPoints[i].force;
	}
	m_data->resForce.value=amount;//.set(amount,0,0);//fbparam;

	if(m_data->modulator)
		amount=m_data->modulator->ModulateValue(amount);
	m_data->modulatedForce.value=amount;//.set(amount,0,0);
	//signal force
	if(m_data->ggComponent)
		m_data->ggComponent->SetChannelValue(m_data->channel,m_data->modulatedForce.value);
	m_data->contactPoints.clear();
}
void SpringTouchSensorComponent::Update_old(float dt)
{
	static const float distance=0.005;
	physics::IPhysicalNode* node=m_data->targetNode->GetPhysicalNode();
	math::vector3d p=node->getGlobalPosition();
	math::quaternion q=node->getGlobalOrintation();
	//process each point and check for potential contacts by raycasting using physics system
	math::Ray3d ray;
	physics::IPhysicManager* phmngr= m_data->mngr->GetPhysicsManager();
	physics::PhysicsRaycaseHit hit;
	math::vector3d accValue;


	math::vector3d lvel=m_data->targetNode->GetPhysicalNode()->getLinearVelocity();
	float v1=lvel.Normalize();
	math::vector3d lpoint;
	math::vector3d vel;
	for(int i=0;i<m_data->contactPoints.size();++i)
	{
		if(m_data->contactPoints[i].erased)
			continue;
		ray.Dir=q*m_data->contactPoints[i].normal;
		ray.Start=p+q*m_data->contactPoints[i].position;//-ray.Dir*0.001;
		if(!phmngr->RaycastClosestShape(ray,physics::EShape_All,hit,distance) || hit.shape && hit.shape->getPhysicalNode()==m_data->targetNode->GetPhysicalNode())
		{
			//mark it as erased
			m_data->contactPoints[i].erased=true;
		}else
		{
			physics::IPhysicalNode* n=hit.shape->getPhysicalNode();
			vel=n->getPointVelocity(hit.worldImpact);
			float v2=vel.Normalize();
			float force=(1-lvel.dotProduct(vel));
			if(force<=0)			
			{
				m_data->contactPoints[i].erased=true;
				continue;
			}
			force*=v1*v2*n->getMass()*node->getMass();

			m_data->contactPoints[i].lastContact=n;
			
			_ChangeStatus(n,true);

			float weight=1-hit.distance/distance;
			accValue+= (ray.Dir)*m_data->contactPoints[i].force*weight;
		}
	}
	if(accValue.LengthSQ()>0.1)
	{
	//	printf("%s\n",core::StringConverter::toString(accValue).c_str());
	}
	m_data->resForce.value=accValue.x;//fbparam;

	//signal force
//	m_data->driver->SendData(&m_data->resForce);
}

void SpringTouchSensorComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	float averageForce=0;
	math::quaternion q=m_data->targetNode->GetPhysicalNode()->getGlobalOrintation();
	math::vector3d pos=m_data->targetNode->GetPhysicalNode()->getGlobalPosition();
	if(m_data->targetNode && m_data->targetNode->GetPhysicalNode())
	{
		
		for(int i=0;i<m_data->contactPoints.size();++i)
		{
 			if(m_data->contactPoints[i].erased)
 				continue;
			math::vector3d start=pos+q*m_data->contactPoints[i].position;
			math::vector3d end=start+q*m_data->contactPoints[i].normal*m_data->contactPoints[i].force;
			renderer->AddLine(start,end,video::DefaultColors::Green,DebugRenderSettings::LineWidth);
		}
		renderer->AddLine(pos,pos+q*math::vector3d(m_data->resForce.value,0,0),video::DefaultColors::Red,DebugRenderSettings::LineWidth);
	}
}


void SpringTouchSensorComponent::OnDestroy()
{
}



void SpringTouchSensorComponent::_OnStartTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
}

void SpringTouchSensorComponent::_OnTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	physics::IPhysicalNode* node= m_data->targetNode->GetPhysicalNode();
	math::quaternion q=node->getGlobalOrintation();
	math::quaternion qinv=q.inverse();
	math::vector3d pos=node->getGlobalPosition();
	math::vector3d averageNormal;
	math::vector3d averagePoint;
	
	float averageForce=0;
	int cnt=0;
	int pCnt=0;

	//calculate global sensor's normal
	//math::vector3d snormal=q*math::vector3d::ZAxis;
	const physics::IPhysicContactStreamIterator::PairsInfoList& pairs= pair->contactStream->GetPairs();
	physics::IPhysicContactStreamIterator::PairsInfoList::const_iterator it=pairs.begin();
	for(;it!=pairs.end();++it)
	{

		physics::IPhysicalShape* shape;
		//ignore self contacts
		if((*it).s1->getPhysicalNode()==(*it).s2->getPhysicalNode())
			continue;
		shape=(*it).s1;
		//check the owner of the shape, and shape's name
		if(shape->getPhysicalNode()==m_data->targetNode->GetPhysicalNode())
		{
			if(shape!=m_data->shape)
				continue;
		}else
		{
			shape=(*it).s2;
			if(shape!=m_data->shape)
				continue;
		}

		SpringTouchSensorComponentImpl::SpringContactPoint point;

		const physics::IPhysicContactStreamIterator::PatchInfoList& patchList=(*it).patches;
		physics::IPhysicContactStreamIterator::PatchInfoList::const_iterator patchIt=patchList.begin();
		
		for(;patchIt!=patchList.end();++patchIt)
		{
			const physics::IPhysicContactStreamIterator::PointsInfoList& pointhList=(*patchIt).points;
			physics::IPhysicContactStreamIterator::PointsInfoList::const_iterator pointIt=(*patchIt).points.begin();

			point.normal=qinv*(*patchIt).normal;
			//point.affect=point.normal.dotProduct(snormal);
			point.force=0;
			++pCnt;

			point.affect=point.normal.Length();//-point.normal.x;// it is equal to: point.normal.dotProduct(math::vector3d::XAxis);
			if(point.affect<=0)
				continue;//this force has no effect on sensor's axis

			int pointsCount=0;
			for(;pointIt!=pointhList.end();++pointIt)
			{
				++pointsCount;
				++cnt;

				point.force+=(*pointIt).force;
				point.position+=(*pointIt).point;
			}
			if(pointsCount>0)
			{
				point.time=0;
				point.position/=pointsCount;
				//convert to local coordinate
				point.position=qinv*point.position-pos;
				m_data->AddPoint(point);
			}
		}
	}
	if(cnt>0)
	{

	}
}
void SpringTouchSensorComponent::_OnSlide(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	//printf("Slide: %s,%s\n",m_data->targetNode->GetName().c_str(),c->GetName());
}
void SpringTouchSensorComponent::_OnEndTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	//printf("End touch: %s,%s\n",m_data->targetNode->GetName().c_str(),c->GetName());
}
core::string SpringTouchSensorComponent::GetDebugString()
{
	char buff[512];
	sprintf(buff,"%.2f --> %.2f",m_data->resForce.value*100,m_data->modulatedForce.value*100);
	return buff;
}

}
}
