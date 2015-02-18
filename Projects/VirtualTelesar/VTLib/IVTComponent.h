

/********************************************************************
	created:	2012/08/07
	created:	7:8:2012   1:29
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\IVTComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	IVTComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IVTComponent___
#define ___IVTComponent___

#include "IGameComponent.h"


namespace mray
{
	namespace GUI
	{
		class IGUIElement;
		class IGUIManager;
	}
namespace VT
{


class IVTComponent:public game::IGameComponent
{
	DECLARE_RTTI
protected:


public:
	IVTComponent();
	virtual~IVTComponent();

	game::IGameComponent* RetriveSubComponent(const core::string& type,const core::string& name); 
	virtual GUI::IGUIElement* GeneratedUIElement(GUI::IGUIManager* mngr){return 0;}
	
};

#define BEGIN_DECLARE_VTCOMPONENT(ClassName,ParentComponent)\
class ClassName;\
class ClassName:public ParentComponent\
{\
	DECLARE_RTTI;\
protected:\
public:\
	ClassName(game::GameEntityManager* mngr);\
	virtual~ClassName();\
	virtual bool InitComponent();\
	virtual void Update(float dt);\


#define END_DECLARE_VTCOMPONENT(ClassName) \
};\
	DECLARE_GAMECOMPONENT_FACTORY(ClassName);

#define IMPLEMENT_VTCOMPONENT(ClassName,ParentComponent)\
	IMPLEMENT_RTTI(ClassName,ParentComponent);\
	IMPLEMENT_GAMECOMPONENT_FACTORY(ClassName);

#define DECLARE_VTPROPERTY(Name,Type) \
	DECLARE_PROPERTY_TYPE(Name,Type,);\
	DECLARE_SETGET_PROP(Name,Type,);

}
}

#endif
