
/********************************************************************
	created:	2009/03/31
	created:	31:3:2009   23:32
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\StaticEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	StaticEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___StaticEntity___
#define ___StaticEntity___

#include "GameEntity.h"
#include <ISceneNode.h>

namespace mray{
namespace gameMod{

class StaticEntity:public GameEntity
{
private:
protected:

	GCPtr<scene::ISceneNode> m_shape;
public:
	StaticEntity(GCPtr<scene::ISceneNode> shape);
	virtual~StaticEntity();

	virtual void setPosition(const math::vector3d&p);
	virtual void setRotation(const math::quaternion&p);
	virtual void setScale(const math::vector3d&p);

	virtual math::vector3d getPosition();
	virtual math::quaternion getRotation();
	virtual math::vector3d getScale();

	virtual GCPtr<scene::ISceneNode> getShape(){
		return m_shape;
	}
	virtual void update(float dt);

	virtual void hasCollide(){}

	virtual math::box3d getBoundingBox();

	virtual bool isStaticEntity(){return true;}

};

}
}


#endif //___StaticEntity___
