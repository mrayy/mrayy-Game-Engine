

#include "stdafx.h"


#include "LuaHelpers.h"
#include "StringConverter.h"
#include "ISceneNode.h"
#include "CameraNode.h"
#include "LightNode.h"
#include "MeshRenderableNode.h"
#include "MeshBufferData.h"
#include "SceneManager.h"
#include "ViewPort.h"
#include "DebugDrawManager.h"

#include "luabind/operator.hpp"

using namespace luabind;

namespace mray{
	using namespace scene;
namespace script{

	void MovableNodeExport(lua_State* L)
	{
		module(L,"scene")[
			class_<IMovable>("IMovable")
				.property("ID",&IMovable::getID,&IMovable::setID)
				.property("Name",&IMovable::getNodeName,&IMovable::setNodeName)
				.property("Position",&IMovable::getPosition,&IMovable::setPosition)
				.property("Orientation",&IMovable::getOrintation,&IMovable::setOrintation)
				.property("Scale",&IMovable::getScale,&IMovable::setScale)
				.property("Parent",&IMovable::getParent,&IMovable::setParent)
				.property("ManualAnimation",&IMovable::IsManualAnimation,&IMovable::SetManualAnimation)

				.def("addChild",&IMovable::addChild)
				.def("removeChild",&IMovable::removeChild)
				.def("removeFromParent",&IMovable::removeFromParent)
				.def("getChildren",&IMovable::getChildren)

				.def("AttachAnimationTrack",&IMovable::AttachAnimationTrack)
				.def("DeattachAnimationTrack",&IMovable::DeattachAnimationTrack)

				.def("rotate",(bool(IMovable::*)(const math::quaternion &q,ETransformSpace space))&IMovable::rotate)
				.def("pitch",&IMovable::pitch)
				.def("yaw",&IMovable::yaw)
				.def("roll",&IMovable::roll)
				.def("translate",&IMovable::translate)


				.def("getAbsolutePosition",&IMovable::getAbsolutePosition)
				.def("getAbsoluteOrintation",&IMovable::getAbsoluteOrintation)
				.def("getAbsoluteScale",&IMovable::getAbsoluteScale)
				.def("getBoundingBox",&IMovable::getBoundingBox)
				.def("getTransformedBoundingBox",&IMovable::getTransformedBoundingBox)
				.def("getAbsoluteTransformation",(const math::matrix4x4&(IMovable::*)()const)&IMovable::getAbsoluteTransformation)
				.def("getRelativeTransformation",&IMovable::getRelativeTransformation)
				,
				class_<ICullable,IMovable>("ICullable")
				.property("ViewDistance",&ICullable::getViewDistance,&ICullable::setViewDistance)
				.property("CullingType",&ICullable::getCullingType,&ICullable::setCullingType)
				,
				class_<ISceneNode,ICullable>("SceneNode")
				.property("DrawBoundingBox",&ISceneNode::getDrawBoundingBox,&ISceneNode::setDrawBoundingBox)
				.property("Visible",&ISceneNode::isVisible,(bool(ISceneNode::*)(const bool&))&ISceneNode::setVisible)

				.def("removeAll",&ISceneNode::removeAll)
				.def("AttachNode",&ISceneNode::AttachNode)
				.def("RemoveNode",&ISceneNode::RemoveNode)
				.def("GetAttachedNodesCount",&ISceneNode::GetAttachedNodesCount)
				.def("GetAttachedNode",&ISceneNode::GetAttachedNode)
				.def("ClearAttachedNodes",&ISceneNode::ClearAttachedNodes)

				.def("getElementById",&ISceneNode::getElementById)
				,
			class_<IViewNode>("ViewNode")
				.def("getProjectionMatrix", &IViewNode::getProjectionMatrix)
				.def("getViewMatrix", &IViewNode::getViewMatrix)
				.def("getPrevViewMatrix", &IViewNode::getPrevViewMatrix)
				.def("getViewFrustrum", &IViewNode::getViewFrustrum)
				.def("getViewPos", &IViewNode::getViewPos)
				.def("isCulling", &IViewNode::isCulling)
				,
			class_<CameraNode,ISceneNode,IViewNode>("Camera")
				.property("FovY",&CameraNode::getFovY,&CameraNode::setFovY)
				.property("ZNear",&CameraNode::getZNear,&CameraNode::setZNear)
				.property("ZFar",&CameraNode::getZFar,&CameraNode::setZFar)
				.property("Aspect",&CameraNode::getAspect,&CameraNode::setAspect)
				.property("Target",(const math::vector3d &(CameraNode::*)())&CameraNode::getTarget,&CameraNode::setTarget)
				.property("UpVector",(const math::vector3d &(CameraNode::*)())&CameraNode::getUpVector,&CameraNode::setUpVector)
				.property("UseTarget",&CameraNode::getIsTargettingEnabled,&CameraNode::setUseTarget)
				.property("AutoUpdateAspect",&CameraNode::getAutoUpdateAspect,&CameraNode::setAutoUpdateAspect)
				.property("ProjectionType",&CameraNode::getProjectionType,&CameraNode::setProjectionType)
				
				.def("SetWindow", &CameraNode::SetWindow)
				.def("PositionCamera", &CameraNode::PositionCamera)
				.def("ScreenToWorld", &CameraNode::ScreenToWorld)
				.def("WorldToScreen", &CameraNode::WorldToScreen)
				.def("enableReflection", &CameraNode::enableReflection)
				.def("isReflect", &CameraNode::isReflect)
				.def("disableReflection", &CameraNode::disableReflection)
				.def("getReflectionPlane", &CameraNode::getReflectionPlane)
				.def("getReflectionMatrix", &CameraNode::getReflectionMatrix)
				.def("updateView", &CameraNode::updateView)
				.def("getReflectionPlane", &CameraNode::getReflectionPlane)
				,
			class_<LightNode,ISceneNode,IViewNode>("Light")
				.property("Enable",&LightNode::isEnabled,&LightNode::setEnabled)
				.property("CastShadows",&LightNode::isCastShadows,&LightNode::setCastShadows)
				.property("Target",&LightNode::getTarget,&LightNode::setTarget)
				.property("Ambient",&LightNode::getAmbient,&LightNode::setAmbient)
				.property("Diffuse",&LightNode::getDiffuse,&LightNode::setDiffuse)
				.property("Specular",&LightNode::getSpecular,&LightNode::setSpecular)
				.property("Radius",&LightNode::getRadius,&LightNode::setRadius)
				.property("Type",&LightNode::getType,&LightNode::setType)

				.property("ShadowMap",&LightNode::getShadowMap,&LightNode::setShadowMap)

				.def("getDir", &LightNode::getDir)
				.def("setProjection", &LightNode::setProjection)
				.def("getShadowViewProjMatrix", &LightNode::getShadowViewProjMatrix)
		];
	}

