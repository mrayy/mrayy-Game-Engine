


/********************************************************************
	created:	2010/07/06
	created:	6:7:2010   21:42
	filename: 	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM\CharacterObject.h
	file path:	i:\Programing\GameEngine\mrayEngine\gameMods\CharacterDemoGM
	file base:	CharacterObject
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef CharacterObject_h__
#define CharacterObject_h__


#include "IAnimationMixer.h"
#include "AnimationController.h"
#include "CharacterCostFunction.h"

namespace mray
{
	namespace AI
	{
		class PathFindPlanner;
		class SteerController;
		class IAIPhysics;
		class AIActor;
	}
namespace GameMod
{

	class AICharacterArch;
	class CFood;

class CharacterObject
{
private:
protected:

	enum EAnimationType
	{
		EAT_None,
		EAT_Idle,
		EAT_IdleWall,
		EAT_Walk,
		EAT_Run,
		EAT_Talk,
		EAT_Shout,
		EAT_Listen,
		EAT_Custom
	}m_animation;

	CharacterCostFunction* m_costFunction;

	bool m_animatedConstrained;
	bool m_busy;

	AICharacterArch* m_arch;
	animation::IAnimationMixer* m_animMixer;
	core::array<animation::AnimationController*> m_activeTracks;

	AI::PathFindPlanner* m_planner;
	AI::SteerController* m_steer;
	AI::IAIPhysics* m_physics;

	CharacterObject* m_targetChar;

	bool m_animLegs;
	bool m_isChating;
	bool m_waitingForChat;
	bool m_requestedFood;

	bool m_physicsBased;

	// inner parameters
	float m_energy;	//controls the character energy and the need to find food
	float m_maxEnergy;


	void _SetTracksSpeed(float speed);
	void _StopAllTracks();
	void _PlayAllTracks();
public:
	CharacterObject(AICharacterArch* arc,animation::IAnimationMixer* animMixer,bool animLegs,bool physicsBased);
	virtual ~CharacterObject();

	AI::AIActor* GetAIActor();
	AI::SteerController* GetSteering();
	AI::IAIPhysics* GetPhysics();
	AI::PathFindPlanner* GetPathfinder();

	void RunScenario(const core::string& path);

	void LookAtTarget();
	void SetTargetFriend(CharacterObject* target);
	CharacterObject* GetTargetFriend();
	bool RequestToTalk(CharacterObject* target);
	bool WaitingForChat(){return m_waitingForChat;}
	bool IsChating();
	void StartChat();
	void StartListenToChat();
	void EndChat(bool interupt=false);

	void SetBusy(bool b);
	bool IsBusy();

	float GetEnergy();

	void AnimateTalk();
	void AnimateListen();
	void AnimateShout();

	void PlayAnimation(const core::string&name,float speed);
	animation::AnimationController* GetAnimationGroup(const core::string&name);

	void SetAnimationConstrained(bool c);
	
	void AnimateIdle();
	void AnimateWalk(float speed);
	void AnimateRun(float speed);

	// go to position in the map
	void GotoPosition(const math::vector3d& pos);

	void FindFood();

	void Eat(CFood*food);

	// stop all active goals, and perform idle
	void StopAndIdle();

	bool IsHungry();

	void Update(float dt);

	AI::ICostFunction* GetCostFunction();
};

}
}


#endif // CharacterObject_h__