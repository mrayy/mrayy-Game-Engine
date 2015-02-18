


/********************************************************************
created:	2012/02/23
created:	23:2:2012   12:10
filename: 	d:\Development\mrayEngine\Engine\mrayGameLayer\PrimitiveCollisionComponent.h
file path:	d:\Development\mrayEngine\Engine\mrayGameLayer
file base:	PrimitiveCollisionComponent
file ext:	h
author:		MHD Yamen Saraiji

purpose:	
*********************************************************************/


#ifndef __PrimitiveCollisionComponent__
#define __PrimitiveCollisionComponent__

#include "ICollisionComponent.h"
#include "MeshRenderableNode.h"
#include "PhysicalShapes.h"

namespace mray
{
namespace game
{

class MRAY_GAME_DLL PrimitiveCollisionComponent:public ICollisionComponent
{
	DECLARE_RTTI
private:
protected:
	physics::IPhysicalShapeDesc* m_shapeDesc;
	void _OnTargetNodeChanged();
public:
	PrimitiveCollisionComponent(GameEntityManager*mngr);
	PrimitiveCollisionComponent();
	virtual~PrimitiveCollisionComponent();

	virtual bool InitComponent();

	bool CopyShapeDesc(physics::IPhysicalShapeDesc*d);

	void SetShapeDesc(physics::IPhysicalShapeDesc*d);
	physics::IPhysicalShapeDesc* GetShapeDesc();


};


DECLARE_GAMECOMPONENT_FACTORY(PrimitiveCollisionComponent);

}
}

#endif
