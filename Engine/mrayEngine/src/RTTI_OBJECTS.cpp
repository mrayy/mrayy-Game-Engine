
#include "stdafx.h"

#include "CPropertieSet.h"
#include "IObject.h"
#include "IResource.h"
#include "AnimationTrackGroup.h"
#include "ISceneNode.h"
#include "IGUIElement.h"
#include "AnimationMixer.h"
#include "TransAnimationGroup.h"
#include "MorphTrackGroup.h"
#include "TransformationAnimationMixer.h"
#include "MorphAnimationMixer.h"
#include "KeyAnimationTrack.h"
#include "ActionAnimationTrack.h"
#include "MeshRenderableNode.h"
#include "CameraNode.h"
#include "LightNode.h"
#include "SLODAnimatedMesh.h"
#include "IPhysicalCloth.h"
#include "IPhysicalNode.h"
#include "TransformationAnimationTrack.h"
#include "ISound.h"
#include "PhysicalVehicle.h"
#include "IPhysicalForceFieldShapes.h"
#include "ParticleEmitter.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "ISoundListener.h"
#include "ICustomParticleEmitter.h"
#include "FileTextureSource.h"
#include "VideoTextureSource.h"

using namespace mray;
using namespace animation;
using namespace scene;
using namespace GUI;
using namespace physics;
using namespace sound;
using namespace video;

IMPLEMENT_ROOT_RTTI(CPropertieSet);

IMPLEMENT_RTTI(IObject,CPropertieSet);

IMPLEMENT_RTTI(IResource,IObject)

IMPLEMENT_RTTI(ISound,IObject);
IMPLEMENT_RTTI(ISoundListener,IObject);

IMPLEMENT_RTTI(AnimationTrackGroup,IObject);
IMPLEMENT_RTTI(IAnimationMixer,IObject);
IMPLEMENT_RTTI(IAnimationTrack,IObject);

IMPLEMENT_RTTI(IGUIElement,IObject)


IMPLEMENT_RTTI(AnimationMixer,IAnimationMixer);

IMPLEMENT_RTTI(TransAnimationGroup,AnimationTrackGroup);
IMPLEMENT_RTTI(MorphTrackGroup,AnimationTrackGroup);

IMPLEMENT_RTTI(TransformationAnimationMixer,AnimationMixer);
IMPLEMENT_RTTI(MorphAnimationMixer,AnimationMixer);

IMPLEMENT_RTTI(KeyAnimationTrack,IAnimationTrack);

IMPLEMENT_RTTI(ActionAnimationTrack,KeyAnimationTrack);
IMPLEMENT_RTTI(TransformationAnimationTrack,KeyAnimationTrack);

IMPLEMENT_RTTI(IRenderable,IObject)
IMPLEMENT_RTTI(IMovable,IObject)
IMPLEMENT_RTTI(ICullable,IMovable)
IMPLEMENT_RTTI(MeshRenderableNode,IRenderable)
IMPLEMENT_RTTI(SLODAnimatedMesh,MeshRenderableNode)

IMPLEMENT_RTTI(ICustomParticleEmitter,IObject)

IMPLEMENT_RTTI(ISceneNode,ICullable)
IMPLEMENT_RTTI(CameraNode,ISceneNode)
IMPLEMENT_RTTI(LightNode,ISceneNode)


IMPLEMENT_RTTI(IPhysicalCloth,IObject)
IMPLEMENT_RTTI(IPhysicalNode,IObject)
IMPLEMENT_RTTI(PhysicalVehicle,IObject)
IMPLEMENT_RTTI(IPhysicalForceFieldShape,IObject)


IMPLEMENT_RTTI(ParticleEmitter,IObject);

IMPLEMENT_RTTI(RenderMaterial,IResource);
IMPLEMENT_RTTI(RenderTechnique,IObject);
IMPLEMENT_RTTI(RenderPass,IObject);


IMPLEMENT_RTTI(ITextureSource,IObject)
IMPLEMENT_RTTI(FileTextureSource,ITextureSource)
IMPLEMENT_RTTI(VideoTextureSource,ITextureSource)