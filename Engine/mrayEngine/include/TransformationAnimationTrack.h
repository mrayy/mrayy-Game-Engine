

#ifndef ___TransformationAnimationTrack___
#define ___TransformationAnimationTrack___

#include "KeyAnimationTrack.h"

#include "GCPtr.h"
#include "TransformKeyFrame.h"
#include "CSpline.h"
#include "QuaternionSpline.h"

namespace mray{
namespace scene{
	class IMovable;
}
namespace animation{
	
class MRAY_DLL TransformationAnimationTrack:public KeyAnimationTrack
{
	DECLARE_RTTI;
protected:

	math::vector3d m_resultPos;
	math::vector3d m_resultScale;
	math::quaternion m_resultOrintation;

	bool m_needRecalcSpline;
	math::CSpline<math::vector3d> m_translateSpline;
	math::CSpline<math::vector3d> m_scaleSpline;

	virtual void recalcSpline();
public:
	typedef TransformKeyFrame KeyType;

	TransformationAnimationTrack(const core::string& name,int affectedId,ulong catagoryBits,ulong priority,AnimationTrackGroup*owner);
	virtual~TransformationAnimationTrack();

	virtual void addKeyFrame(IKeyFrame*kf);
	IKeyFrame* createKeyFrame(float time,const math::vector3d& pos,const math::vector3d& scale,const math::quaternion& orintation);

	virtual void interpolate(float time,TransformKeyFrame&result,bool useSplineInterpolation=false);

	virtual bool applyOnObject(float time,scene::IMovable*object,int id,float weight,bool useSplineInterpolation=false);

	virtual IAnimationTrack *duplicate();
	virtual void copy(IAnimationTrack* o);
	virtual void loadKeyFrame(xml::XMLElement*elem);

};

MakeSharedPtrType(TransformationAnimationTrack)


}
}



#endif

