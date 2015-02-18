

/********************************************************************
	created:	2010/06/02
	created:	2:6:2010   17:16
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MeshAnimatorManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MeshAnimatorManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MeshAnimatorManager_h__
#define MeshAnimatorManager_h__

#include "CompileConfig.h"
#include "ISingleton.h"
#include "ISkeletonAnimator.h"
#include "IMorphAnimator.h"
#include "IShaderConstantsCallback.h"
#include "ISkinCluster.h"
#include "Skeleton.h"

namespace mray
{
namespace scene
{

class MRAY_DLL MeshAnimatorManager:public ISingleton<MeshAnimatorManager>
{
private:
protected:
	bool m_isGPUAnimators;
	int m_morphPosStreamStart;
	int m_morphNormalStreamStart;
	bool m_morphUseNormals;

	int m_skelWeightStream;
	int m_skelBoneStream;
public:
	MeshAnimatorManager();
	virtual ~MeshAnimatorManager();

	void SetGPUBasedAnimators(bool e);
	bool IsGPUBasedAnimators();

	void SetGPUMorphAnimatorParams(int posStreamStart,int normalStreamStart,bool useNormals);
	void SetGPUSkeletonAnimatorParams(int weightStream,int boneStream);
	
	int GetGPUBoneWeightStream(){return m_skelWeightStream;}
	int GetGPUBoneIDStream(){return m_skelBoneStream;}

	GCPtr<IMorphAnimator> CreateMorphAnimator(IMeshBuffer*buff);
	GCPtr<ISkeletonAnimator> CreateSkeletonAnimator(IMeshBuffer*buff,GCPtr<ISkinCluster> skinCluster,GCPtr<Skeleton>skeleton,
		int affectedAttrs = ISkeletonAnimator::EAA_Position);
	

	video::IShaderConstantsCallback* CreateSkeletonShaderCallback(ISkeletonAnimator*anim);
	video::IShaderConstantsCallback* CreateMorphShaderCallback(IMorphAnimator*anim);

};

}
}

#endif // MeshAnimatorManager_h__
