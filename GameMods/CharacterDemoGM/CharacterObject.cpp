
#include "stdafx.h"
#include "CharacterObject.h"
#include "Thinking.h"
#include "Randomizer.h"

#include "AICharacterArch.h"
#include "GoalWait.h"
#include "GoalGoToPoint.h"
#include "GoalFindFood.h"
#include "GoalScenario.h"

#include "AIComponentTypes.h"
#include "IAIPhysics.h"

#include "CHouse.h"
#include "CFood.h"

#include "GoalsTypeEnum.h"
#include "GoalWaitForChat.h"

#include "AIActor.h"
#include "PathFindPlanner.h"

namespace mray
{
namespace GameMod
{

CharacterObject::CharacterObject(AICharacterArch*arch,animation::IAnimationMixer* animMixer,bool animLegs,bool physicsBased)
{
	m_arch=arch;
	m_animLegs=animLegs;
	m_animMixer=animMixer;
	m_targetChar=0;
	m_waitingForChat=0;
	m_costFunction=0;
	m_physicsBased=physicsBased;

	m_animatedConstrained=false;
	m_busy=false;
	m_isChating=false;

	m_maxEnergy=50;
	m_energy=40;

	m_planner=(AI::PathFindPlanner*) arch->GetComponent(AI::GetComponentName(AI::ECT_PathFinder));
	m_steer=(AI::SteerController*) arch->GetComponent(AI::GetComponentName(AI::ECT_Steering));
	m_physics=(AI::IAIPhysics*) arch->GetComponent(AI::GetComponentName(AI::ECT_Physics));

	m_animation=EAT_None;

	CHouse::getInstance().AddCharacter(this);

	m_costFunction=new CharacterCostFunction(arch->GetOwner()->GetNavWorld()); 
	if(m_physicsBased)
		m_costFunction->ChangeWeight(mT("slop"),0);

	m_planner->SetCostFunction(m_costFunction);

}
CharacterObject::~CharacterObject()
{
	CHouse::getInstance().RemoveCharacter(this);
	//delete m_costFunction;
}


void CharacterObject::RunScenario(const core::string& path)
{
	m_arch->GetBrain()->GetThinkingGoal()->removeAllGoals();
	m_arch->GetBrain()->GetThinkingGoal()->addSubGoal(new AI::GoalScenario(m_arch,path));
}

AI::AIActor* CharacterObject::GetAIActor()
{
	return m_arch->GetOwner();
}
AI::SteerController* CharacterObject::GetSteering()
{
	return m_steer;
}
AI::IAIPhysics* CharacterObject::GetPhysics()
{
	return m_physics;
}
AI::PathFindPlanner* CharacterObject::GetPathfinder()
{
	return m_planner;
}
float CharacterObject::GetEnergy()
{
	return m_energy;
}

void CharacterObject::SetBusy(bool b)
{
	m_busy=b;
}

bool CharacterObject::IsBusy()
{
	return m_busy;
}

void CharacterObject::LookAtTarget()
{
	if(!m_targetChar)
		return;

	math::vector3d pos=m_physics->GetGlobalPosition();
	math::vector3d target=m_targetChar->GetPhysics()->GetGlobalPosition();
	pos.y=0;
	target.y=0;

	math::matrix4x4 m,lookAt;
	m.buildCameraLookAtLH(pos,target,math::vector3d::YAxis);
	m.getTransposeMatrix(lookAt);
	math::quaternion q(lookAt);
	//math::vector3d angle;
	//q*=m_physics->GetGlobalOrintation().inverse();
	//q.toEulerAngles(angle);
	m_physics->SetGlobalOrintation(q);
	m_physics->SetAngularVelocity(math::vector3d::Zero);
}

void CharacterObject::SetTargetFriend(CharacterObject* target)
{
	m_targetChar=target;
}
CharacterObject* CharacterObject::GetTargetFriend()
{
	return m_targetChar;
}
bool CharacterObject::RequestToTalk(CharacterObject* target)
{
	if(IsBusy() && m_targetChar!=target)
		return false;

	if(m_targetChar==target)
		return true;

	m_targetChar=target;
	LookAtTarget();

	m_waitingForChat=true;

	SetBusy(true);
	m_arch->GetBrain()->GetThinkingGoal()->addSubGoal(new GoalWaitForChat(this));
	return true;
}

bool CharacterObject::IsChating()
{
	return m_isChating;
}

void CharacterObject::StartChat()
{
	AnimateTalk();
	SetBusy(true);
	m_isChating=true;
	SetAnimationConstrained(true);
	LookAtTarget();
	m_physics->SetAngularVelocity(0);
}
void CharacterObject::StartListenToChat()
{
	AnimateListen();
	SetBusy(true);
	m_isChating=true;
	SetAnimationConstrained(true);
	LookAtTarget();
	m_physics->SetAngularVelocity(0);
}
void CharacterObject::EndChat(bool interupt)
{
	SetBusy(false);
	m_isChating=false;
	SetAnimationConstrained(false);
	m_targetChar=0;
	m_waitingForChat=false;
	if(interupt)
		m_targetChar->EndChat();
}

void CharacterObject::SetAnimationConstrained(bool c)
{
	m_animatedConstrained=c;
}

void CharacterObject::_StopAllTracks()
{
	for(int i=0;i<m_activeTracks.size();++i){
		if(m_activeTracks[i]){
			//m_activeTracks[i]->stopAll();
			m_activeTracks[i]->setWeight(1);
			m_activeTracks[i]->setTargetDelay(1);
			m_activeTracks[i]->setTargetWeight(0);
		}
	}
	m_activeTracks.clear();
}
void CharacterObject::_PlayAllTracks()
{
	if(m_physicsBased)
		return;
	for(int i=0;i<m_activeTracks.size();++i){
		if(m_activeTracks[i]){
			m_activeTracks[i]->getGroup()->playAll();
			m_activeTracks[i]->play(1,animation::EAM_Cycle);
			m_activeTracks[i]->setWeight(0);
			m_activeTracks[i]->setTargetDelay(1);
			m_activeTracks[i]->setTargetWeight(1);
		}
	}
}
void CharacterObject::_SetTracksSpeed(float speed)
{
	for(int i=0;i<m_activeTracks.size();++i){
		if(m_activeTracks[i]){
			m_activeTracks[i]->setSpeed(speed);
		}
	}
}


void CharacterObject::AnimateTalk()
{
	if(m_animation==EAT_Talk)
		return;
	m_animation=EAT_Talk;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(mT("Talk1")));
		_PlayAllTracks();
		_SetTracksSpeed(10);
	}
}
void CharacterObject::AnimateListen()
{
	if(m_animation==EAT_Listen)
		return;
	m_animation=EAT_Listen;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(mT("Listen")));
		_PlayAllTracks();
		_SetTracksSpeed(10);
	}
}
void CharacterObject::AnimateShout()
{
	if(m_animation==EAT_Shout)
		return;
	m_animation=EAT_Shout;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(mT("Shout")));
		_PlayAllTracks();
		_SetTracksSpeed(25);
	}
}

