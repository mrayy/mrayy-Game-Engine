
/********************************************************************
	created:	2009/07/18
	created:	18:7:2009   18:47
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MeshBufferData.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MeshBufferData
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___MeshBufferData___
#define ___MeshBufferData___

#include "IMeshbuffer.h"
#include "IShaderConstantsCallback.h"
#include "RenderMaterial.h"
#include "IMeshAnimator.h"

#include "MorphAnimationMixer.h"
#include "MorphAnimationTrack.h"
#include "IMorphAnimator.h"

namespace mray{
namespace scene{

	MakeSharedPtrType(MeshBufferData)

class MRAY_DLL MeshBufferData
{
private:
protected:
	IMeshBufferPtr m_buffer;
	video::RenderMaterialPtr m_material;

	video::IShaderConstantsCallbackPtr shaderCallback;
	IMeshAnimatorPtr m_animator;
	bool m_isVisible;

	animation::MorphAnimationMixerPtr m_animationMixer;
	IMorphAnimatorPtr m_morphAnimator;

public:
	MeshBufferData();
	virtual~MeshBufferData();

	uint calcSizeInternal();




	void setMeshBuffer(const IMeshBufferPtr& buffer);
	const IMeshBufferPtr& getMeshBuffer();

	virtual void setMaterial(const video::RenderMaterialPtr& mtrl);
	virtual const video::RenderMaterialPtr& getMaterial();
	virtual void setShaderCallback(const video::IShaderConstantsCallbackPtr& callback);
	virtual const video::IShaderConstantsCallbackPtr& getShaderCallback();

	virtual void setVisible(bool v);
	virtual bool isVisible();


	virtual const animation::MorphAnimationMixerPtr& getAnimationMixer();
	virtual void addAnimationTrack(animation::MorphAnimationTrack* track);

	virtual void setAnimator(const IMeshAnimatorPtr& anim);
	virtual const IMeshAnimatorPtr& getAnimator();

	virtual void setMorphAnimator(const IMorphAnimatorPtr& anim);
	virtual const IMorphAnimatorPtr& getMorphAnimator();

	void update(float dt);

	virtual MeshBufferData* getDuplicate();
	virtual MeshBufferData* duplicateContext();
};

}
}


#endif //___MeshBufferData___
