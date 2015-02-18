

#include "stdafx.h"
#include "TouchSensorComponent.h"
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


namespace mray
{
namespace VT
{


	class TouchSenseorContactReporter:public physics::IPhysicUserContactReport
	{
	protected:
		TouchSensorComponent* m_owner;
	public:
		TouchSenseorContactReporter(TouchSensorComponent* o)
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
		void OnPairStartTouch(physics::PhysicContactPair* pair)
		{
			physics::IPhysicalNode* n=GetOtherNode(pair);
			if(!n)
				return;
			m_owner->_OnStartTouch(pair,n);
		}
		void OnPairTouch(physics::PhysicContactPair* pair)
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
		void OnPairEndTouch(physics::PhysicContactPair* pair)
		{
			physics::IPhysicalNode* n=GetOtherNode(pair);
			if(!n)
				return;
			m_owner->_OnEndTouch(pair,n);
		}
	};

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetNode,TouchSensorComponent,core::string,mT("TargetNode"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetNode,TouchSensorComponent,core::string,SetTargetNodeName,GetTargetNodeName,,,false);

IMPLEMENT_PROPERTY_TYPE_HEADER(TargetShape,TouchSensorComponent,core::string,mT("TargetShape"),EPBT_Basic,mT(""),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(TargetShape,TouchSensorComponent,core::string,SetTargetShapeName,GetTargetShapeName,,,false);


TouchSensorComponent::TouchSensorComponent(game::GameEntityManager*m)
{
	m_mngr=m;
	m_targetNode=0;
	m_reporter=new TouchSenseorContactReporter(this);
	CPropertieDictionary* dic;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeTargetNode::instance);
		dic->addPropertie(&PropertyTypeTargetShape::instance);
	}
}
TouchSensorComponent::~TouchSensorComponent()
{
	delete m_reporter;
}

bool TouchSensorComponent::InitComponent()
{
	if(!game::IGameComponent::InitComponent())
		return false;

	m_targetNode=RetriveComponent<game::PhysicsComponent>(GetOwnerComponent(),m_targetNodeName);
	if(m_targetNode)
	{
		m_targetNode->InitComponent();
		if(m_targetNode->GetPhysicalNode())
		{
			m_targetNode->GetPhysicalNode()->SetContactReport(m_reporter);
		//	m_targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_StartTouch);
			m_targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_Touch);
		//	m_targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_EndTouch);
		//	m_targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_Slide);
		//	m_targetNode->GetPhysicalNode()->EnableContactReportFlag(physics::ECRF_NotifyForces);
		}
	}

	return true;
}

void TouchSensorComponent::OnDestroy()
{
	IVTComponent::OnDestroy();
	if(m_targetNode)
		m_targetNode->GetPhysicalNode()->SetContactReport(0);
}

bool TouchSensorComponent::SetTargetNodeName(const core::string& name)
{
	m_targetNodeName=name;
	return true;
}
const core::string& TouchSensorComponent::GetTargetNodeName()
{
	return m_targetNodeName;
}
bool TouchSensorComponent::SetTargetShapeName(const core::string& name)
{
	m_targetShapeName=name;
	return true;
}
const core::string& TouchSensorComponent::GetTargetShapeName()
{
	return m_targetShapeName;
}
void TouchSensorComponent::DebugRender(scene::IDebugDrawManager* renderer)
{
	float averageForce=0;
	if(m_targetNode && m_targetNode->GetPhysicalNode())
	{
		for(int i=0;i<m_feedback.size();++i)
		{
			math::vector3d start=m_feedback[i].point;
			math::vector3d end=start+m_feedback[i].normal*m_feedback[i].force;
			renderer->AddLine(start,end,video::DefaultColors::Green,DebugRenderSettings::LineWidth);
			averageForce+=m_feedback[i].force;
		}
	}
// 	if(m_feedback.size()>0)
// 		averageForce/=(float)m_feedback.size();
// 	averageForce=math::Min(1.0f,averageForce);
// 	scene::ISceneNode* snode=	m_targetNode->GetTargetNode()->GetSceneNode();
// 	const scene::AttachNodesList& nodeList= snode->GetAttachedNodes();
// 	scene::AttachNodesList::const_iterator it=nodeList.begin();
// 	for(;it!=nodeList.end();++it)
// 	{
// 		for(int i=0;i<(*it)->getMaterialCount();++i)
// 		{
// 			video::RenderMaterial* mat= (*it)->getMaterial(i);
// 			mat->GetActiveTechnique()->GetPassAt(0)->SetDiffuse(video::SColor(1,0,0,1)*averageForce);
// 		}
// 	}

	game::IGameComponent::DebugRender(renderer);
}
void TouchSensorComponent::PreUpdate()
{
	IVTComponent::PreUpdate();

	m_feedback.clear();

}

