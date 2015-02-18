#include "stdafx.h"
#include "MeshAnimatorManager.h"

#include "GPUMorphAnimator.h"
#include "MorphAnimator.h"

#include "GPUSkeletonAnimator.h"
#include "SkeletonAnimator.h"
#include "GPUShader.h"

namespace mray
{
namespace scene
{



	class GPUMorphCallback:public video::IShaderConstantsCallback
	{
		scene::GPUMorphAnimator* m_anim;
	protected:
		virtual void setUniform(video::IGPUShaderProgram*shader,video::GPUUniform* u){
			if(shader->GetProgramType()==video::EShader_VertexProgram && u->name.equals_ignore_case("MorphWeight")){
				math::vector4d v=m_anim->getWeightVec();
				shader->setConstant(u,&v.x,4);
			}else{
				video::IShaderConstantsCallback::setUniform(shader,u);
			}
		}
	public:
		GPUMorphCallback(scene::GPUMorphAnimator* anim){
			m_anim=anim;
		}
	};
	class GPUSkinnedCallback:public video::IShaderConstantsCallback
	{
		scene::GPUSkeletonAnimator* m_anim;
	protected:
		virtual void setUniform(video::IGPUShaderProgram*shader,video::GPUUniform* u){
			if(shader->GetProgramType()==video::EShader_VertexProgram && u->name.equals_ignore_case("JointPalette")){
				math::matrix4x4* m=m_anim->getBonesMatricies();
				int cnt=m_anim->getBonesMatriciesCount();
				shader->setMatrixArray(u,m,cnt);
			}else{
				video::IShaderConstantsCallback::setUniform(shader,u);
			}
		}
	public:
		GPUSkinnedCallback(scene::GPUSkeletonAnimator* anim){
			m_anim=anim;
		}
	};


MeshAnimatorManager::MeshAnimatorManager()
{
	m_isGPUAnimators=false;

	m_morphPosStreamStart=1;
	m_morphNormalStreamStart=2;
	m_morphUseNormals=true;

	m_skelBoneStream=1;
	m_skelWeightStream=2;
}
MeshAnimatorManager::~MeshAnimatorManager()
{
}

void MeshAnimatorManager::SetGPUBasedAnimators(bool e)
{
	m_isGPUAnimators=e;
}
bool MeshAnimatorManager::IsGPUBasedAnimators()
{
	return m_isGPUAnimators;
}

void MeshAnimatorManager::SetGPUMorphAnimatorParams(int posStreamStart,int normalStreamStart,bool useNormals)
{
	m_morphPosStreamStart=posStreamStart;
	m_morphNormalStreamStart=normalStreamStart;
	m_morphUseNormals=useNormals;
}
void MeshAnimatorManager::SetGPUSkeletonAnimatorParams(int weightStream,int boneStream)
{
	m_skelBoneStream=boneStream;
	m_skelWeightStream=weightStream;
}

GCPtr<IMorphAnimator> MeshAnimatorManager::CreateMorphAnimator(IMeshBuffer*buff)
{
	if(m_isGPUAnimators)
	{
		return new GPUMorphAnimator(buff,m_morphPosStreamStart,m_morphNormalStreamStart,m_morphUseNormals);
	}else
		return new MorphAnimator(buff);
}
GCPtr<ISkeletonAnimator> MeshAnimatorManager::CreateSkeletonAnimator(IMeshBuffer*buff,GCPtr<ISkinCluster> skinCluster,GCPtr<Skeleton>skeleton,
	int affectedAttrs)
{
	if(m_isGPUAnimators)
	{
		return new GPUSkeletonAnimator(buff,skinCluster,skeleton,m_skelWeightStream,m_skelBoneStream);
	}else
		return new SkeletonAnimator(buff,skinCluster,skeleton,affectedAttrs);
}

video::IShaderConstantsCallback* MeshAnimatorManager::CreateSkeletonShaderCallback(ISkeletonAnimator*anim)
{
	return new GPUSkinnedCallback((GPUSkeletonAnimator*)anim);
}
video::IShaderConstantsCallback* MeshAnimatorManager::CreateMorphShaderCallback(IMorphAnimator*anim)
{
	return new GPUMorphCallback((GPUMorphAnimator*)anim);
}


}
}
