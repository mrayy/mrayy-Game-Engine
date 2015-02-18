

/********************************************************************
	created:	2012/02/09
	created:	9:2:2012   19:32
	filename: 	d:\Development\mrayEngine\Tools\MayaExporter\MayaSkeleton.h
	file path:	d:\Development\mrayEngine\Tools\MayaExporter
	file base:	MayaSkeleton
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __MayaSkeleton__
#define __MayaSkeleton__

#include "MayaAnimationTrack.h"

namespace mray
{
namespace scene
{
	class Skeleton;
	class BoneNode;
}
namespace exporter
{
	class ExporterParams;
	class AnimationTrackDesc;

	class MayaBone
	{
	public:
		MString name;
		int id;
		MMatrix localMatrix;
		MMatrix bindMatrix;
		MayaBone* parent;

		math::vector3d pos;
		math::quaternion orintation;
		math::vector3d scale;
		MDagPath jointDag;

		std::vector<MayaBone*> children;
	};

class MayaSkeleton
{
private:
protected:

	MString m_restorePose;
	std::vector<MayaBone*> m_bones;
	std::vector<int> m_roots;

	class AnimationTrack
	{
	public:
		AnimationTrack()
		{
			posTrack=new MayaAnimationTrack();
			rotTrack=new MayaAnimationTrack();
			scaleTrack=new MayaAnimationTrack();

			rotTrack->trackType=MayaAnimationTrack::Quaternion;
		}
		~AnimationTrack()
		{
			delete posTrack;
			delete rotTrack;
			delete scaleTrack;
		}

		core::string targetNode;
		int id;
		int keysCount;

		MayaAnimationTrack* posTrack;
		MayaAnimationTrack* rotTrack;
		MayaAnimationTrack* scaleTrack;
	};
	class AnimationClip
	{
	public:
		~AnimationClip()
		{
			for(int i=0;i<tracks.size();++i)
				delete tracks[i];
		}
		core::string name;
		std::vector<AnimationTrack*> tracks;
	};

	std::vector<AnimationClip*> m_clips;

	void SaveBone(scene::Skeleton* sk,MayaBone* b,scene::BoneNode*parent);
	MayaBone* loadBone(MDagPath& jointDag, MayaBone* parent, ExporterParams& params,MFnSkinCluster* pSkinCluster);
	void readBoneTransformations(MDagPath& jointDag, MayaBone* parent,ExporterParams& params,
		math::vector3d&pos,math::quaternion&ori,math::vector3d&scale,
		MMatrix *localMatrix,MMatrix *bindMatrix);
public:
	MayaSkeleton();
	virtual~MayaSkeleton();


	std::vector<MayaBone*>& GetBones(){return m_bones;}

	void clear();
	MStatus read(MFnSkinCluster* pSkinCluster,ExporterParams& params);

	AnimationClip* loadAnimationClip(const AnimationTrackDesc& track,ExporterParams& params);

	void fillSkeleton(scene::Skeleton* sk,ExporterParams& params);
	void exportAnimations(ExporterParams& params);

};


}
}

#endif

