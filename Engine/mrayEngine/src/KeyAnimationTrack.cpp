#include "stdafx.h"

#include "KeyAnimationTrack.h"
#include "EnumManager.h"


#include "AnimationTrackGroup.h"
#include "IKeyFrame.h"

#include <algorithm>

namespace mray{
namespace animation{

		KeyAnimationTrack::KeyFrameTicket::KeyFrameTicket(){
			m_keyFrame=0;
		}
		KeyAnimationTrack::KeyFrameTicket::KeyFrameTicket(IKeyFrame*kf){
			m_keyFrame=kf;
		}
		KeyAnimationTrack::KeyFrameTicket::~KeyFrameTicket(){
			m_keyFrame=0;
		}
		IKeyFrame* KeyAnimationTrack::KeyFrameTicket::getKeyFrame(){
			return m_keyFrame;
		}
		const IKeyFrame* KeyAnimationTrack::KeyFrameTicket::getKeyFrame()const 
		{
			return m_keyFrame;
		}
		float KeyAnimationTrack::KeyFrameTicket::getTime()const{
			return m_keyFrame->getTime();
		}
		bool KeyAnimationTrack::KeyFrameTicket::operator<(const KeyAnimationTrack::KeyFrameTicket&o)const{
			return getTime()<o.getTime();
		}

	
KeyAnimationTrack::KeyAnimationTrack(const core::string& name,int affectedID,ulong catagoryBits,ulong priority,AnimationTrackGroup*owner)
	: IAnimationTrack(name,affectedID,catagoryBits,priority)
{
	m_owner=owner;

}



KeyAnimationTrack::~KeyAnimationTrack(){
	removeAllKeyFrames();
}

void KeyAnimationTrack::setOwner(AnimationTrackGroup* owner)
{
	m_owner=owner;
}
AnimationTrackGroup* KeyAnimationTrack::getOwner()
{
	return m_owner;
}

void KeyAnimationTrack::addKeyFrame(IKeyFrame*kf){
	IAnimationTrack::addKeyFrame(kf);
	m_keyFrames.push_back(kf);
	if(m_owner)
		m_owner->_OnKeyFrameAdded(this);
}
int KeyAnimationTrack::getKeysframsCount()
{
	return m_keyFrames.size();
}

void KeyAnimationTrack::removeKeyFrame(int index){
	if(index>=m_keyFrames.size())return;
	delete m_keyFrames[index].getKeyFrame();
	KeyFrameList::iterator it=m_keyFrames.begin();
	std::advance(it,index);
	m_keyFrames.erase(it);
}
void KeyAnimationTrack::removeAllKeyFrames()
{
	for(int i=0;i<m_keyFrames.size();++i)
		delete m_keyFrames[i].getKeyFrame();
	m_keyFrames.clear();
}



float KeyAnimationTrack::getKeyAtTime(float time,IKeyFrame**kf1,IKeyFrame**kf2)
{
	*kf1=0;
	*kf2=0;
	if(!m_keyFrames.size())
	{
		return 0;
	}
	if(m_keyFrames[0].getTime()>=time)
	{
		*kf1=m_keyFrames[0].getKeyFrame();
		return 0;
	}
	if(m_keyFrames[m_keyFrames.size()-1].getTime()<=time)
	{
		*kf1=m_keyFrames[m_keyFrames.size()-1].getKeyFrame();
		return 0;
	}
	std::sort(m_keyFrames.begin(),m_keyFrames.end());


	int left=1;
	int right=m_keyFrames.size()-2;
	IKeyFrame*lastKf=0;
	float t1,t2;

	int m;
	KeyFrameTicket* pdata= &m_keyFrames[0];

	do
	{
		m = (left+right)>>1;

		if (time <=pdata[m].getTime())
		{
			if(pdata[m-1].getTime()<=time)
			{
				--m;
				break;
			}else
				right = m - 1;
		}
		else{
			if(pdata[m+1].getTime()>=time)
			{
				break;
			}else
				left = m + 1;
		}

	} while( left<=right);

	*kf1=pdata[m].getKeyFrame();
	*kf2=pdata[m+1].getKeyFrame();
	t1=pdata[m].getTime();
	t2=pdata[m+1].getTime();
	return (time-t1)/(t2-t1);
	/*
	for(int i=0;i<size;++i){
		t1=m_keyFrames[i].getTime();
		if(t1<time){
			lastKf=m_keyFrames[i].getKeyFrame();
		}else{
			if(lastKf && t1>time){
				t2=t1;
				t1=lastKf->getTime();
				*kf1=lastKf;
				*kf2=m_keyFrames[i].getKeyFrame();
				return (time-t1)/(t2-t1);
			}
			else{
				*kf1=m_keyFrames[i].getKeyFrame();
				if(i==0 || i==size-1){
					return 0;
				}else{
					t2=m_keyFrames[i+1].getTime();
					*kf2=m_keyFrames[i+1].getKeyFrame();
					if(t1==t2)
						return 0;
					return (time-t1)/(t2-t1);
				}
			}
		}
	}
	if(size>0){
		*kf1=m_keyFrames[0].getKeyFrame();
		return 0;
	}*/
	return 0;
}


void KeyAnimationTrack::copy(IAnimationTrack* o){
	IAnimationTrack::copy(o);
	KeyAnimationTrack* ao=dynamic_cast<KeyAnimationTrack*>(o);
	if(!ao)
		return;

	ao->m_keyFrames.reserve(m_keyFrames.size());

	for(int i=0;i<m_keyFrames.size();i++){
		ao->m_keyFrames[i]=m_keyFrames[i];
	}
}

xml::XMLElement* KeyAnimationTrack::loadXMLSettings(xml::XMLElement* root)
{
	xml::XMLElement* elem=IAnimationTrack::loadXMLSettings(root);
	if(!elem)return 0;

	xml::XMLElement* keyFrames=elem->getSubElement(mT("KeyFrames"));
	if(keyFrames){
		xml::xmlSubElementsMapIT it= keyFrames->getElementsBegin();
		xml::xmlSubElementsMapIT end= keyFrames->getElementsEnd();
		for(;it!=end;++it){
			if((*it)->GetType()!=xml::ENT_Element)continue;
			xml::XMLElement*e=dynamic_cast<xml::XMLElement*>(*it);
			if(e->getName().equals_ignore_case(mT("KeyFrame"))){
				loadKeyFrame(e);
			}
		}
	}
	return elem;
}
xml::XMLElement*  KeyAnimationTrack::exportXMLSettings(xml::XMLElement* elem){
	xml::XMLElement* e=IAnimationTrack::exportXMLSettings(elem);
	{
		xml::XMLElement* keyFrames=new xml::XMLElement(mT("KeyFrames"));
		e->addSubElement(keyFrames);
		for(int i=0;i<m_keyFrames.size();i++){
			m_keyFrames[i].getKeyFrame()->exportXMLSettings(keyFrames);
		}
	}
	return e;
}


}
}