


/********************************************************************
	created:	2012/02/23
	created:	23:2:2012   12:10
	filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\ModelCollisionComponent.h
	file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
	file base:	ModelCollisionComponent
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __ModelCollisionComponent__
#define __ModelCollisionComponent__

#include "ICollisionComponent.h"
#include "MeshRenderableNode.h"
#include "PhysicalShapes.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL ModelCollisionComponent:public ICollisionComponent
{
	DECLARE_RTTI
private:
protected:
	core::string m_modelPath;
	math::vector3d m_scale;
	bool m_convex;
	physics::IPhysicalShapeDesc* m_shapeDesc;
	GCPtr<scene::MeshRenderableNode> m_CollNode;
	void _OnTargetNodeChanged();
public:
	DECLARE_PROPERTY_TYPE(ModelPath,core::string,);
	DECLARE_PROPERTY_TYPE(IsConvex,bool,);
public:
	ModelCollisionComponent(GameEntityManager*mngr);
	ModelCollisionComponent();
	virtual~ModelCollisionComponent();

	virtual bool InitComponent();

	virtual bool CopyShapeDesc(physics::IPhysicalShapeDesc*d);
	virtual physics::IPhysicalShapeDesc* GetShapeDesc();

	bool SetModelPath(const core::string& path);
	const core::string& GetModelPath()const;

	bool SetConvex(bool c);
	bool GetConvex()const;

};


DECLARE_GAMECOMPONENT_FACTORY(ModelCollisionComponent);

}
}

#endif
