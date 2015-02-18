
/********************************************************************
	created:	2009/02/06
	created:	6:2:2009   14:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IPhysicContactListener2D.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IPhysicContactListener2D
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IPhysicContactListener2D___
#define ___IPhysicContactListener2D___



namespace mray{
namespace physics{

	class IPhysicalNode2D;

/// This is used to report contact points.
class IContactPoint2D{
public:
	virtual physics::IPhysicalNode2D* getShape1()const=0;
	virtual physics::IPhysicalNode2D* getShape2()const=0;
	virtual math::vector2d getPosition()const=0;
	virtual math::vector2d getVelocity()const=0;
	virtual math::vector2d getNormal()const=0;
	virtual float getSeparation()const=0;
	virtual float getFriction()const=0;
	virtual float getRestitution()const=0;
};

/// This is used to report contact point results.
class IContactResult2D{
public:
	virtual physics::IPhysicalNode2D* getShape1()const=0;
	virtual physics::IPhysicalNode2D* getShape2()const=0;
	virtual math::vector2d getPosition()const=0;
	virtual math::vector2d getNormal()const=0;
	virtual float getNormalImpulse()const=0;
	virtual float getTangentImpulse()const=0;
};

class IPhysicContactListener2D
{
public:
	/// Called when a contact point is added. This includes the geometry
	/// and the forces.
	virtual void onAdd(const IContactPoint2D*point)=0;
	/// Called when a contact point persists. This includes the geometry
	/// and the forces.
	virtual void onPresist(const IContactPoint2D*point)=0;
	/// Called when a contact point is removed. This includes the last
	/// computed geometry and forces.
	virtual void onRemove(const IContactPoint2D*point)=0;
	/// Called after a contact point is solved.
	virtual void onResult(const IContactResult2D*point)=0;
};

}
}


#endif //___IPhysicContactListener2D___