void TouchSensorComponent::Update(float dt)
{
	IVTComponent::Update(dt);

	if(m_feedback.size()>0)
	{
		math::vector3d norm= m_targetNode->GetPhysicalNode()->getGlobalOrintation()*math::vector3d::ZAxis;
		TouchSensorFeedback fb;
		fb.owner=this;
		fb.name="Touch";

		for(int i=0;i<m_feedback.size();++i)
		{
			//calculate the applied force : f*dot(Nforce,Nsensor)
			fb.value.force+=m_feedback[i].force*m_feedback[i].normal.dotProduct(norm);
			fb.value.normal+=m_feedback[i].normal;
			fb.value.point+=m_feedback[i].point;
		}

		fb.value.normal/=(float)m_feedback.size();
		fb.value.point/=(float)m_feedback.size();

		FIRE_LISTENR_METHOD(OnFeedbackValue,(this,&fb));
	}



}

void TouchSensorComponent::_OnStartTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
}

void TouchSensorComponent::_OnTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	math::vector3d averageNormal;
	math::vector3d averagePoint;
	float averageForce=0;
	int cnt=0;
	int pCnt=0;
	const physics::IPhysicContactStreamIterator::PairsInfoList& pairs= pair->contactStream->GetPairs();
	physics::IPhysicContactStreamIterator::PairsInfoList::const_iterator it=pairs.begin();
	for(;it!=pairs.end();++it)
	{

		physics::IPhysicalShape* shape;
		shape=(*it).s1;
		//check the owner of the shape, and shape's name
		if(shape->getPhysicalNode()==m_targetNode->GetPhysicalNode())
		{
			if(shape->getName()!=m_targetShapeName)
				continue;
		}else
		{
			shape=(*it).s2;
			if(shape->getName()!=m_targetShapeName)
				continue;
		}
		const physics::IPhysicContactStreamIterator::PatchInfoList& patchList=(*it).patches;
		physics::IPhysicContactStreamIterator::PatchInfoList::const_iterator patchIt=patchList.begin();

		for(;patchIt!=patchList.end();++patchIt)
		{
			const physics::IPhysicContactStreamIterator::PointsInfoList& pointhList=(*patchIt).points;
			physics::IPhysicContactStreamIterator::PointsInfoList::const_iterator pointIt=(*patchIt).points.begin();

			averageNormal+=(*patchIt).normal;
			++pCnt;
			for(;pointIt!=pointhList.end();++pointIt)
			{
				averageForce+=(*pointIt).force;
				averagePoint+=(*pointIt).point;
// 				fb.value.point=pair->contactStream->GetPoint();
// 				fb.value.normal=pair->contactStream->GetPatchNormal();
// 				fb.value.force=pair->contactStream->GetPointNormalForce();
// 				m_feedback.push_back(fb);
// 				FIRE_LISTENR_METHOD(OnFeedbackValue,(this,&fb));
				cnt++;
			}
		}
	}
	if(cnt>0)
	{
		TouchSensorFBType value;
		value.point=averagePoint/(float)cnt;
		value.normal=averageNormal/(float)pCnt;
		value.force=averageForce;///(float)cnt;
		m_feedback.push_back(value);
	}
}
void TouchSensorComponent::_OnSlide(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	//printf("Slide: %s,%s\n",m_targetNode->GetName().c_str(),c->GetName());
}
void TouchSensorComponent::_OnEndTouch(physics::PhysicContactPair* pair,physics::IPhysicalNode* other)
{
	game::PhysicsComponent* c=dynamic_cast<game::PhysicsComponent*>( other->GetUserData());
	if(!c)
		return;
	//printf("End touch: %s,%s\n",m_targetNode->GetName().c_str(),c->GetName());
}

// void TouchSensorComponent::GetFeedbackResult(std::vector<IFeedbackResult*>& lst)
// {
// }
// void TouchSensorComponent::ClearFeedbackResults()
// {
// 	m_feedback.clear();
// }

}
}
