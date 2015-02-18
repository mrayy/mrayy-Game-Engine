

/********************************************************************
	created:	2012/08/25
	created:	25:8:2012   12:56
	filename: 	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib\ErrorCalculatorComponent.h
	file path:	d:\Development\mrayEngine\Projects\VirtualTelesar\VTLib
	file base:	ErrorCalculatorComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___ErrorCalculatorComponent___
#define ___ErrorCalculatorComponent___

#include "IVTComponent.h"

namespace mray
{
namespace VT
{
	class IControllableComponent;
	class CoupledJointComponent;
	class CoupledJointInfo;

	class ContorlComponentErrorInfo
	{
	public:
		CoupledJointInfo* comp;
		float errorValue;
	public:
		ContorlComponentErrorInfo():comp(0),errorValue(0)
		{
		}

	};

// this component calculates for each joint the error by substracting the desired rotation with the current rotation for each joint
class ErrorCalculatorComponent:public IVTComponent
{
	DECLARE_RTTI;
protected:
	std::vector<CoupledJointComponent*> m_joints;
	std::vector<ContorlComponentErrorInfo> m_errorValues;
public:
	ErrorCalculatorComponent(game::GameEntityManager* mngr);
	virtual~ErrorCalculatorComponent();
	
	virtual bool InitComponent();

	virtual void Update(float dt);
	
};

DECLARE_GAMECOMPONENT_FACTORY(ErrorCalculatorComponent);


}
}

#endif
