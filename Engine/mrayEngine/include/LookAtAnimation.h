
/********************************************************************
	created:	2009/03/09
	created:	9:3:2009   23:18
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\LookAtAnimation.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	LookAtAnimation
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___LookAtAnimation___
#define ___LookAtAnimation___

#include "IAnimationTrack.h"

namespace mray{
namespace animation{


class MRAY_DLL LookAtAnimation:public IAnimationTrack
{
protected:
	virtual void recalcSpline(){}
	scene::IMovable* m_node;
	float m_damping;
	float m_dt;
public:
	LookAtAnimation(const core::string& name,scene::IMovable* node,float damping,int affectedID,ulong catagoryBits,ulong priority);
	virtual~LookAtAnimation();
	float getDamping();
	void setDamping(float d);
	virtual void animate(float time,float weight);
	virtual void update(float dt);
	virtual bool applyOnObject(scene::IMovable*object,int id);

	virtual void addKeyFrame(IKeyFrame*kf){}
	virtual int getKeysframsCount(){return 0;}

	virtual void removeKeyFrame(int index){}
	virtual void removeAllKeyFrames(){}

	virtual float getKeyAtTime(float time,IKeyFrame**kf1,IKeyFrame**kf2){
		*kf1=*kf2=0;
		return 0;
	}

	virtual IAnimationTrack* duplicate();
	virtual void copy(IAnimationTrack* o);

};

}
}


#endif //___LookAtAnimation___
