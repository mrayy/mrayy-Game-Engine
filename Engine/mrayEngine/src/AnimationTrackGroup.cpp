#include "stdafx.h"

#include "IMovable.h"
#include "AnimationTrackGroup.h"

#include "ILogManager.h"
#include "DefaultObjects.h"
#include "AnimationLoggerSystem.h"


namespace mray{
namespace animation{

AnimationTrackGroup::AnimationTrackGroup(const core::string &name)
{

	m_name=name;

	m_start=0;
	m_end=1;

	fillProperties();
}
AnimationTrackGroup::~AnimationTrackGroup()
{
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		delete *p;
	}
	m_tracks.clear();
	m_playingTracks.clear();
}


void AnimationTrackGroup::fillProperties()
{
	CPropertieDictionary *dic=0;
	if(CreateDictionary(&dic))
	{
		dic->addPropertie(&PropertyTypeName::instance);
	}
}

void AnimationTrackGroup::refreshAffectIDMap()
{
	m_affectedIDMap.clear();
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		m_affectedIDMap[(*p)->getAffectedNodeID()]=(*p);
	}
}

void AnimationTrackGroup::addTrack(IAnimationTrack* track)
{
	m_tracks.push_back(track);
	if(track->getAffectedNodeID()!=-1)
	m_affectedIDMap[track->getAffectedNodeID()]=track;
}

void AnimationTrackGroup::removeTrack(IAnimationTrack* track)
{
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		if((*p)==track){
			m_tracks.erase(p);
			break;
		}
	}
	p=m_playingTracks.begin();
	end=m_playingTracks.end();
	for(;p!=end;++p)
	{
		if(*p==track){
			m_playingTracks.erase(p);
			break;
		}
	}
	m_affectedIDMap.erase(track->getAffectedNodeID());
}
void AnimationTrackGroup::removeTrack(const core::string&name)
{
	IAnimationTrack* track=0;
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		IAnimationTrackListIT it=p;
		if((*it)->getName().equals_ignore_case(name)){

			track=*it;
			m_affectedIDMap.erase((*it)->getAffectedNodeID());
			m_tracks.erase(it);
			break;
		}
	}
	p=m_playingTracks.begin();
	end=m_playingTracks.end();
	for(;p!=end;++p)
	{
		if((*p)->getName().equals_ignore_case(name)){
			m_playingTracks.erase(p);
			break;
		}
	}
	delete track;
}

IAnimationTrack* AnimationTrackGroup::getTrack(const core::string&name)
{
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		if((*p)->getName().equals_ignore_case(name))
			return *p;
	}
	return 0;
}
IAnimationTrack* AnimationTrackGroup::getTrackByAffectID(uint id)
{
	AffectedIDTrackMap::iterator it= m_affectedIDMap.find(id);
	if(it==m_affectedIDMap.end())
		return 0;
	return it->second;
}

// void AnimationTrackGroup::setCurrentTrack(const core::string&name)
// {
// 	m_currTrack=getTrack(name);
// }

void AnimationTrackGroup::reset(){

}

void AnimationTrackGroup::stopTrack(const core::string&name)
{
	IAnimationTrackList::iterator p=m_playingTracks.begin();
	IAnimationTrackList::iterator end=m_playingTracks.end();
	for(;p!=end;++p)
	{
		IAnimationTrack* t=*p;
		if(t->getName().equals_ignore_case(name)){
			m_playingTracks.erase(p);
			return;
		}
	}
}
void AnimationTrackGroup::playAll()
{
	IAnimationTrackList::iterator p=m_tracks.begin();
	IAnimationTrackList::iterator end=m_tracks.end();
	for(;p!=end;++p)
	{
		IAnimationTrack* t=*p;
		playTrack(t->getName());
	}
}
void AnimationTrackGroup::stopAll()
{
	m_playingTracks.clear();
}

