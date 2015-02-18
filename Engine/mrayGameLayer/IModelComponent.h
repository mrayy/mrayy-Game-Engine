
/********************************************************************
	created:	2012/09/19
	created:	19:9:2012   12:37
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\IModelComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	IModelComponent
	file ext:	h
	author:		MHD YAMEN SARAIJI
	
	purpose:	
*********************************************************************/
#ifndef ___IModelComponent___
#define ___IModelComponent___

#include "IGameComponent.h"
#include "MeshRenderableNode.h"


namespace mray
{
namespace game
{

class IModelComponent:public IGameComponent
{
	DECLARE_RTTI;
protected:

public:
	IModelComponent(){}
	virtual~IModelComponent(){}

	virtual const GCPtr<scene::MeshRenderableNode>& GetModel()=0;

	
};

}
}


#endif