	void RenderableNodeExport(lua_State* L)
	{
		module(L,"scene")[
			class_<IRenderable>("Renderable")
				.property("ShadowCaster",&IRenderable::isShadowCaster,&IRenderable::setShadowCaster)
				.property("ShadowReciver",&IRenderable::isShadowReciver,&IRenderable::setShadowReciver)
				.property("Layer",&IRenderable::getLayer,&IRenderable::setLayer)
				.property("Owner",&IRenderable::getOwner,&IRenderable::setOwner)
				.property("IsCustomRenderGroup",&IRenderable::HasCustomRenderGroup,&IRenderable::SetHasCustomRenderGroup)
				.property("TargetRenderGroup",&IRenderable::GetTargetRenderGroup,&IRenderable::SetTargetRenderGroup)
				.property("UseIdentityMatrix",&IRenderable::IsUseIdentityMatrix,&IRenderable::SetUseIdentityMatrix)

				.def("setMaterial", &IRenderable::setMaterial)
				.def("getMaterial", &IRenderable::getMaterial)
				.def("getMaterialCount", &IRenderable::getMaterialCount)
				.def("setShader", &IRenderable::setShader)
				.def("getShader", &IRenderable::getShader)
				.def("getBoundingBox", &IRenderable::getBoundingBox)
		];
		module(L,"scene")[
			class_<MeshRenderableNode,IRenderable>("MeshRenderableNode")
				.def(constructor<SMesh*>())
				.property("CustomLayer",&MeshRenderableNode::getCustomLayer,&MeshRenderableNode::setCustomLayer)
				.property("Mesh",&MeshRenderableNode::getMesh,&MeshRenderableNode::setMesh)
		];
	}
	void SMeshExport(lua_State* L)
	{
		module(L,"scene")[
			class_<SMesh>("Mesh")
				.def(constructor<>())
				.property("skeleton",&SMesh::getSkeleton,&SMesh::setSkeleton)
				.property("boundingBox",&SMesh::getBoundingBox,&SMesh::setBoundingBox)

				.def("getBuffersCount", &SMesh::getBuffersCount)
				.def("GetWorldMatricies", (std::vector<math::matrix4x4>&(SMesh::*)())&SMesh::GetWorldMatricies)
				.def("addNewBuffer", &SMesh::addNewBuffer)
				.def("addBuffer", &SMesh::addBuffer)
				.def("getBufferIndex", &SMesh::getBufferIndex)
				.def("getBuffer", &SMesh::getBuffer)
				.def("getBufferData", &SMesh::getBufferData)
				.def("getBufferByName", &SMesh::getBufferByName)
				.def("removeBuffer", &SMesh::removeBuffer)
				.def("getBufferByName", &SMesh::getBufferByName)
				.def("getBufferByName", &SMesh::getBufferByName)
				.def("getDuplicate", &SMesh::getDuplicate)
				.def("duplicateContextOnly", &SMesh::duplicateContextOnly)
		];
	}
	void MeshBufferDataExport(lua_State* L)
	{
		module(L,"scene")[
			class_<MeshBufferData>("MeshBufferData")
				.def(constructor<>())
				.property("meshBuffer",&MeshBufferData::getMeshBuffer,&MeshBufferData::setMeshBuffer)
				.property("material",&MeshBufferData::getMaterial,&MeshBufferData::setMaterial)
				.property("visible",&MeshBufferData::isVisible,&MeshBufferData::setVisible)
				.property("animator",&MeshBufferData::getAnimator,&MeshBufferData::setAnimator)
				.property("morphAnimator",&MeshBufferData::getMorphAnimator,&MeshBufferData::setMorphAnimator)

				.def("getDuplicate", &MeshBufferData::getDuplicate)
				.def("duplicateContextOnly", &MeshBufferData::duplicateContext)
		];
	}
	void IMeshBufferExport(lua_State* L)
	{
		module(L,"scene")[
			class_<IMeshBuffer>("MeshBuffer")
				.property("name",&IMeshBuffer::getMeshName,&IMeshBuffer::setMeshName)
				.property("renderType",&IMeshBuffer::getRenderType,&IMeshBuffer::setRenderType)
				.property("boundingBox",&IMeshBuffer::getBoundingBox,&IMeshBuffer::setBoundingBox)
				.property("skeletonName",&IMeshBuffer::getSkeletonName,&IMeshBuffer::setSkeletonName)
				.property("skinCluster",&IMeshBuffer::getSkinCluster,&IMeshBuffer::setSkinCluster)

				.def("addStream", &IMeshBuffer::addStream)
				.def("createStream", &IMeshBuffer::createStream)
				.def("removeStream", &IMeshBuffer::removeStream)
				.def("getStream", &IMeshBuffer::getStream)
				.def("getStreamsCount", &IMeshBuffer::getStreamsCount)
				.def("getStreamsData", &IMeshBuffer::getStreamsData)
				.def("createIndexBuffer", &IMeshBuffer::createIndexBuffer)
				.def("getIndexData", &IMeshBuffer::getIndexData)
				.def("createIndexBuffer", &IMeshBuffer::createIndexBuffer)
				.def("createIndexBuffer", &IMeshBuffer::createIndexBuffer)
				.def("getDuplicate", &IMeshBuffer::getDuplicate)
				.def("duplicateContextOnly", &IMeshBuffer::duplicateContext)
		];
	}

