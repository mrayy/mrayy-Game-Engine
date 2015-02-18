


/********************************************************************
	created:	2012/07/24
	created:	24:7:2012   22:12
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\DistanceCalculatorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	DistanceCalculatorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___DistanceCalculatorComponent___
#define ___DistanceCalculatorComponent___


#include "IVTComponent.h"

namespace mray
{
namespace VT
{
	class MountableComponent;

class DistanceCalculatorComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:

	core::string m_target1;
	core::string m_target2;

	MountableComponent* m_comp1;
	MountableComponent* m_comp2;

public:
	DECLARE_PROPERTY_TYPE(TargetNode1,core::string,);
	DECLARE_PROPERTY_TYPE(TargetNode2,core::string,);
public:
	DistanceCalculatorComponent(game::GameEntityManager* mngr);
	virtual~DistanceCalculatorComponent();

	virtual bool InitComponent();

	bool SetNode1Name(const core::string &name);
	const core::string& GetNode1Name();

	bool SetNode2Name(const core::string &name);
	const core::string& GetNode2Name();

	float GetDistance();


	virtual void DebugRender(scene::IDebugDrawManager* renderer);
	
};
DECLARE_GAMECOMPONENT_FACTORY(DistanceCalculatorComponent);

}
}

#endif
