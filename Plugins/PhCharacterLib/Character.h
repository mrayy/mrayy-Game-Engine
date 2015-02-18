/********************************************************************
	created:	2010/03/15
	created:	15:3:2010   1:09
	filename: 	d:\Character Animation\Solution\Project\Character.h
	file path:	d:\Character Animation\Solution\Project
	file base:	Character
	file ext:	h
	author:		Khaldoon Ghanem
	
	purpose:	
*********************************************************************/
#ifndef Character_h__
#define Character_h__

#include <IPhysicalNode.h>
#include "CompileConfig.h"
#include "IObject.h"
#include "ListenerContainer.h"
#include "ICharacterListener.h"
#include "RTTI.h"

namespace mray
{
	namespace scene
	{
		class IDebugDrawManager;
	}
namespace PhysicsBasedCharacter
{
	class Segment;
	class Environment;
	class Controller;
	class ControllingParams;
	class CharacterGait;
	class CharacterManager;
	class CharacterTracking;

	class MRAY_PH_CHARACTER_DLL Character:public IObject,public ListenerContainer<ICharacterListener>
	{
		DECLARE_RTTI;
	private:
		Segment* m_rootSegment;
		std::vector<Controller*> m_controllingSlots;
		std::vector<Controller*> m_waitingSlots;
		std::map<core::string, Controller*> m_controllers;
		std::map<core::string, Segment*> m_segments;
		CharacterGait* m_gait;
		CharacterTracking* m_tracking;
		CharacterManager* m_creator;
		scene::IMovable* m_model;
		scene::Skeleton* m_duplicat;
		scene::Skeleton* m_skeleton;

		int m_numOfSlots;
		bool isActive;

		float m_scale;

		IObject* m_userData;

		DECLARE_FIRE_METHOD(OnCharacterContact,(Character* ch,physics::IPhysicalNode* node),(ch,node));

	public:
		Character( CharacterManager* creator);
		virtual~Character(void);

		bool Create(const core::string& descriptionFile, scene::Skeleton* skeleton,float scale,
			const math::vector3d &pos,	physics::IPhysicManager*phMngr,IObject* data=0);

		CharacterManager* GetCreator(){return m_creator;}

		void playController(const core::string& controllerName, int slotNum);
		void swapController(const core::string& controllerName, int slotNum);
		void forcePlayController(const core::string& controllerName, int slotNum);
		void stopController(int slotNum);
		void forceStopController(int slotNum);
		core::string getControllerName(int slotNum);
		void getControllingNames(std::list<core::string>&names);
		Segment* getSegment(const core::string& name);
		void setParams(const core::string& controllerName, ControllingParams* params);
		void applyForce(const core::string& segmentName, math::vector3d& force, float dt);
		Segment* getRoot();
		CharacterGait* getGait();


		CharacterTracking* getTracking();
		void setModel(scene::IMovable*m){m_model=m;}
		scene::IMovable* getModel(){return m_model;}
		scene::Skeleton* getDuplicatedSkeleton(){return m_duplicat;}

		void activate();
		void deactivate();

		void OnContact(physics::IPhysicalNode* node);

		void SetUserData(IObject*d){ m_userData=d;}
		IObject* GetUserData(){return m_userData;}

		void update(float dt);
		void Visualize(scene::IDebugDrawManager* renderer);
	};

	MakeSharedPtrType(Character);
}
}
#endif // Character_h__
