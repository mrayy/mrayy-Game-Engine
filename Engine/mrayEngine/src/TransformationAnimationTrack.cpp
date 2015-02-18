#include "stdafx.h"
 
#include "TransformationAnimationTrack.h"
#include "IMovable.h"

namespace mray{
namespace animation{




TransformationAnimationTrack::TransformationAnimationTrack(const core::string& name,int affectedId,ulong catagoryBits,ulong priority,AnimationTrackGroup*owner):
KeyAnimationTrack(name,affectedId,catagoryBits,priority,owner),m_translateSpline(0,0,4),m_scaleSpline(0,0,4)
{
	m_needRecalcSpline=false;
}
TransformationAnimationTrack::~TransformationAnimationTrack(){}

void TransformationAnimationTrack::recalcSpline()
{
	m_translateSpline.clearPoints();
	m_scaleSpline.clearPoints();
	for(int i=0;i<m_keyFrames.size();++i){
		TransformKeyFrame * kf=(TransformKeyFrame*)(m_keyFrames[i].getKeyFrame());
		m_translateSpline.addPoint(kf->getPos());
		m_scaleSpline.addPoint(kf->getScale());
	}
	m_needRecalcSpline=false;

	m_translateSpline.createCurve();
	m_scaleSpline.createCurve();
}


IKeyFrame* TransformationAnimationTrack::createKeyFrame(float time,const math::vector3d& pos,const math::vector3d& scale,const math::quaternion& orintation){
	TransformKeyFrame* kf=new TransformKeyFrame(time,pos,scale,orintation);
	addKeyFrame(kf);
	return kf;
}
void TransformationAnimationTrack::addKeyFrame(IKeyFrame*kf)
{
	KeyAnimationTrack::addKeyFrame(kf);
	m_needRecalcSpline=true;
}
math::quaternion NLerp(const math::quaternion&rkP,const math::quaternion&rkQ,float t,bool shortestPath=true)
{

	math::quaternion result;
	float fCos = rkP.dotProduct(rkQ);
	if (fCos < 0.0f && shortestPath)
	{
		result = rkP +  ((-rkQ) - rkP) * t;
	}
	else
	{
		result = rkP + (rkQ - rkP) * t;
	}
	result.Normalize();
	return result;
}

bool g_useShortPath=false;
bool g_slerpInterpolation=true;

void TransformationAnimationTrack::interpolate(float time,TransformKeyFrame&result,bool useSplineInterpolation)
{
	IKeyFrame*kf1,*kf2;
	TransformKeyFrame*tkf1,*tkf2;

	float res=getKeyAtTime(time,&kf1,&kf2);

	tkf1=((TransformKeyFrame*)kf1);
	if(res==0){
		result.setPos(tkf1->getPos());
		result.setScale(tkf1->getScale());
		result.setOrintation(tkf1->getOrintation());
	}
	else 
	{
		tkf2=((TransformKeyFrame*)kf2);
		result.setPos(math::lerp(tkf1->getPos(),tkf2->getPos(),res));
		result.setScale(math::lerp(tkf1->getScale(),tkf2->getScale(),res));
		if(useSplineInterpolation){
			math::quaternion q;
			q.Slerp(tkf1->getOrintation(),tkf2->getOrintation(),res,g_useShortPath);
			result.setOrintation(q);
		}
		else
			result.setOrintation(NLerp(tkf1->getOrintation(),tkf2->getOrintation(),res,g_useShortPath));
	}
	/*
 	m_resultPos*=weight;
	m_resultScale*=weight;
	m_resultOrintation=m_resultOrintation*weight;
	
	if(g_slerpInterpolation)
		m_resultOrintation.Slerp(math::quaternion::Identity,m_resultOrintation,weight,g_useShortPath);
	else
 		m_resultOrintation=NLerp(math::quaternion::Identity,m_resultOrintation,weight,g_useShortPath);
	*/
}

bool TransformationAnimationTrack::applyOnObject(float time,scene::IMovable*object,int id,float weight,bool useSplineInterpolation)
{
	if(m_affectedNodeID!=id && id!=-1)return false;

	TransformKeyFrame kf;
	interpolate(time,kf,useSplineInterpolation);

	object->setPosition(kf.getPos()*weight);
	object->setScale(kf.getScale()*weight);
		//object->setOrintation(m_resultOrintation);

	math::quaternion q;
	q.Slerp(math::quaternion::Identity,kf.getOrintation(),weight);
 	object->setOrintation(q);
	return true;

}
void TransformationAnimationTrack::loadKeyFrame(xml::XMLElement*elem){
	IKeyFrame* kf= createKeyFrame(0,0,1,math::quaternion::Identity);
	kf->loadXMLSettings(elem);
}

IAnimationTrack* TransformationAnimationTrack::duplicate(){
	TransformationAnimationTrack* o=new TransformationAnimationTrack(m_name,m_affectedNodeID,m_catagoryBits,m_priority,m_owner);
	copy(o);
	return o;
}
void TransformationAnimationTrack::copy(IAnimationTrack* o){
	KeyAnimationTrack::copy(o);
	TransformationAnimationTrack* ao=dynamic_cast<TransformationAnimationTrack*>(o);
	if(!ao)
		return;
	ao->m_translateSpline.createCurve(m_translateSpline.getControlCurvePoints(),m_translateSpline.getControlPointsCount(),m_translateSpline.getCurveSubD());
	ao->m_scaleSpline.createCurve(m_scaleSpline.getControlCurvePoints(),m_scaleSpline.getControlPointsCount(),m_scaleSpline.getCurveSubD());
}

}
}


