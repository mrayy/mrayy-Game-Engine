


/********************************************************************
	created:	2012/02/12
	created:	12:2:2012   21:44
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IGameComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IGameComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IGameComponent__
#define __IGameComponent__

#include "CompileConfig.h"
#include "TypedProperty.h"
#include "IObjectComponent.h"
#include "IGameComponentFactory.h"

namespace mray
{
namespace scene
{
	class IDebugDrawManager;
}
namespace GUI
{
	class IGUIRenderer;
}
namespace game
{
	class GameEntity;
	class IGameVisitor;
	class GameEntityManager;

	class IGhostInterface;
 
class MRAY_GAME_DLL IGameComponent:public IObjectComponent
{
	DECLARE_RTTI;
private:
protected:
	GameEntity* m_ownerEntity;
	bool m_enabled;
	bool m_inited;
	core::string m_name;
	uint m_ID;
public:
	DECLARE_PROPERTY_TYPE(Enabled,bool,);
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_GAME_DLL);

	void _LoadComponents(xml::XMLElement*elem,const core::string&pref);

public:
	IGameComponent();
	virtual~IGameComponent();

	static game::IGameComponent* RetriveComponent(IObjectComponent*comp,const core::string& type,const core::string& name); 
	template<class T>
	static T* RetriveComponent(IObjectComponent*comp,const core::string& name)
	{
		return dynamic_cast<T*>(RetriveComponent(comp,T::getClassType(),name));
	}

	void SetPrefix(const core::string& p)
	{
		SetName(p+GetName());
	}

	void SetID(uint id){m_ID=id;}
	uint GetID(){return m_ID;}

	//called after loading finished and the component is ready to init it self
	virtual bool InitComponent();
	bool IsInited(){return m_inited;}

	GameEntity* GetOwnerEntity()const;

	virtual bool SetName(const core::string& name){m_name=name;return true;}
	const core::string& GetName(){return m_name;}

	virtual bool InvokeMessage(const core::string& msg,const std::vector<void*>& params){return false;}

	virtual void OnAttachedToComponent(IObjectComponent* owner);
	virtual void OnRemovedFromComponent(IObjectComponent* owner);

	virtual bool SetEnabled(bool enabled);
	bool IsEnabled()const;

	//depending on the type of the game component, if it was physic component then the result is a physical node object
	virtual IObject* GetAttachedObject(){return 0;}

	virtual void OnReset();

	virtual void PreUpdate();
	virtual void Update(float dt);
	virtual void LateUpdate(float dt);
	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	virtual void OnGUIRender(GUI::IGUIRenderer* renderer, const math::rectf& vp);

	virtual void OnDestroy();

	void Visit(IGameVisitor* visitor);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);

	//override to return ghost interface for this object, if exist
	virtual IGhostInterface* GetGhostInterface(){return 0;}
};


}
}

#endif