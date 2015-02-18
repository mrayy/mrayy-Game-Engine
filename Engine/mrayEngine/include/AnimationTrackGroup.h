
/********************************************************************
	created:	2008/12/26
	created:	26:12:2008   22:37
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\AnimationTrackGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	AnimationTrackGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___AnimationTrackGroup___
#define ___AnimationTrackGroup___

#include "IAnimationTrack.h"

#include "IObject.h"



namespace mray{
namespace animation{
	
typedef std::list<IAnimationTrack*> IAnimationTrackList;
typedef IAnimationTrackList::iterator IAnimationTrackListIT;

class MRAY_DLL AnimationTrackGroup:public IObject
{
	DECLARE_RTTI;
protected:

	virtual void fillProperties();

	core::string m_name;

	typedef std::map<uint,IAnimationTrack*> AffectedIDTrackMap;

	IAnimationTrackList m_tracks;
	AffectedIDTrackMap m_affectedIDMap;
	IAnimationTrackList m_playingTracks;
	float m_start;
	float m_end;

	//	GCPtr<IAnimationTrack> m_currTrack;
public:
	DECLARE_PROPERTY_TYPE(Name,core::string,MRAY_DLL);
public:


	AnimationTrackGroup(const core::string &name);
	virtual~AnimationTrackGroup();

	float getStart(){return m_start;}
	float getEnd(){return m_end;}

	bool setName(const core::string& w){m_name=w;return true;}
	const core::string& getName(){return m_name;}

	void refreshAffectIDMap();

	virtual void addTrack(IAnimationTrack* track);

	void removeTrack(IAnimationTrack* track);
	void removeTrack(const core::string&name);

	IAnimationTrack* getTrack(const core::string&name);
	IAnimationTrack* getTrackByAffectID(uint id);

	//virtual void applyOnNode(scene::IMovable* node,int id);

	virtual void reset();

// 	void setCurrentTrack(const core::string&name);
// 	GCPtr<IAnimationTrack> getCurrentTrack();

	virtual void playAll();
	virtual void stopAll();

	virtual void playTrack(const core::string&name);
	virtual void stopTrack(const core::string&name);
	
	int getTracksCount();

	const IAnimationTrackList& getTracks();
	const IAnimationTrackList& getPlayingTracks();


// 	AnimationTrackListIT begin();
// 	AnimationTrackListIT end();

	void clear();

	virtual AnimationTrackGroup* duplicate(bool contextOnly,bool playingTracks);
	virtual void copy(AnimationTrackGroup* other,bool contextOnly,bool playingTracks);


	virtual xml::XMLElement* loadXMLSettings(xml::XMLElement* elem);
	virtual xml::XMLElement* exportXMLSettings(xml::XMLElement* elem);


	//////////////////////////////////////////////////////////////////////////
	void _OnKeyFrameAdded(IAnimationTrack*track);
};

}
}


#endif //___AnimationTrackGroup___