	void SceneManagerExport(lua_State*L)
	{
		module(L,"scene")[
			class_<ISceneManager>("ISceneManager")
				.def("createLightNode", &ISceneManager::createLightNode)
				.def("createSceneNode", &ISceneManager::createSceneNode)
				.def("createCamera", &ISceneManager::createCamera)
				.def("addSceneNode", &ISceneManager::addSceneNode)
				.def("removeSceneNode", &ISceneManager::removeSceneNode)
				.def("getNodeByID", &ISceneManager::getNodeByID)
				.def("clearSceneNodes", &ISceneManager::clearSceneNodes)
				.def("getActiveCamera", &ISceneManager::getActiveCamera)
				.def("setActiveCamera", &ISceneManager::setActiveCamera)
				.def("isNodeCulled", &ISceneManager::isNodeCulled)
				.def("setActiveViewnode", &ISceneManager::setActiveViewnode)
				.def("getActiveViewnode", &ISceneManager::getActiveViewnode)
				.def("addRenderPass", &ISceneManager::addRenderPass)
				.def("removeRenderPass", &ISceneManager::removeRenderPass)
				.def("haveRenderPass", &ISceneManager::haveRenderPass)
				.def("getCurrnetRenderPass", &ISceneManager::getCurrnetRenderPass)
				,
			class_<SceneManager,ISceneManager>("SceneManager")
				.def(constructor<video::IVideoDevice*>())

		];
	}
	void ViewPortExport(lua_State*L)
	{
		module(L,"scene")[
			class_<ViewPort>("ViewPort")

				.def(constructor<>())
				.property("autoClearRT",&ViewPort::isAutoClearRT,&ViewPort::setAutoClearRT)
				.property("autoUpdateRTSize",&ViewPort::GetAutoUpdateRTSize,&ViewPort::SetAutoUpdateRTSize)
				.property("onlyToRenderTarget",&ViewPort::isOnlyToRenderTarget,&ViewPort::setOnlyToRenderTarget)
				.property("name",&ViewPort::getName,&ViewPort::setName)
				.property("zorder",&ViewPort::getZOrder,&ViewPort::setZOrder)
				.property("camera",&ViewPort::getCamera,&ViewPort::setCamera)
				.property("active",&ViewPort::isActive,&ViewPort::setActive)
				.property("postProcessing",&ViewPort::isEnabledPostProcessing,&ViewPort::enablePostProcessing)
				.property("renderTarget",&ViewPort::getRenderTarget,&ViewPort::setRenderTarget)
				.property("clearColor",&ViewPort::GetClearColor,&ViewPort::SetClearColor)
				.property("absViewPort",&ViewPort::getAbsViewPort,&ViewPort::setAbsViewPort)


				.def("getRelViewPort", &ViewPort::getRelViewPort)
				.def("setViewPort", &ViewPort::setViewPort)
				.def("getAbsRenderingViewPort", &ViewPort::getAbsRenderingViewPort)
				.def("setPostProcessing", &ViewPort::setPostProcessing)
				.def("getColorTexture", &ViewPort::getColorTexture)
				.def("getSize", &ViewPort::getSize)
		];
	}
	void DebugDrawManagerExport(lua_State*L)
	{
		module(L,"scene")[
			class_<DebugDrawManager>("IDebugDrawManager")
				.def("AddLine", &IDebugDrawManager::AddLine)
				.def("AddArrow", &IDebugDrawManager::AddArrow)
				.def("AddCross", &IDebugDrawManager::AddCross)
				.def("AddSphere", &IDebugDrawManager::AddSphere)
				.def("AddCircle", &IDebugDrawManager::AddCircle)
				.def("AddAxis", &IDebugDrawManager::AddAxis)
				.def("AddMesh", &IDebugDrawManager::AddMesh)
				.def("AddAABB", &IDebugDrawManager::AddAABB)
				.def("AddOBB", &IDebugDrawManager::AddOBB)
				.def("AddString", &IDebugDrawManager::AddString)
				.def("StartDraw", &IDebugDrawManager::StartDraw)
				.def("EndDraw", &IDebugDrawManager::EndDraw)
				.def("Update", &IDebugDrawManager::Update)
			/*,
			class_<DebugDrawManager,IDebugDrawManager>("DebugDrawManager")

				.def(constructor<video::IVideoDevice*>())*/

		];
	}
	void SceneEnumExport(lua_State*L)
	{
		module(L,"scene")[
			class_<ETransformSpace>("TransformSpace")
				.enum_("constants")
				[
					value("Local",TS_Local),
					value("World",TS_World),
					value("Parent",TS_Parent)
				]
		];
	}


	void OpenScene(lua_State*L)
	{
		MovableNodeExport(L);
		SMeshExport(L);
		RenderableNodeExport(L);
		MeshBufferDataExport(L);
		IMeshBufferExport(L);
		SceneManagerExport(L);
		ViewPortExport(L);
		DebugDrawManagerExport(L);
		SceneEnumExport(L);
	}
}
}
