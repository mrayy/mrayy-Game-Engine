
#include "stdafx.h"
#include "MayaNode.h"
#include "ExporterParams.h"
#include "MayaAnimationTrack.h"
#include "TransformationAnimationTrack.h"
#include "TransAnimationGroup.h"
#include "NodeTypeMap.h"
#include "IComponentData.h"



namespace mray
{
namespace exporter
{

MayaNode::~MayaNode()
{
	for(int i=0;i<m_clips.size();++i)
		delete m_clips[i];


	for(int i=0;i<m_children.size();++i)
	{
		delete m_children[i];
	}

	delete m_compData;

}

MFnAnimCurve* MayaNode::readAnimCurve(MPlug& plug,ExporterParams& params)
{
	MPlugArray srcplugarray;
	MFnAnimCurve* animCurve = NULL;
	MStatus stat;

	if (plug.isConnected() && params.exportAnimCurves)
	{
		plug.connectedTo(srcplugarray,true,false,&stat);
		for (int i=0; i < srcplugarray.length(); i++)
		{
			if (srcplugarray[i].node().hasFn(MFn::kAnimCurve))
			{
				animCurve = new MFnAnimCurve(srcplugarray[i].node());
				continue;
			}
			else if (i == srcplugarray.length()-1)
			{
				std::cout << "Invalid link to translateX attribute\n";
				return 0;
			}
		}
		std::cout<<"Found animation: "<<animCurve->name().asChar() <<" \n";
		std::cout.flush();

		return animCurve;
		//m_params.outCameras << "anim " << animCurve->name().asChar() << "\n";
	}
	return 0;
}

MayaNode::AnimationClip* MayaNode::loadAnimationClip(MDagPath& dPath,AnimationTrackDesc& track)
{
	AnimationClip* ret=new AnimationClip();
	ret->name=track.name.asChar();
	ret->pos=new MayaAnimationTrack();
	ret->rot=new MayaAnimationTrack();
	ret->rot->trackType=MayaAnimationTrack::Quaternion;
	ret->scale=new MayaAnimationTrack();

	std::vector<float> times;
	for(float t=track.start;t<track.end;t+=track.sampling)
		times.push_back(t);
	times.push_back(track.end);

	ret->keysCount=times.size();
	math::vector3d pos;
	MayaAnimationTrack::KeyFrame kf;
	MTime ctime= MAnimControl::currentTime();
	for(int i=0;i<times.size();++i)
	{
		//MTransformationMatrix trans=dPath.transformation();
		MAnimControl::setCurrentTime(MTime(times[i],ctime.unit()));

		kf.t=times[i];

		MMatrix bindMatrix = dPath.inclusiveMatrix();;
		MMatrix localMatrix;
		if (m_parent)
			localMatrix = bindMatrix * m_parent->m_bindMatrix.inverse();
		else
		{	// root node of skeleton
			localMatrix = bindMatrix;
		}


		MVector vPos=((MTransformationMatrix)localMatrix).getTranslation(MSpace::kWorld);
		kf.v.set(vPos.x,vPos.y,vPos.z);
		ret->pos->keyframes.push_back(kf);

		MQuaternion qOri;
		((MTransformationMatrix)localMatrix).getRotationQuaternion(qOri.x,qOri.y,qOri.z,qOri.w);
		kf.q=math::quaternion(qOri.w,qOri.x,qOri.y,qOri.z);
		ret->rot->keyframes.push_back(kf);

		double scale[3];
		((MTransformationMatrix)localMatrix).getScale(scale,MSpace::kWorld);

		kf.v.set(scale[0],scale[1],scale[2]);
		ret->scale->keyframes.push_back(kf);
	
	}
	return ret;
}


void MayaNode::GenerateTracks(MayaAnimationTrack*t, 
					std::vector<MayaAnimationTrack*>& tracks,ExporterParams& params)
{
	for(int i=0;i<params.animationTracks.size();++i)
	{
		AnimationTrackDesc& desc=params.animationTracks[i];
		MayaAnimationTrack* newTrack=new MayaAnimationTrack();
		newTrack->name=desc.name;
		tracks.push_back(newTrack);
		for(int j=0;j<t->keyframes.size();++j)
		{
			if(t->keyframes[j].t>=desc.start && t->keyframes[j].t<=desc.end)
			{
				newTrack->keyframes.push_back(t->keyframes[j]);
			}
		}
		if(newTrack->keyframes.size()==0 )
		{
			if(desc.end-desc.start>0)
				newTrack->keyframes.push_back(t->GetKeyAt(desc.start));
		}else 
		{
			if(newTrack->keyframes[0].t!=desc.start)
			{
				newTrack->keyframes.insert(newTrack->keyframes.begin(),t->GetKeyAt(desc.start));
			}
			if(newTrack->keyframes[newTrack->keyframes.size()-1].t!=desc.end)
			{
				newTrack->keyframes.push_back(t->GetKeyAt(desc.end));
			}
		}
	}
}

void MayaNode::_OnLoadComponent(const MDagPath& dpath,ExporterParams& params)
{
}

MStatus MayaNode::read(MFnTransform& dPath,ExporterParams& params)
{
	MPlug plug;
	double dist;
	MAngle angle;
	MString strVal;
	//MFnAnimCurve *x,*y,*z;

	MTransformationMatrix trans=dPath.transformation();

	MDagPath dag;
	dPath.getPath(dag);

	m_bindMatrix=dag.inclusiveMatrix();

	plug = dPath.findPlug("NodeType");
	plug.getValue(strVal);
	m_componentType=strVal.asChar();

	_OnLoadComponent(dag,params);
	m_compData=NodeTypeMap::CreateComponent(m_componentType,this);
	if(m_compData)
		m_compData->LoadData(dag,params);

	m_name=ExporterParams::fixName(dPath.partialPathName());

	plug = dPath.findPlug("translateX");
	plug.getValue(dist);
	m_position.x=dist*params.scaler;

	//x=readAnimCurve(plug,params);

	plug = dPath.findPlug("translateY");
	plug.getValue(dist);
	m_position.y=dist*params.scaler;

	//y=readAnimCurve(plug,params);

	plug = dPath.findPlug("translateZ");
	plug.getValue(dist);
	m_position.z=dist*params.scaler;

	//z=readAnimCurve(plug,params);
//	if(x && y && z)
	{
	//	MayaAnimationTrack* t=new MayaAnimationTrack();
//		t->read(x,y,z);
//		GenerateTracks(t,m_posAnim,params);
	}
/*

	if (x)
		delete x;
	if (y)
		delete y;
	if (z)
		delete z;*/



	plug = dPath.findPlug("rotateX");
	plug.getValue(angle);
	m_rotation.x=angle.asDegrees();

	//x=readAnimCurve(plug,params);


	plug = dPath.findPlug("rotateY");
	plug.getValue(angle);
	m_rotation.y=angle.asDegrees();

	//y=readAnimCurve(plug,params);

	plug = dPath.findPlug("rotateZ");
	plug.getValue(angle);
	m_rotation.z=angle.asDegrees();

	//z=readAnimCurve(plug,params);
//	if(x && y && z)
	{
	//	MayaAnimationTrack* t=new MayaAnimationTrack();
	//	t->read(x,y,z);
	//	GenerateTracks(t,m_rotAnim,params);
	}


/*
	if (x)
			delete x;
		if (y)
			delete y;
		if (z)
			delete z;*/
	

	plug = dPath.findPlug("scaleX");
	plug.getValue(dist);
	m_scale.x=dist;

	//x=readAnimCurve(plug,params);

	plug = dPath.findPlug("scaleY");
	plug.getValue(dist);
	m_scale.y=dist;

	//y=readAnimCurve(plug,params);

	plug = dPath.findPlug("scaleZ");
	plug.getValue(dist);
	m_scale.z=dist;

	//z=readAnimCurve(plug,params);

	//if(x && y && z)
	{
	//	MayaAnimationTrack* t=new MayaAnimationTrack();
	//	t->read(x,y,z);
	//	GenerateTracks(t,m_scaleAnim,params);
	}

	//if (x)
	//	delete x;
	//if (y)
	//	delete y;
	//if (z)
	//	delete z;

	for(int i=0;i<params.animationTracks.size();++i)
	{
		m_clips.push_back(loadAnimationClip(dag,params.animationTracks[i]));
	}

	ConvertTrack(params);

	return MS::kSuccess;
}

xml::XMLElement* MayaNode::saveXML(xml::XMLElement* parent,ExporterParams& params)
{
	xml::XMLElement* ret=new xml::XMLElement("Node");
	if(parent)
		parent->addSubElement(ret);

	ret->addAttribute("Name",m_name.asChar());
	ret->addAttribute("Position",core::StringConverter::toString(m_position));
	ret->addAttribute("Rotation",core::StringConverter::toString(m_rotation));
	ret->addAttribute("Scale",core::StringConverter::toString(m_scale));

	animation::TransAnimationGroup* track=ConvertTrack(params);
	track->exportXMLSettings(ret);

	delete track;

	for(int i=0;i<m_children.size();++i)
	{
		m_children[i]->saveXML(ret,params);
	}
	
	return ret;
}


xml::XMLElement* MayaNode::saveXMLAsComponent(xml::XMLElement* parent,ExporterParams& params)
{
	xml::XMLElement* ret=0;
	if(m_compData)
		ret=m_compData->Export(parent,params);
	else ret=parent;

	for(int i=0;i<m_children.size();++i)
	{
		m_children[i]->saveXMLAsComponent(ret,params);
	}

	return ret;
}


animation::TransAnimationGroup* MayaNode::ConvertTrack(ExporterParams& params)
{
	animation::TransAnimationGroup* group=new animation::TransAnimationGroup(m_name.asChar());

	for(int t=0;t<m_clips.size();++t)
	{
		animation::TransformationAnimationTrack* ret=new animation::TransformationAnimationTrack(m_clips[t]->name,0,0,0,0);
		group->addTrack(ret);
		int keysCount=0;
		keysCount= m_clips[t]->keysCount;

		math::vector3d pos;
		math::quaternion rot;
		math::vector3d scale;
		float time=0;
		for(int i=0;i<keysCount;++i)
		{
			time=m_clips[t]->pos->keyframes[i].t;
			pos=m_clips[t]->pos->keyframes[i].v;
			rot=m_clips[t]->rot->keyframes[i].q;
			scale=m_clips[t]->scale->keyframes[i].v;

			ret->createKeyFrame(time,pos,scale,rot);
		}
	}

	return group;
}
}
}

