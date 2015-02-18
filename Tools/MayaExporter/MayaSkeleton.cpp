
#include "stdafx.h"
#include "MayaSkeleton.h"
#include "ExporterParams.h"
#include "Skeleton.h"
#include "BoneNode.h"


#include "TransformationAnimationMixer.h"
#include "TransformationAnimationTrack.h"
#include "AnimWriter.h"


namespace mray
{
namespace exporter
{

MayaSkeleton::MayaSkeleton()
{
}

MayaSkeleton::~MayaSkeleton()
{
	for(int i=0;i<m_bones.size();++i)
		delete m_bones[i];
	for(int i=0;i<m_clips.size();++i)
		delete m_clips[i];
}

void MayaSkeleton::clear()
{
	m_bones.clear();
	m_restorePose="";
}

MStatus MayaSkeleton::read(MFnSkinCluster* pSkinCluster,ExporterParams& params)
{
	MStatus stat;
	//check for valid skin cluster pointer
	if (!pSkinCluster)
	{
		std::cout << "Could not load skeleton data, no skin cluster specified\n";
		std::cout.flush();
		return MS::kFailure;
	}
	//retrieve and load joints from the skin cluster
	MDagPath jointDag,rootDag;
	MDagPathArray influenceDags;
	int numInfluenceObjs = pSkinCluster->influenceObjects(influenceDags,&stat);
	std::cout << "num influence objects: " << numInfluenceObjs << "\n";
	std::cout.flush();
	for (int i=0; i<numInfluenceObjs; i++)
	{
		jointDag = influenceDags[i];
		if (influenceDags[i].hasFn(MFn::kJoint))
		{
			//retrieve root joint
			rootDag = jointDag;
			while (jointDag.length()>0)
			{
				jointDag.pop();
				if (jointDag.hasFn(MFn::kJoint) && jointDag.length()>0)
					rootDag = jointDag;
			}
			//check if skeleton has already been loaded
			bool skip = false;
			for (int j=0; j<m_bones.size() && !skip; j++)
			{
				//skip skeleton if already loaded
				if (rootDag.partialPathName() == m_bones[j]->name)
				{
					skip = true;
				}
			}
			//load joints data from root
			if (!skip)
			{
				// load the skeleton
				std::cout <<  "Loading skeleton with root: " << rootDag.fullPathName().asChar() << "...\n";
				std::cout.flush();
				// save current selection list
				MSelectionList selectionList;
				MGlobal::getActiveSelectionList(selectionList);
				// select the root joint dag
				MGlobal::selectByName(rootDag.fullPathName(),MGlobal::kReplaceList);
				//save current pose (if no pose has been saved yet)
				if (m_restorePose == "")
				{
					MString poseName;
					MGlobal::executeCommand("dagPose -s",poseName,true);
					m_restorePose = poseName;
				}
				//set the skeleton to the desired neutral pose
				if (params.neutralPoseType == NPT_BINDPOSE)
				{
					//disable constraints, IK, etc...
					MGlobal::executeCommand("doEnableNodeItems false all",true);
					// Note: we reset to the bind pose
					MGlobal::executeCommand("dagPose -r -g -bp",true);
				}
				//load joints data
				MayaBone*b = loadBone(rootDag,NULL,params,pSkinCluster);
				if (b!=0)
				{
					std::cout << "OK\n";
					std::cout.flush();
				}
				else
				{
					std::cout << "Failed\n";
					std::cout.flush();
				}
				//restore selection list
				MGlobal::setActiveSelectionList(selectionList,MGlobal::kReplaceList);
			}
		}
	}

	for(int i=0;i<params.animationTracks.size();++i)
	{
		m_clips.push_back(loadAnimationClip(params.animationTracks[i],params));
	}
	return MS::kSuccess;
}

MayaBone* MayaSkeleton::loadBone(MDagPath& jointDag, MayaBone* parent, ExporterParams& params,MFnSkinCluster* pSkinCluster)
{
	MStatus stat;
	MayaBone* newJoint=0;
	MayaBone* parentJoint = parent;
	// if it is a joint node translate it and then proceed to child nodes, otherwise skip it
	// and proceed directly to child nodes
	if (jointDag.hasFn(MFn::kJoint))
	{
		MFnIkJoint jointFn(jointDag);
		// Display info
		std::cout << "Loading joint: " << jointFn.fullPathName().asChar();
		std::cout.flush();
		if (parent)
		{
			std::cout << " (parent: " << parent->name.asChar() << ")\n";
			std::cout.flush();
		}
		else
		{
			std::cout << "\n";
			std::cout.flush();
		}
		// Get parent index
		int idx=-1;
		if (parent)
		{
			for (int i=0; i<m_bones.size() && idx<0; i++)
			{
				if (m_bones[i]->name == parent->name)
					idx=i;
			}
		}

		MMatrix localMatrix;
		MMatrix bindMatrix;
		math::vector3d pos;
		math::vector3d scale;
		math::quaternion ori;

		newJoint=new MayaBone();
		readBoneTransformations(jointDag,parent,params,pos,ori,scale,&newJoint->localMatrix,&newJoint->bindMatrix);

		newJoint->name = jointFn.partialPathName();
		newJoint->id = m_bones.size();
		newJoint->parent = parent;
		newJoint->pos = pos * params.scaler;

		newJoint->orintation=ori;
		/*
		newJoint->angle = theta;
		newJoint->axis.x = axis.x;
		newJoint->axis.y = axis.y;
		newJoint->axis.z = axis.z;
		*/
		newJoint->scale = scale;
		newJoint->jointDag = jointDag;
		m_bones.push_back(newJoint);
		// If root is a root joint, save its index in the roots list
		if (idx < 0)
		{
			m_roots.push_back(m_bones.size() - 1);
		}
		// Get pointer to newly created joint
		parentJoint = newJoint;
	}
	// Load child joints
	for (int i=0; i<jointDag.childCount();i++)
	{
		MObject child;
		child = jointDag.child(i);
		MDagPath childDag = jointDag;
		childDag.push(child);
		MayaBone* b= loadBone(childDag,parentJoint,params,pSkinCluster);
		if(b)
			newJoint->children.push_back(b);
	}

	return newJoint;
}

void MayaSkeleton::readBoneTransformations(MDagPath& jointDag, MayaBone* parent,ExporterParams& params,math::vector3d&pos,math::quaternion&ori,math::vector3d&scale,
										   MMatrix *_localMatrix,MMatrix *_bindMatrix)
{
	// Get world bind matrix
	MMatrix bindMatrix = jointDag.inclusiveMatrix();;
	// Calculate local bind matrix
	MMatrix localMatrix;
	if (parent)
		localMatrix = bindMatrix * parent->bindMatrix.inverse();
	else
	{	// root node of skeleton
		localMatrix = bindMatrix;
	}

	if(_localMatrix)
		*_localMatrix=localMatrix;
	if(_bindMatrix)
		*_bindMatrix=bindMatrix;
	// Get translation
	MVector translation = ((MTransformationMatrix)localMatrix).getTranslation(MSpace::kPostTransform);
	/*	if (fabs(translation.x) < PRECISION)
	translation.x = 0;
	if (fabs(translation.y) < PRECISION)
	translation.y = 0;
	if (fabs(translation.z) < PRECISION)
	translation.z = 0;*/
	/*
	// Calculate rotation data
	double qx,qy,qz,qw;
	((MTransformationMatrix)localMatrix).getRotationQuaternion(qx,qy,qz,qw);
	MQuaternion rotation(qx,qy,qz,qw);
	MVector axis;
	double theta;
	rotation.getAxisAngle(axis,theta);
	if (fabs(axis.x) < PRECISION)
	axis.x = 0;
	if (fabs(axis.y) < PRECISION)
	axis.y = 0;
	if (fabs(axis.z) < PRECISION)
	axis.z = 0;
	axis.normalize();
	if (fabs(theta) < PRECISION)
	theta = 0;
	if (axis.length() < 0.5)
	{
	axis.x = 0;
	axis.y = 1;
	axis.z = 0;
	theta = 0;
	}*/

	double qx,qy,qz,qw;
	((MTransformationMatrix)localMatrix).getRotationQuaternion(qx,qy,qz,qw);
	// Get joint scale
	double scaleVec[3];
	((MTransformationMatrix)localMatrix).getScale(scaleVec,MSpace::kPostTransform);
	/*	if (fabs(scale[0]) < PRECISION)
	scale[0] = 0;
	if (fabs(scale[1]) < PRECISION)
	scale[1] = 0;
	if (fabs(scale[2]) < PRECISION)
	scale[2] = 0;*/
	// Set joint info

	pos.set(translation.x,translation.y,translation.z);
	ori=math::quaternion(qw,qx,qy,qz);
	scale.set(scaleVec[0],scaleVec[1],scaleVec[2]);
}
MayaSkeleton::AnimationClip* MayaSkeleton::loadAnimationClip(const AnimationTrackDesc& track,ExporterParams& params)
{
	AnimationClip* ret=new AnimationClip();
	ret->name=track.name.asChar();

	std::vector<float> times;
	for(float t=track.start;t<track.end;t+=track.sampling)
		times.push_back(t);
	times.push_back(track.end);

	for(int i=0;i<m_bones.size();++i)
	{
		AnimationTrack* track=new AnimationTrack();
		track->targetNode=m_bones[i]->name.asChar();
		track->id=m_bones[i]->id;
		track->keysCount=times.size();

		ret->tracks.push_back(track);
	}


	math::vector3d pos,scale;
	math::quaternion ori;
	MayaAnimationTrack::KeyFrame kf;
	MTime ctime= MAnimControl::currentTime();
	for(int i=0;i<times.size();++i)
	{
		MAnimControl::setCurrentTime(MTime(times[i],ctime.unit()));
		for(int j=0;j<m_bones.size();++j)
		{
			readBoneTransformations(m_bones[j]->jointDag,m_bones[j]->parent,params,pos,ori,scale,0,0);
			kf.v=pos;
			ret->tracks[j]->posTrack->keyframes.push_back(kf);
			kf.q=ori;
			ret->tracks[j]->rotTrack->keyframes.push_back(kf);
			kf.v=scale;
			ret->tracks[j]->scaleTrack->keyframes.push_back(kf);
		}
	}
	return ret;
}

void MayaSkeleton::fillSkeleton(scene::Skeleton* sk,ExporterParams& params)
{
	for(int i=0;i<m_roots.size();++i)
	{
		SaveBone(sk,m_bones[m_roots[i]],0);
	}
	sk->refreshRoots();
	exportAnimations(params);
}

void MayaSkeleton::SaveBone(scene::Skeleton* sk,MayaBone* b,scene::BoneNode*parent)
{
	scene::BoneNode* bone= sk->createBone(b->name.asChar(),parent);
	
	/*
	math::matrix4x4 mat;
	for(int i=0;i<4;++i)
		for(int j=0;j<4;++j)
			mat.m4x4[i][j]=b->bindMatrix.matrix[i][j];
	*/

	bone->setBasePosition(b->pos);
	bone->setBaseOrintation(b->orintation);
	//bone->setScale(b->scale);

	bone->setPosition(b->pos);
	bone->setOrintation(b->orintation);
	bone->setScale(b->scale);
	for(int i=0;i<b->children.size();++i)
		SaveBone(sk,b->children[i],bone);
}

void MayaSkeleton::exportAnimations(ExporterParams& params)
{
	animation::TransformationAnimationMixer* mixer=new animation::TransformationAnimationMixer();
	for(int i=0;i<m_clips.size();++i)
	{
		animation::TransAnimationGroup* group=new animation::TransAnimationGroup(m_clips[i]->name);
		mixer->addTrackGroup(group);
		for(int j=0;j<m_clips[i]->tracks.size();++j)
		{
			AnimationTrack* at=m_clips[i]->tracks[j];
			animation::TransformationAnimationTrack* track=new animation::TransformationAnimationTrack(at->targetNode,at->id,0,0,group);
			group->addTrack(track);

			for(int k=0;k<at->keysCount;++k)
			{
				track->createKeyFrame(at->posTrack->keyframes[k].t,at->posTrack->keyframes[k].v,at->scaleTrack->keyframes[k].v,
					at->rotTrack->keyframes[k].q);
			}
		}
	}

	loaders::AnimWriter writer;
	writer.write(mixer,(params.basePath+ "Skeleton.anim").asChar());
	delete mixer;
}

}
}