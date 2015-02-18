
/********************************************************************
	created:	2009/03/29
	created:	29:3:2009   19:37
	filename: 	i:\Programing\GameEngine\mrayEngine\EaglesProject\GameEntity.h
	file path:	i:\Programing\GameEngine\mrayEngine\EaglesProject
	file base:	GameEntity
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	base class for game entitys
*********************************************************************/

#ifndef ___GameEntity___
#define ___GameEntity___

#include <CPropertieSet.h>
#include <ISceneNode.h>
#include <mstring.h>
#include <Point3d.h>
#include <quaternion.h>
#include <C_List.h>

namespace mray{

class GameEntity:public PropertieObject
{
private:
protected:
	virtual void fillProperties(){
		addPropertie(&ID);
		addPropertie(&Name);
	}
public:

	uint m_id;
	core::string m_name;

	rwProperty<GameEntity,uint> ID;
	rwProperty<GameEntity,core::string> Name;

	ulong m_collisionGroup;
	bool m_passable;

	bool m_exploded;
public:
	GameEntity():
		ID(this,&GameEntity::setId,&GameEntity::getId,EPT_INT,mT("ID")),
		Name(this,&GameEntity::setName,&GameEntity::getName,EPT_INT,mT("Name")),
		m_collisionGroup(0),m_passable(0),m_exploded(0)
	{
		fillProperties();
	}
	virtual~GameEntity(){};

	virtual uint& getId() {return m_id;}
	virtual void setId(const uint &val){m_id=val;}

	virtual core::string& getName()  {return m_name;}
	virtual void setName(const core::string &val) {m_name=val;}


	virtual void setPosition(const math::vector3d&p)=0;
	virtual void setRotation(const math::quaternion&p)=0;
	virtual void setScale(const math::vector3d&p)=0;

	virtual math::vector3d getPosition()=0;
	virtual math::quaternion getRotation()=0;
	virtual math::vector3d getScale()=0;

	virtual GCPtr<scene::ISceneNode> getShape()=0;

	virtual math::box3d getBoundingBox()=0;

	virtual bool isStaticEntity()=0;

	virtual void update(float dt)=0;

	virtual void hasCollide()=0;

	bool isDestroyed(){return m_exploded;}

	virtual void setCollisionGroup(ulong g){
		m_collisionGroup=g;
	}
	virtual ulong getCollisionGroup(){
		return m_collisionGroup;
	}
	bool isPassable(){
		return m_passable || isDestroyed();
	}
};

typedef std::list<GCPtr<GameEntity>> EntityList;

}


#endif //___GameEntity___
