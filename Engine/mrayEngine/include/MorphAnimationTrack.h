
/********************************************************************
	created:	2009/07/24
	created:	24:7:2009   1:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MorphAnimationTrack.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MorphAnimationTrack
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MorphAnimationTrack___
#define ___MorphAnimationTrack___

#include "KeyAnimationTrack.h"
#include "IKeyFrame.h"

namespace mray{
	namespace scene
	{
		class IMeshBuffer;
		class IMorphAnimator;
	}
namespace animation{

class MRAY_DLL MorphAnimationTrack:public KeyAnimationTrack
{
private:
protected:

	class MRAY_DLL MorphKeyFrame:public IKeyFrame
	{
		scene::IMeshBuffer* m_value;
	public:
		MorphKeyFrame():m_value(0){
		}

		MorphKeyFrame(float time,scene::IMeshBuffer* value)
			:IKeyFrame(time),m_value(value)
		{
		}

		scene::IMeshBuffer* getValue()const{
			return m_value;
		}
		void setValue(scene::IMeshBuffer* v){
			m_value=v;
		}

		virtual IKeyFrame*duplicate()const{
			return new MorphKeyFrame(getTime(),m_value);
		}
		virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
		virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
	};

	scene::IMeshBuffer* m_res1;
	scene::IMeshBuffer* m_res2;


	scene::IMorphAnimator* m_anim;

	float m_resTime;
	virtual void recalcSpline(){}

	virtual void loadKeyFrame(xml::XMLElement*elem);
public:
	MorphAnimationTrack(scene::IMorphAnimator* anim,const core::string& name,ulong catagoryBits,ulong priority);
	virtual~MorphAnimationTrack();

	void setAnimator(scene::IMorphAnimator* anim);
	scene::IMorphAnimator* getAnimator();

	IKeyFrame* createKeyFrame(float time,scene::IMeshBuffer* b);
	virtual void animate(float time,float weight);
	virtual uint getTransformationType();

	virtual bool apply(float time,float weight);
	virtual IAnimationTrack* duplicate();
	virtual void copy(IAnimationTrack* o);
};

MakeSharedPtrType(MorphAnimationTrack)

}
}

#endif //___MorphAnimationTrack___
