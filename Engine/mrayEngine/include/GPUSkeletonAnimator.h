
/********************************************************************
	created:	2009/06/28
	created:	28:6:2009   0:06
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GPUSkeletonAnimator.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GPUSkeletonAnimator
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___GPUSkeletonAnimator___
#define ___GPUSkeletonAnimator___


#include "ISkeletonAnimator.h"



namespace mray{
namespace scene{

class MRAY_DLL GPUSkeletonAnimator:public ISkeletonAnimator
{
private:
protected:
	Skeleton*m_skeleton;
	IMeshBuffer*m_buffer;
	ISkinCluster* m_skinCluster;

	std::vector<math::matrix4x4> m_bonesMatricies;

	int m_weightStream;
	int m_boneStream;
public:

	GPUSkeletonAnimator(IMeshBuffer*buff,ISkinCluster* skinCluster,Skeleton*skeleton,
		int weightStream=1,int boneStream=2);
	virtual~GPUSkeletonAnimator();

	virtual void update(float dt);

	math::matrix4x4*getBonesMatricies(){return &m_bonesMatricies[0];}
	int getBonesMatriciesCount(){return m_bonesMatricies.size();}

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

#endif //___GPUSkeletonAnimator___
