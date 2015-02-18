


/********************************************************************
	created:	2012/02/23
	created:	23:2:2012   10:57
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\ModelComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	ModelComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ModelComponent__
#define __ModelComponent__

#include "IModelComponent.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL ModelComponent:public IModelComponent
{
	DECLARE_RTTI;
private:
protected:
	core::string m_path;

	GCPtr<scene::MeshRenderableNode> m_model;
public:
	DECLARE_PROPERTY_TYPE(ModelPath,core::string,);
public:
	ModelComponent(GameEntityManager*mngr);
	ModelComponent();
	virtual~ModelComponent();

	virtual bool InitComponent();


	bool SetModelPath(const core::string& path);
	const core::string& GetModelPath()const;

	const GCPtr<scene::MeshRenderableNode>& GetModel();

	virtual bool SetEnabled(bool enabled);

	virtual IObject* GetAttachedObject();

};
DECLARE_GAMECOMPONENT_FACTORY(ModelComponent);


}
}

#endif

