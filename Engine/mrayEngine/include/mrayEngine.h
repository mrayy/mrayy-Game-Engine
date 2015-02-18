 

//used to disable some annoying warnings
#pragma warning (disable:4244)
#pragma warning (disable:4305)
#pragma warning (disable:4018)
#pragma warning (disable:4172)
#pragma warning (disable:4183)

//#include <omp.h>


//math lib
#include <point2d.h>
#include <point3d.h>
#include <Box3D.h>
#include <ExtrudedPolygon.h>
#include <Frustum.h>
#include <GenericMatrix.h>
#include <Intersection.h>
#include <line2d.h>
#include <line3d.h>
#include <MathUtil.h>
#include <matrix3x3.h>
#include <matrix4x4.h>
#include <mMath.h>
#include <Plane.h>
#include <quaternion.h>
#include <Polygon2D.h>
#include <randomizer.h>
#include <Ray3d.h>
#include <Rect.h>
#include <Sphere.h>
#include <triangle3d.h>

#include "common.h"

#include "CompileConfig.h"
#include "macros.h"
#include "IVideoDevice.h"
#include "KEY_MAP.h"
//#include "IObjectFactory.h"
#include "ObjectFactoryManager.h"

#include "Engine.h"
#include "Event.h"
#include "EventMemoryManager.h"
#include "EventQueue.h"
#include "MouseEvent.h"
#include "KeyboardEvent.h"
#include "JoystickEvent.h"
#include "GUIEvent.h"
#include "NetworkEvent.h"
#include "ResizeEvent.h"
#include "SystemEvent.h"
#include "ChangeModeEvent.h"

#include "FileResourceManager.h"

#include "KEYCode.h"
#include "IEventReciver.h"
#include "IFileSystem.h"
#include "IOSystem.h"
#include "IOSClipboard.h"
#include "IConsole.h"
#include "CSettingsFile.h"
#include "CPAKArchive.h"
#include "CMemory.h"
#include "CMemoryStream.h"
//#include "glext.h"
//#include "CJPGLoader.h"
#include "StringConverter.h"
#include "ILogManager.h"

#include "InputManager.h"

#include "ILexical.h"
//#include "CStringLexical.h"
//#include "CStreamLexical.h"
#include "CTokenTranslator.h"
#include "ITokenTranslator.h"
#include "CustomTTranslater.h"

#include "GCPtr.h"

#include "IMutex.h"

#include "CSpline.h"
#include "CCatmullCurve.h"
#include "CBezierCurve.h"

#include "CPropertieSet.h"

#include "ICursorControl.h"
#include "mString.h"

#include "heapSort.h"



#include "mTypes.h"
#include "ITimer.h"
#include "FPSCalc.h"
#include "ILogManager.h"
#include "CRawLoader.h"
#include "TexWriter.h"



#include "SColor.h"
#include "DefaultColors.h"
#include "ITexture.h"
#include "ITexture.h"
#include "ITexture.h"
#include "ITexture.h"
#include "ITexture.h"
#include "TextureBank.h"
#include "ColorConverter.h"
#include "SVideoMode.h"
#include "CMultiPassPP.h"
#include "CShaderPP.h"
#include "GPUShader.h"

#include "IAnimLoader.h"
#include "animLoader.h"
#include "IAnimationMixer.h"

#include "HeightMapRaw16.h"
#include "HeightMapRaw8.h"

