
/********************************************************************
	created:	2009/03/12
	created:	12:3:2009   22:49
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\KeyAnimationTrack.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	KeyAnimationTrack
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___KeyAnimationTrack___
#define ___KeyAnimationTrack___


#include "IAnimationTrack.h"
#include "AnimationTypes.h"


namespace mray{
namespace animation{

	class AnimationTrackGroup;

class MRAY_DLL KeyAnimationTrack:public IAnimationTrack
{
	DECLARE_RTTI;
public:
	class KeyFrameTicket
	{
	protected:
		IKeyFrame*m_keyFrame;
	public:
		KeyFrameTicket();
		KeyFrameTicket(IKeyFrame*kf);
		virtual~KeyFrameTicket();
		IKeyFrame*getKeyFrame();
		const IKeyFrame*getKeyFrame()const;
		float getTime()const;
		bool operator<(const KeyFrameTicket&o)const;
	};

	typedef std::vector<KeyFrameTicket> KeyFrameList;
protected:
	KeyFrameList m_keyFrames;

	AnimationTrackGroup* m_owner;

	virtual void loadKeyFrame(xml::XMLElement*elem)=0;

public:

	KeyAnimationTrack(const core::string& name,int affectedID,ulong catagoryBits,ulong priority,AnimationTrackGroup*owner=0);
	virtual~KeyAnimationTrack();

	void setOwner(AnimationTrackGroup* owner);
	AnimationTrackGroup* getOwner();

	virtual void addKeyFrame(IKeyFrame*kf);
	virtual int getKeysframsCount();
	virtual const KeyFrameList& getKeyFrames()const{return m_keyFrames;}

	virtual void removeKeyFrame(int index);
	virtual void removeAllKeyFrames();

	virtual float getKeyAtTime(float time,IKeyFrame**kf1,IKeyFrame**kf2);

	virtual void copy(IAnimationTrack* o);


	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement*  exportXMLSettings(xml::XMLElement* elem);
};

}
}


#endif //___KeyAnimationTrack___
