

/********************************************************************
	created:	2012/02/12
	created:	12:2:2012   22:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\IObjectComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	IObjectComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IObjectComponent__
#define __IObjectComponent__

#include "IObject.h"

namespace mray
{

class MRAY_DLL IObjectComponent:public IObject
{
	typedef std::map<const RTTI*,std::list<IObjectComponent*>> ComponentsMap;

private:
protected:
	ComponentsMap m_components;
	std::list<IObjectComponent*> m_componentList;

	IObjectComponent* m_ownerComponent;
	void _SearchForComponentByType(const RTTI* type,std::list<IObjectComponent*>& lst)const;
public:
	IObjectComponent();
	virtual~IObjectComponent();

	//override this function if you want to make from this component unique,can't be added to object more than once
	virtual bool IsUnique()const{return false;}

	virtual bool CanAddComponent(IObjectComponent* comp);

	virtual bool AddComponent(IObjectComponent* comp);

	virtual void RemoveComponent(IObjectComponent* comp);

	const ComponentsMap& GetComponents()const;
	const std::list<IObjectComponent*>& GetComponentsList()const{return m_componentList;}

	virtual void ClearComponents();

	virtual void OnAttachedToComponent(IObjectComponent* owner);
	virtual void OnRemovedFromComponent(IObjectComponent* owner);
	IObjectComponent* GetOwnerComponent()const;

	virtual const std::list<IObjectComponent*>& GetComponent(const RTTI* type)const;
	virtual const std::list<IObjectComponent*>& GetComponent(const core::string& type)const;

	virtual IObjectComponent* GetFirstComponent(const RTTI* type)const;
	virtual IObjectComponent* GetFirstComponent(const core::string& type)const;

	virtual std::list<IObjectComponent*> SearchForComponentByType(const RTTI* type)const;

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};


}

#endif