#include "MeshGenerator.h"
#include "SMesh.h"
#include "SLocator.h"
#include "CameraNode.h"
#include "ISceneNode.h"
#include "IMeshLoader.h"
#include "IMeshBuffer.h"
//#include "SGrass.h"
#include "SLODAnimatedMesh.h"
#include "SAnimatedSprite.h"
#include "ISceneManager.h"
#include "SceneHelper.h"
#include "SSkyBoxNode.h"
#include "STerrain.h"
#include "RenderMaterial.h"
#include "SBillboardNode.h"
//#include "SWaterSurface.h"
#include "SAnimatorDirection.h"
#include "SAnimatorSpline.h"
#include "LightNode.h"
#include "IAnimators.h"
#include "SAnimatorRotate.h"
#include "SAnimatorCircle.h"
#include "SParticleSystem.h"
#include "IParticle.h"
#include "STextNode.h"
//#include "SWinTextNode.h"
#include "SSkyBoxNode.h"
#include "SSkyDomeNode.h"
#include "SParticleSystemFactory.h"
#include "Skeleton.h"
#include "SkeletonResourceManager.h"
#include "SMeshManipulator.h"
#include "MeshRenderableNode.h"
#include "TangentCalculater.h"
#include "SPatchTerrain.h"

#include "SkyBoxManager.h"

#include "SMeshBuffer.h"

#include "XMLSceneLoader.h"
#include "XMLTerrainReader.h"

#include "CTriangleSelector.h"

#include "GUIOverlayManager.h"
/*
#include "GUIWindow.h"
#include "GUIBenchMark.h"
#include "GUIButton.h"
#include "GUIElement.h"
#include "GUIManager.h"
#include "GUIStaticText.h"
#include "IGUITheme.h"
#include "GUIThemeManager.h"
#include "GUIImage.h"
#include "GUIButton.h"
#include "GUIEditBox.h"
#include "GUICheckBox.h"
#include "GUIRadioButton.h"
#include "GUIList.h"
#include "GUISlider.h"
#include "GUIComboList.h"
#include "GUIDummy.h"
#include "GUIToolBar.h"
#include "GUITabPanel.h"
#include "GUIFileBox.h"
#include "GUIContextMenu.h"
#include "GUIMenu.h"
#include "GUIDebug.h"
#include "GUIMessage.h"
#include "GUIParticleSystem.h"
#include "GUIPropertyGrid.h"
#include "GUICellSprite.h"
#include "GUIAnimatedSprite.h"*/

#include "GUITheme.h"
#include "XML_GUITheme.h"
/*
#include "GUIEffector.h"
#include "GUIFadeEffector.h"
#include "GUICircularEffector.h"
#include "GUITranslateEffector.h"
*/
#include "CShaderPP.h"

#include "IShaderConstantsCallback.h"

#include "CMouseController.h"
#include "CKeyboardController.h"
#include "CJoysticController.h"

#include "IDllManager.h"
#include "SoundManagerFactory.h"


#include "XMLWriter.h"


#include "AnimationMixer.h"
#include "IAnimationTrack.h"
#include "LookAtAnimation.h"
#include "TransformationAnimationMixer.h"


#include "IDelegate.h"


#include "PhysicObject.h"
#include "PhysicMesh.h"
#include "IPhysicObject.h"
#include "IBlockerObject.h"
#include "ElasticObject.h"
#include "SpringSystem.h"
#include "IPOAirFan.h"
#include "IPOForce.h"
#include "IPOGravity.h"


#include "GPUMorphAnimator.h"
#include "SkeletonAnimator.h"
#include "GPUSkeletonAnimator.h"

#include "Skeleton.h"
#include "ISkinCluster.h"

#include "MaterialResourceManager.h"
#include "ScriptResourceManager.h"
#include "IOSRegistry.h"
#include "ShaderSemanticTable.h"
#include "StreamLogger.h"
#include "HTMLLogger.h"
#include "IRenderTarget.h"


#include "IPhysicManager.h"

#include "IPhysicalJoint2D.h"
#include "IPhysicManager2D.h"
#include "IPhysicalNode2D.h"

#include "IPhysicMaterial.h"

#include "IPhysicContactListener2D.h"

#include "GenericValues.h"
#include "MathValues.h"
#include "VideoValues.h"

#include "RemoteDebugClient.h"
#include "RemoteDebugger.h"


#include "CAdjacent.h"

