


#ifndef ___IAnimationTrack___
#define ___IAnimationTrack___


#include "GCPtr.h"
#include "mString.h"
#include "IObject.h"
#include "XMLElement.h"

#include "TypedProperty.h"

namespace mray{
namespace animation
{
	class IKeyFrame;

class MRAY_DLL IAnimationTrack:public IObject
{

	DECLARE_RTTI;
protected:

	int m_affectedNodeID;

	// catagory bits used for identifing the track
	// it's used to mix this track with other tracks
	ulong m_catagoryBits;
	// when mixing the animation with other animations, this field used for the priority
	ulong m_priority;

	core::string m_name;

	float m_start;
	float m_end;


public:

	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);

public:

	IAnimationTrack(const core::string& name,int affectedID,ulong catagoryBits,ulong priority);
	virtual~IAnimationTrack();

	core::string getName() { return m_name; }
	bool setName(const core::string &val) { m_name = val; return true;}

	void setCatagoryBits(ulong bits){ m_catagoryBits = bits;}
	ulong getCatagoryBits()const{ return m_catagoryBits ;}

	void setPriority(ulong p){ m_priority = p;}
	ulong getPriority()const{ return m_priority ;}

	void setAffectedNodeID(int id){m_affectedNodeID=id;}
	int getAffectedNodeID(){return m_affectedNodeID;}

	float getStart();
	float getEnd();
	float getLength();

	virtual void addKeyFrame(IKeyFrame*kf);
	virtual int getKeysframsCount()=0;


	virtual void removeKeyFrame(int index)=0;
	virtual void removeAllKeyFrames()=0;


	virtual float getKeyAtTime(float time,IKeyFrame**kf1,IKeyFrame**kf2)=0;

	virtual IAnimationTrack* duplicate()=0;
	virtual void copy(IAnimationTrack* o);

	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* elem);
};

MakeSharedPtrType(IAnimationTrack)

}
}



#endif