// AI Game Wisdom "Useful Techniques - 4_Orkin"
void AnimationTrackGroup::playTrack(const core::string&name)
{
	IAnimationTrack* track=getTrack(name);
	ulong tBits=track->getCatagoryBits();
	ulong tPriority=track->getPriority();

	bool add=true;
	IAnimationTrackList::iterator p=m_playingTracks.begin();
	IAnimationTrackList::iterator end=m_playingTracks.end();
	for(;p!=end;++p)
	{
		IAnimationTrack* t=*p;
		if(t==track){
			add=false;
			break;
		}
		ulong cbits= t->getCatagoryBits();
		if(cbits & tBits){
			ulong priority=t->getPriority();
			if(cbits == tBits){
				if(priority>tPriority){
					add=false;
				}else{
					m_playingTracks.erase(p);
				}
				break;
			}else if(cbits<tBits && priority<tPriority){
				m_playingTracks.erase(p);
				break;
			}
		}
	}
	if(add){
		m_playingTracks.push_back(track);
	}
}
int AnimationTrackGroup::getTracksCount()
{
	return m_tracks.size();
}

const IAnimationTrackList& AnimationTrackGroup::getTracks()
{
	return m_tracks;
}
const IAnimationTrackList& AnimationTrackGroup::getPlayingTracks()
{
	return m_playingTracks;
}

void AnimationTrackGroup::clear()
{
	m_tracks.clear();
}

AnimationTrackGroup* AnimationTrackGroup::duplicate(bool contextOnly,bool playingTracks){
	AnimationTrackGroup* o=new AnimationTrackGroup(m_name);
	copy(o,contextOnly,playingTracks);
	return o;
}

void AnimationTrackGroup::copy(AnimationTrackGroup* o,bool contextOnly,bool playingTracks){
	IAnimationTrackList::iterator p=m_playingTracks.begin();
	IAnimationTrackList::iterator end=m_playingTracks.end();

	if(contextOnly){
		for(;p!=end;++p){
			o->addTrack(*p);
		}
	}else{
		for(;p!=end;++p){
			o->addTrack((*p)->duplicate());
		}
	}
	if(playingTracks){
		p=m_playingTracks.begin();
		end=m_playingTracks.end();
		for(;p!=end;++p){
			o->playTrack((*p)->getName());
		}
	}

	o->m_start=m_start;
	o->m_end=m_end;
	
}

xml::XMLElement* AnimationTrackGroup::loadXMLSettings(xml::XMLElement* elem)
{
	//xml::XMLElement* elem=root->getSubElement(mT("AnimationTrackGroup"));
	//if(!elem)return 0;

	xml::XMLAttribute*attr;
	attr=elem->getAttribute(mT("Name"));
	if(attr)
		m_name = attr->value;
	xml::xmlSubElementsMapIT it= elem->getElementsBegin();
	xml::xmlSubElementsMapIT end= elem->getElementsEnd();
	for(;it!=end;++it)
	{
		if((*it)->GetType()!=xml::ENT_Element)continue;
		xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
		if(e->getName().equals_ignore_case(mT("AnimationTrack"))){
			xml::XMLAttribute*attr= e->getAttribute(mT("type"));
			if(!attr){
				gAnimationLoggerSystem.log(mT("no type for AnimationTrack."),ELL_WARNING);
				continue;
			}
			IAnimationTrack* track=(IAnimationTrack*)DefaultObjects::getInstance().CreateObject(attr->value);
			if(track==0){
				gAnimationLoggerSystem.log(mT("Unkown AnimationTrack type: ")+attr->value,ELL_WARNING);
				continue;
			}
			track->loadXMLSettings(e);
			addTrack(track);
		}
	}
	return elem;
}
xml::XMLElement* AnimationTrackGroup::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement* e=new xml::XMLElement(mT("AnimationTrackGroup"));
	e->addAttribute(mT("Type"),getObjectType());
	e->addAttribute(mT("Name"),m_name);

	IAnimationTrackList::iterator it=m_tracks.begin();
	for(;it!=m_tracks.end();++it){
		(*it)->exportXMLSettings(e);
	}
	elem->addSubElement(e);
	return e;
}

//////////////////////////////////////////////////////////////////////////
void AnimationTrackGroup::_OnKeyFrameAdded(IAnimationTrack*track)
{
	m_start=math::Min(m_start,track->getStart());
	m_end=math::Max(m_end,track->getEnd());
}




//////////////////////////////////////////////////////////////////////////

IMPLEMENT_PROPERTY_TYPE_HEADER(Name,AnimationTrackGroup,string,mT("Name"),EPBT_Basic,mT("Track Name"),mT(""));
IMPLEMENT_PROPERTY_TYPE_GENERIC(Name,AnimationTrackGroup,core::string,setName,getName,,,false)



}
}