void CharacterObject::AnimateIdle()
{
	if(m_animation==EAT_Idle)
		return;
	m_animation=EAT_Idle;
	core::string IdleAnimations[2]=
	{
		mT("Idle1"),
		mT("Idle1")
	};
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(IdleAnimations[math::Randomizer::rand(2)]));
		_PlayAllTracks();
		_SetTracksSpeed(10);
	}
}
void CharacterObject::AnimateWalk(float speed)
{
	if(m_animation==EAT_Walk){
		_SetTracksSpeed(speed);
		return;
	}
	m_animation=EAT_Walk;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(mT("Walk")));
		_PlayAllTracks();
		_SetTracksSpeed(speed);
	}
}
void CharacterObject::AnimateRun(float speed)
{
	if(m_animation==EAT_Run){
		_SetTracksSpeed(speed);
		return;
	}
	m_animation=EAT_Run;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(mT("Run")));
		_PlayAllTracks();
		_SetTracksSpeed(speed);
	}
}

void CharacterObject::PlayAnimation(const core::string&name,float speed)
{
	m_animation=EAT_Custom;
	if(m_animMixer)
	{
		_StopAllTracks();
		m_activeTracks.push_back(m_animMixer->getAnimationController(name));
		_PlayAllTracks();
		_SetTracksSpeed(speed);
	}
}
animation::AnimationController* CharacterObject::GetAnimationGroup(const core::string&name)
{
	return m_animMixer->getAnimationController(name);
}

void CharacterObject::GotoPosition(const math::vector3d& pos)
{
	if(IsBusy())
		return;
	m_arch->GetBrain()->GetThinkingGoal()->removeAllGoals();
	m_arch->GetBrain()->GetThinkingGoal()->addSubGoal(new GoalGoToPoint(this,pos));
	//AnimateWalk();
}

void CharacterObject::StopAndIdle()
{
	m_arch->GetBrain()->GetThinkingGoal()->removeAllGoals();
	m_arch->GetBrain()->GetThinkingGoal()->addSubGoal(new GoalWait(this));
	m_physics->SetLinearVelocity(0);
	//AnimateIdle();

}
void CharacterObject::FindFood()
{
	if(IsChating())
		EndChat(true);
	if(m_arch->GetBrain()->GetThinkingGoal()->isGoalPresented(EGoal_FindFood))
		return;
	//m_arch->GetBrain()->GetThinkingGoal()->removeAllGoals();
	m_arch->GetBrain()->GetThinkingGoal()->addSubGoal(new GoalFindFood(this,m_maxEnergy-m_energy,EFT_Meat));
	SetBusy(true);
}
void CharacterObject::Eat(CFood*food)
{
	m_requestedFood=false;
	float a=food->GetAmount();
	float req=min(a,max(0,min(m_maxEnergy,a)-m_energy));
	
	food->SetAmount(a-req);
	m_energy+=req;
	
}

void CharacterObject::Update(float dt)
{
	float len=m_physics->GetLinearVelocity().Length();
	if(!m_animatedConstrained){
		if(len==0)
			AnimateIdle();
		else if(len<4)
			AnimateWalk(math::Max<float>(len*0.4,1)*20);
		else
			AnimateRun((len-3)*20);
	}

	/*m_energy-=len*dt*0.5;
	if(m_energy<0)
		m_energy=0;

	if(IsHungry() && !m_requestedFood)
	{
		m_requestedFood=true;
		FindFood();
	}*/
}



bool CharacterObject::IsHungry()
{
	return m_energy<20;
}

}
}