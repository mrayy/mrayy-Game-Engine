

#include "StdAfx.h"
#include "IVTComponent.h"



namespace mray
{
namespace VT
{



	game::IGameComponent* IVTComponent::RetriveSubComponent(const core::string& type,const core::string& name)
	{
		return game::IGameComponent::RetriveComponent(this,type,name);
	}
	IVTComponent::IVTComponent()
	{
	}
	IVTComponent::~IVTComponent()
	{

	}

}
}

