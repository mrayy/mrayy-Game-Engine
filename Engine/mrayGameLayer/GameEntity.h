


/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   16:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayGameLayer\GameEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayGameLayer
	file base:	GameEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GameEntity_h__
#define GameEntity_h__

#include "CompileConfig.h"
#include "GCPtr.h"
#include "IGameAction.h"
#include "IObjectComponent.h"
#include "TypedProperty.h"

namespace mray
{
namespace scene
{
	class ISceneNode;
}
namespace AI
{
	class AIActor;
}
namespace game
{
	class GameEntityManager;
	class IGameComponent;
	class IGameVisitor;
	class SceneComponent;
	class AIComponent;

	typedef std::list<GUID> GameEntityTagList;

class MRAY_GAME_DLL GameEntity:public IGameComponent
{
	DECLARE_RTTI;
private:
protected:
//	IGameAction* m_action;
	GameEntityManager* m_manager;
	std::vector<core::string> m_loadedArchives;
	std::vector<core::string> m_materialSets;

	GameEntityTagList m_tags;

// 	SceneComponent* m_sceneComponent;
// 	AIComponent* m_aiComponent;
	void _SetCreator(GameEntityManager*mngr){m_manager=mngr;}

	friend class GameEntityManager;
public:
//	DECLARE_PROPERTY_TYPE(Action,core::string, );
public:
	GameEntity(const core::string& name,GameEntityManager*mngr);
	virtual ~GameEntity();

	void AddTag(const GUID& t);
	void RemoveTag(const GUID& t);
	void ClearTags();
	bool HasTag(const GUID& t);
	const GameEntityTagList& GetTags();


	void Initialize();

	//uint GetShapeID()const;

	GameEntityManager* GetCreator(){return m_manager;}

//	virtual bool SetGameAction(IGameAction* action);
//	virtual IGameAction* GetGameAction();

//	const core::string& GetActionName()const;
//	bool SetActionByName(const core::string&name);

	virtual const core::string& GetName();
	virtual bool SetName(const core::string &val) ;

	virtual void Update(float dt);

	/*
	std::list<IGameComponent*> GetGameComponents(const RTTI* type)const;
	std::list<IGameComponent*> GetGameComponents(const core::string& type)const;
*/
	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);


	void Visit(IGameVisitor* visitor);


};

}
}

#endif // GameEntity_h__
