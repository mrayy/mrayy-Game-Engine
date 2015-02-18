

#ifndef ___IMovable___
#define ___IMovable___



#include "mstring.h"
#include "IMovable.h"

#include "GCPtr.h"

#include "IObject.h"

namespace mray
{
namespace animation
{
	class TransformationAnimationTrack;
	class AnimationController;
}
namespace scene
{


	enum ETransformSpace
	{
		TS_Local,
		TS_World,
		TS_Parent
	};

	class ISceneVisitor;

MakeSharedPtrType(IMovable);

typedef std::list<IMovablePtr> MovableNodeList;
class MRAY_DLL IMovable:public IObject
{
	DECLARE_RTTI
protected:
	class AttachedAnimationTrack
	{
	public:
		animation::TransformationAnimationTrack* track;
		animation::AnimationController*controller;

		AttachedAnimationTrack():track(0),controller(0)
		{
		}
		AttachedAnimationTrack(animation::TransformationAnimationTrack* t,animation::AnimationController*c):track(t),controller(c)
		{
		}
	};

	std::list<AttachedAnimationTrack> m_attachedTracks;
	bool m_isAnimationManual;

	virtual void _UpdateAnimationTrackState();

	
public:

	IMovable();
	virtual~IMovable();

	virtual const core::string&getNodeName()const=0;
	virtual bool setNodeName(const core::string&name)=0;

	virtual void removeChild(IMovable* elem,bool parentSpace=false)=0;
	virtual void addChild(IMovableCRef elem,bool parentSpace=false)=0;
	virtual void removeFromParent(bool parentSpace=false)=0;
	virtual const MovableNodeList& getChildren()=0;

	virtual const uint& getID()=0;
	virtual bool setID(const uint& id)=0;

	virtual IMovable* getParent()=0;
	virtual void setParent(IMovable*p)=0;
	virtual bool NeedChildUpdate()=0;

	virtual void SetManualAnimation(bool m);
	bool IsManualAnimation();

	virtual void AttachAnimationTrack(animation::TransformationAnimationTrack* track,animation::AnimationController*controller);
	virtual void DeattachAnimationTrack(animation::TransformationAnimationTrack* track);

	virtual math::vector3d getAbsolutePosition()const=0;
	virtual math::quaternion getAbsoluteOrintation()const=0;
	virtual math::vector3d getAbsoluteScale()const=0;
	virtual const math::vector3d& getPosition()const=0;
	virtual const math::quaternion& getOrintation()const=0;
	virtual const math::vector3d& getScale()const=0;
	
	virtual void updateAbsoluteTransformation()=0;
	virtual math::matrix4x4&getAbsoluteTransformation()=0;
	virtual const math::matrix4x4&getPrevAbsoluteTransformation()const=0;
	virtual const math::matrix4x4&getAbsoluteTransformation()const=0;
	virtual const math::matrix4x4&getRelativeTransformation()const=0;

	virtual bool setPosition(const math::vector3d& v)=0;
	virtual bool setOrintation(const math::quaternion& v)=0;
	virtual bool setScale(const math::vector3d& v)=0;

	virtual const math::box3d& getBoundingBox()=0;
	virtual math::box3d getTransformedBoundingBox()=0;

	virtual void preRender();
	virtual void update(float dt);


	virtual bool rotate(float angle,const math::vector3d& axis,ETransformSpace space);
	virtual bool rotate(const math::quaternion &q,ETransformSpace space);

	virtual bool pitch(float angle,ETransformSpace space);
	virtual bool yaw(float angle,ETransformSpace space);
	virtual bool roll(float angle,ETransformSpace space);

	virtual bool translate(const math::vector3d& p,ETransformSpace space);

//	virtual core::IteratorPair<NodeList> getChildren()=0;

//	IMovable* getElementById(int id,bool searchChildren=0);


	virtual void OnVisit(ISceneVisitor*visitor);


};



}
}


#endif


