

/********************************************************************
	created:	2008/10/19
	created:	19:10:2008   19:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\Skeleton.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine
	file base:	Skeleton
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	Skeleton Object
*********************************************************************/


#ifndef ___Skeleton___
#define ___Skeleton___

#include "IResource.h"
#include "BoneNode.h"



namespace mray{
namespace animation{
	class TransformationAnimationMixer;
	class AnimationTrackGroup;
}
namespace scene{

class MRAY_DLL Skeleton:public IResource
{
protected:
	typedef std::vector<BoneNode*> BonesArray;
	BonesArray m_roots;

	BonesList m_bones;
	animation::TransformationAnimationMixer* m_animationMixer;

	int m_lastID;

	std::vector<math::matrix4x4> m_bonesMatricies;

	BoneNode* duplicateBone(BoneNode* b,Skeleton* sk);

	virtual uint calcSizeInternal();
public:
	Skeleton(const core::string&name);
	virtual~Skeleton();


	const std::vector<math::matrix4x4>& GetBonesMatricies()const{return m_bonesMatricies;}

	virtual void registerNodes();

	BoneNodePtr createBone(const core::string& name,BoneNode*parent);
	void addBone(BoneNodeCRef bone);

	void refreshRoots();

	void addAnimationTrackGroup(animation::AnimationTrackGroup*group);

	animation::TransformationAnimationMixer* getAnimationMixer();

	void update(float dt);

	int getRootBonesCount();
	BoneNode*getRootBone(int i=0);

	int getBonesCount();
	BoneNode*getBone(int i);
	BoneNode*getBone(const core::string& name);

	const BonesList& getBones();

	Skeleton* getDuplicate();
};

MakeSharedPtrType(Skeleton);

}
}

#endif //___Skeleton___
