#ifndef ___CPropertieSet___
#define ___CPropertieSet___


#include "IProperty.h"
#include "XMLElement.h"

#include "IDelegate.h"
#include "IDelegateContainer.h"
#include "RTTI.h"


namespace mray{

	class RTTI;
	class CPropertieDictionary;

	typedef DelegateEvent2<IObject*,PVOID> ObjectEvent;
#define CreateObjectDelegate(ObjectType,obj,Delegate) newClassDelegate2<ObjectType,void,IObject*,void*>(#Delegate,obj,&ObjectType::Delegate)


typedef std::map<core::string,IProperty*> TPropertieList;
typedef TPropertieList::iterator TPropertieListIT;

typedef std::map<core::string,ObjectEvent*> TEventsList;
typedef TEventsList::iterator TEventsListIT;

typedef std::map<core::string,CPropertieDictionary*> TSubPropertyDictionary;
typedef TSubPropertyDictionary::iterator TSubPropertyDictionaryIT;


class SPropertyGroup
{
public:
	SPropertyGroup()
	{
	}
	~SPropertyGroup()
	{
		properties.clear();
	}
	core::string name;
	TPropertieList properties;
};

typedef std::map<core::string,SPropertyGroup> TPropertieGroupList;


class MRAY_DLL CPropertieDictionary
{
protected:
	TPropertieList m_properties;
	TPropertieGroupList m_groups;
	
	TSubPropertyDictionary m_subDics;

	const RTTI* m_rtti;


public:

	CPropertieDictionary(const RTTI*rtti);
	virtual~CPropertieDictionary();

	const mray::GUID* getPropertyType(const core::string&name);
	int getPropertiesCount();
	const TPropertieList& getProperties();


	const TPropertieGroupList& getGroups();

	IProperty*getPropertie(const core::string&name);


	const RTTI* GetRTTI()const{return m_rtti;}


	void addPropertie(IProperty*prop,const core::string&group=mT("Default"));

	void addSubDictionary(const core::string&name, CPropertieDictionary* set);
	CPropertieDictionary* GetSubDictionary(const core::string&name);
	const TSubPropertyDictionary&getSubDictionaries();

	SPropertyGroup*getGroup(const core::string&name);

	core::string getPropertieInfo(const core::string&name);
	void setPropertieInfo(const core::string&name,const  core::string&info);

	virtual void OnVisit(IPropertyVistor*visitor);
};



class MRAY_DLL CPropertieSet:public IDelegateContainer
{
	DECLARE_RTTI;
public:
	typedef std::map<core::string,CPropertieSet*> SubsetMap;
private:
	typedef std::map<core::string,GCPtr<CPropertieDictionary>> PropertyDictionaryMap;
	static PropertyDictionaryMap m_dictionaries;
protected:

	typedef std::map<core::string,std::list<core::string>> EventDelegateMap;
	EventDelegateMap m_eventDelegMap;

	typedef std::list<IDelegateContainer*> EventContainerList;
	EventContainerList m_eventContainer;
	CPropertieDictionary* m_dictionary;
	SubsetMap m_subSets;
	TEventsList m_events;

	void _OnEventFired(ObjectEvent* name,IObject* p1,PVOID p2);
public:

	static bool CreateDictionary(const core::string&name,const RTTI*obj,CPropertieDictionary**dic);
	static CPropertieDictionary* GetDictionary(const core::string&name);

	CPropertieSet();
	virtual~CPropertieSet();

	bool CreateDictionary(CPropertieDictionary**dic);
	CPropertieDictionary* GetDictionary();

	core::string GetPropertyValue(const core::string&name);
	void SetPropertyValue(const core::string&name,const core::string&v);

	void AddSubSet(const core::string&name,CPropertieSet* s);
	CPropertieSet* GetSubSet(const core::string&name);
	const SubsetMap& GetSubSets();


	void AddEvent(ObjectEvent*e);
	ObjectEvent* GetEvent(const core::string&name);
	void FireEvent(const core::string& name,IObject* p1,PVOID p2);
	int GetEventsCount();
	const TEventsList& GetEvents();

	void AddEventContainer(IDelegateContainer* d);
	void RemoveEventContainer(IDelegateContainer* d);
	void ClearEventContainers();
	void AddEventDelegatePair(const core::string& event,const core::string& target);
	void RemoveEventDelegatePair(const core::string& event,const core::string& target);
	void ClearDelegatesForEvent(const core::string& event);


	virtual void loadFromXML(xml::XMLElement* elem);
	virtual void exportToXML(xml::XMLElement* elem);


	virtual void OnVisit(IPropertyVistor*visitor);
};

}


#endif