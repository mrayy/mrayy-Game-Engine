
/********************************************************************
	created:	2009/03/16
	created:	16:3:2009   0:30
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SkeletonAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SkeletonAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SkeletonAnimator___
#define ___SkeletonAnimator___

#include "ISkeletonAnimator.h"



namespace mray{
namespace scene{

class MRAY_DLL SkeletonAnimator:public ISkeletonAnimator
{
private:
protected:
	Skeleton*m_skeleton;
	IMeshBuffer*m_buffer;
	ISkinCluster* m_skinCluster;
	std::vector<math::vector3d> m_baseVectors[EAA_AttrsCount];

	int m_affectedAttrs;
	
public:

	SkeletonAnimator(IMeshBuffer*buff,ISkinCluster* skinCluster,Skeleton*skeleton,
		int affectedAttrs = EAA_Position);
	virtual~SkeletonAnimator();

	virtual void update(float dt);

	virtual IMeshBuffer* getMeshBuffer(){
		return m_buffer;
	}
	virtual void setMeshBuffer(IMeshBuffer*b);
	Skeleton*getSkeleton();
	ISkinCluster*getSkinCluster();

	virtual IMeshAnimator* duplicate();
};

}
}


#endif //___SkeletonAnimator___
