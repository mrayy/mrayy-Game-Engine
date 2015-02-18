#include "stdafx.h"

#include "DefaultEnum.h"
#include "EnumManager.h"
#include "Event.h"
#include "GUIEvent.h"
#include "JoystickEvent.h"
#include "MouseEvent.h"
#include "NetworkEvent.h"
#include "SystemEvent.h"
#include "IMouseController.h"
#include "FontAttributes.h"
#include "IGUIElement.h"
#include "IAnimationTrack.h"
#include "IGPUShaderProgram.h"
#include "IGUIStackPanel.h"

#include "videoCommon.h"

#include "IValue.h"

#include "IPhysical3DDef.h"
#include "IVideoDevice.h"

#include "AnimationTypes.h"
#include "LightNode.h"
#include "CameraNode.h"
#include "GPUUniform.h"

#include "VehicleWheelDesc.h"

namespace mray{

void DefaultEnum::registerDefaultEnum(){
	//Value Type
	StartRegisterEnum(EValueType);
	RegisterEnumValueA(mT("Unkown"),EVT_UNKOWN);
	RegisterEnumValueA(mT("Int"),EVT_INT);
	RegisterEnumValueA(mT("Float"),EVT_FLOAT);
	RegisterEnumValueA(mT("Boolean"),EVT_BOOLEAN);
	RegisterEnumValueA(mT("Color"),EVT_COLOR);
	RegisterEnumValueA(mT("String"),EVT_STRING);
	RegisterEnumValueA(mT("Vector2di"),EVT_VECTOR2Di);
	RegisterEnumValueA(mT("Vector2df"),EVT_VECTOR2Df);
	RegisterEnumValueA(mT("Vector3di"),EVT_VECTOR3Di);
	RegisterEnumValueA(mT("Vector3df"),EVT_VECTOR3Df);
	RegisterEnumValueA(mT("Vector4di"),EVT_VECTOR4Di);
	RegisterEnumValueA(mT("Vector4df"),EVT_VECTOR4Df);
	RegisterEnumValueA(mT("Line2d"),EVT_LINE2D);
	RegisterEnumValueA(mT("Line3d"),EVT_LINE3D);
	RegisterEnumValueA(mT("Box3d"),EVT_BOX3D);
	RegisterEnumValueA(mT("Matrix3x3"),EVT_MATRIX3x3);
	RegisterEnumValueA(mT("Matrix4x4"),EVT_MATRIX4x4);
	RegisterEnumValueA(mT("Plane"),EVT_PLANE);
	RegisterEnumValueA(mT("Quaternion"),EVT_QUATERNION);
	RegisterEnumValueA(mT("Recti"),EVT_RECTi);
	RegisterEnumValueA(mT("Rectf"),EVT_RECTf);
	RegisterEnumValueA(mT("Material"),EVT_MATERIAL);
	RegisterEnumValueA(mT("Texture"),EVT_TEXTURE);
	RegisterEnumValueA(mT("Shader"),EVT_SHADER);
	EndRegisterEnum();

	//Event Type
	StartRegisterEnum(EVENT_TYPE);
	RegisterEnumValueA(mT("Unkown"),ET_Unkown.ID());
	RegisterEnumValueA(mT("Mouse"),ET_Mouse.ID());
	RegisterEnumValueA(mT("Keyboard"),ET_Keyboard.ID());
	RegisterEnumValueA(mT("Joystick"),ET_Joystick.ID());
	RegisterEnumValueA(mT("GUI"),ET_GUIEvent.ID());
	RegisterEnumValueA(mT("Network"),ET_NetworkEvent.ID());
	RegisterEnumValueA(mT("Resize"),ET_ResizeEvent.ID());
	RegisterEnumValueA(mT("System"),ET_SystemEvent.ID());
	RegisterEnumValueA(mT("ChangeMode"),ET_ChangeModeEvent.ID());
	EndRegisterEnum();

	//GUI Event Type
	StartRegisterEnum(GUI_EVENT_TYPE);
	RegisterEnumValueA(mT("None"),GET_NONE);
	RegisterEnumValueA(mT("HasFocus"),GET_HAS_FOCUS);
	RegisterEnumValueA(mT("MouseDown"),GET_Mouse_DOWN);
	RegisterEnumValueA(mT("MouseDownAgain"),GET_Mouse_DOWN_AGAIN);
	RegisterEnumValueA(mT("MouseUP"),GET_Mouse_UP);
	RegisterEnumValueA(mT("MouseMove"),GET_Mouse_MOVE);
	RegisterEnumValueA(mT("Changed"),GET_CHANGED);
	RegisterEnumValueA(mT("DialogClose"),GET_DIALOG_CLOSE);
	RegisterEnumValueA(mT("SelectItem"),GET_SELECT_ITEM);
	EndRegisterEnum();

	//Joystick Event Type
	StartRegisterEnum(JOYSTIC_EVENT_TYPE);
	RegisterEnumValueA(mT("ButtonPressed"),JET_BUTTON_PRESSED);
	RegisterEnumValueA(mT("ButtonReleased"),JET_BUTTON_RELEASED);
	RegisterEnumValueA(mT("AxisMoved"),JET_AXIS_MOVED);
	RegisterEnumValueA(mT("PovMoved"),JET_POV_MOVED);
	RegisterEnumValueA(mT("Vector3Moved"),JET_VECTOR3_MOVED);
	EndRegisterEnum();

	//Mouse Event Type
	StartRegisterEnum(MOUSE_EVENT_TYPE);
	RegisterEnumValueA(mT("None"),MET_NONE);
	RegisterEnumValueA(mT("LeftDown"),MET_LEFTDOWN);
	RegisterEnumValueA(mT("RightDown"),MET_RIGHTDOWN);
	RegisterEnumValueA(mT("MiddleDown"),MET_MIDDLEDOWN);
	RegisterEnumValueA(mT("LeftUp"),MET_LEFTUP);
	RegisterEnumValueA(mT("RightUp"),MET_RIGHTUP);
	RegisterEnumValueA(mT("MiddleUp"),MET_MIDDLEUP);
	RegisterEnumValueA(mT("LeftDblClick"),MET_LEFTDblClick);
	RegisterEnumValueA(mT("RightDblClick"),MET_RIGHTDblClick);
	RegisterEnumValueA(mT("MiddleDblClick"),MET_MIDDLEDblClick);
	RegisterEnumValueA(mT("Wheel"),MET_WHEEL);
	RegisterEnumValueA(mT("Moved"),MET_MOVED);
	EndRegisterEnum();

	//Network Event Type
	StartRegisterEnum(ENetworkEventType);
	RegisterEnumValueA(mT("Recived"),ENET_Recived);
	RegisterEnumValueA(mT("Connected"),ENET_Connected);
	RegisterEnumValueA(mT("Disconnect"),ENET_Disconnect);
	EndRegisterEnum();

	//System Event Type
	StartRegisterEnum(ESystemEventType);
	RegisterEnumValueA(mT("Show"),ESET_Show);
	RegisterEnumValueA(mT("Hide"),ESET_Hide);
	RegisterEnumValueA(mT("Resized"),ESET_Resized);
	RegisterEnumValueA(mT("LostFocus"),ESET_LostFocus);
	RegisterEnumValueA(mT("GotFocus"),ESET_GotFocus);
	RegisterEnumValueA(mT("LostActive"),ESET_LostActive);
	RegisterEnumValueA(mT("GotActive"),ESET_GotActive);
	RegisterEnumValueA(mT("Destroy"),ESET_Destroy);
	EndRegisterEnum();

	//Mouse Button
	StartRegisterEnum(EMouseButton);
	RegisterEnumValueA(mT("Left"),controllers::EMB_Left);
	RegisterEnumValueA(mT("Right"),controllers::EMB_Right);
	RegisterEnumValueA(mT("Middle"),controllers::EMB_Middle);
	EndRegisterEnum();

	using namespace GUI;
	//Font Alignment
	StartRegisterEnum(EFontAligment);
	RegisterEnumValueA(mT("TopLeft"),EFA_TopLeft);
	RegisterEnumValueA(mT("TopCenter"), EFA_TopCenter);
	RegisterEnumValueA(mT("TopRight"), EFA_TopRight);
	RegisterEnumValueA(mT("MiddleLeft"),EFA_MiddleLeft);
	RegisterEnumValueA(mT("MiddleCenter"), EFA_MiddleCenter);
	RegisterEnumValueA(mT("MiddleRight"), EFA_MiddleRight);
	RegisterEnumValueA(mT("BottomLeft"),EFA_BottomLeft);
	RegisterEnumValueA(mT("BottomCenter"), EFA_BottomCenter);
	RegisterEnumValueA(mT("BottomRight"), EFA_BottomRight);
	EndRegisterEnum();

	//Alignment Button
	StartRegisterEnum(EHorizontalAlignment);
	RegisterEnumValueA(mT("Left"),EHA_Left);
	RegisterEnumValueA(mT("Center"),EHA_Center);
	RegisterEnumValueA(mT("Right"),EHA_Right);
	EndRegisterEnum();

	StartRegisterEnum(EVerticalAlignment);
	RegisterEnumValueA(mT("Top"),EVA_Top);
	RegisterEnumValueA(mT("Center"),EVA_Center);
	RegisterEnumValueA(mT("Bottom"),EVA_Bottom);
	EndRegisterEnum();

	//EDock
	StartRegisterEnum(EElementDock);
	RegisterEnumValueA(mT("None"),EED_None);
	RegisterEnumValueA(mT("Fill"),EED_Fill);
	RegisterEnumValueA(mT("Left"),EED_Left);
	RegisterEnumValueA(mT("Right"),EED_Right);
	RegisterEnumValueA(mT("Top"),EED_Top);
	RegisterEnumValueA(mT("Bottom"),EED_Bottom);
	EndRegisterEnum();

	StartRegisterEnum(IGUIStackPanel::EStackDirection);
	RegisterEnumValueA(mT("Horizontal"),IGUIStackPanel::EHorizontal);
	RegisterEnumValueA(mT("Vertical"),IGUIStackPanel::EVertical);
	EndRegisterEnum();

/*
	//EDock
	StartRegisterEnum(EPosAnchor);
	RegisterEnumValueA(mT("Anchor_U"),EPAnchor_U);
	RegisterEnumValueA(mT("Anchor_B"),EPAnchor_B);
	RegisterEnumValueA(mT("Anchor_L"),EPAnchor_L);
	RegisterEnumValueA(mT("Anchor_R"),EPAnchor_R);
	RegisterEnumValueA(mT("Anchor_MidX"),EPAnchor_MidX);
	RegisterEnumValueA(mT("Anchor_MidY"),EPAnchor_MidY);
	RegisterEnumValueA(mT("Anchor_UL"),EPAnchor_UL);
	RegisterEnumValueA(mT("Anchor_BL"),EPAnchor_BL);
	RegisterEnumValueA(mT("Anchor_UR"),EPAnchor_UR);
	RegisterEnumValueA(mT("Anchor_BR"),EPAnchor_BR);
	RegisterEnumValueA(mT("Anchor_Center"),EPAnchor_CENTER);
	EndRegisterEnum();*/


	using namespace scene;
	//ELightType
	StartRegisterEnum(ELightType);
	RegisterEnumValueA(mT("Point"),LT_PointLight);
	RegisterEnumValueA(mT("Directional"),LT_DirectionalLight);
	RegisterEnumValueA(mT("Spot"),LT_SpotLight);
	EndRegisterEnum();
	//EProjectionType
	StartRegisterEnum(EProjectionType);
	RegisterEnumValueA(mT("Perspective"),EPT_Perspective);
	RegisterEnumValueA(mT("Ortho"),EPT_Ortho);
	EndRegisterEnum();


	using namespace animation;
	//EAnimationMode
	StartRegisterEnum(EAnimationMode);
	RegisterEnumValueA(mT("Cycle"),EAM_Cycle);
	RegisterEnumValueA(mT("PingPong"),EAM_PingPong);
	RegisterEnumValueA(mT("Clamp"),EAM_Clamp);
	EndRegisterEnum();

	//EInterpolationType
	StartRegisterEnum(EInterpolationType);
	RegisterEnumValueA(mT("Linear"),EIT_Linear);
	RegisterEnumValueA(mT("Spline"),EIT_Spline);
	EndRegisterEnum();

	using namespace video;

	StartRegisterEnum(EMeshRenderType);
	RegisterEnumValueA(mT("Point"),MR_POINTS);
	RegisterEnumValueA(mT("Line"),MR_LINES);
	RegisterEnumValueA(mT("LineLoop"),MR_LINE_LOOP);
	RegisterEnumValueA(mT("LineStrip"),MR_LINE_STRIP);
	RegisterEnumValueA(mT("Triangle"),MR_TRIANGLES);
	RegisterEnumValueA(mT("TriangleStrip"),MR_TRIANGLE_STRIP);
	RegisterEnumValueA(mT("TriangleFan"),MR_TRIANGLE_FAN);
	RegisterEnumValueA(mT("Quad"),MR_QUADS);
	RegisterEnumValueA(mT("QuadStrip"),MR_QUAD_STRIP);
	RegisterEnumValueA(mT("Polygon"),MR_POLYGON);
	EndRegisterEnum();

	StartRegisterEnum(ETextureType);
	RegisterEnumValueA(mT("1DTex"),ETT_1DTex);
	RegisterEnumValueA(mT("2DTex"),ETT_2DTex);
	RegisterEnumValueA(mT("3DTex"),ETT_3DTex);
	RegisterEnumValueA(mT("CubeTex"),ETT_CubeTex);
	EndRegisterEnum();


	StartRegisterEnum(EPixelFormat);
	RegisterEnumValueA(mT("Unkown"),EPixel_Unkown);
	RegisterEnumValueA(mT("LUMINANCE8"),EPixel_LUMINANCE8);
	RegisterEnumValueA(mT("LUMINANCE16"),EPixel_LUMINANCE16);
	RegisterEnumValueA(mT("Alpha8"),EPixel_Alpha8);
	RegisterEnumValueA(mT("Alpha4Luminance4"),EPixel_Alpha4Luminance4);
	RegisterEnumValueA(mT("Alpha8Luminance8"),EPixel_Alpha8Luminance8);
	RegisterEnumValueA(mT("R5G6B5"),EPixel_R5G6B5);
	RegisterEnumValueA(mT("R8G8B8"),EPixel_R8G8B8);
	RegisterEnumValueA(mT("A8R8G8B8"),EPixel_R8G8B8A8);
	RegisterEnumValueA(mT("X8R8G8B8"),EPixel_X8R8G8B8);
	RegisterEnumValueA(mT("B8G8R8"),EPixel_B8G8R8);
	RegisterEnumValueA(mT("A8B8G8R8"),EPixel_B8G8R8A8);
	RegisterEnumValueA(mT("X8B8G8R8"),EPixel_X8B8G8R8);
	RegisterEnumValueA(mT("Float16_R"),EPixel_Float16_R);
	RegisterEnumValueA(mT("Float16_RGB"),EPixel_Float16_RGB);
	RegisterEnumValueA(mT("Float16_RGBA"),EPixel_Float16_RGBA);
	RegisterEnumValueA(mT("Float16_GR"),EPixel_Float16_GR);
	RegisterEnumValueA(mT("Float32_R"),EPixel_Float32_R);
	RegisterEnumValueA(mT("Float32_RGB"),EPixel_Float32_RGB);
	RegisterEnumValueA(mT("Float32_RGBA"),EPixel_Float32_RGBA);
	RegisterEnumValueA(mT("Float32_GR"),EPixel_Float32_GR);
	RegisterEnumValueA(mT("DEPTH"),EPixel_Depth);
	RegisterEnumValueA(mT("Short_RGBA"),EPixel_Short_RGBA);
	RegisterEnumValueA(mT("Short_RGB"),EPixel_Short_RGB);
	RegisterEnumValueA(mT("Short_GR"),EPixel_Short_GR);
	RegisterEnumValueA(mT("DXT1"),EPixel_DXT1);
	RegisterEnumValueA(mT("DXT2"),EPixel_DXT2);
	RegisterEnumValueA(mT("DXT3"),EPixel_DXT3);
	RegisterEnumValueA(mT("DXT4"),EPixel_DXT4);
	RegisterEnumValueA(mT("DXT5"),EPixel_DXT5);
	EndRegisterEnum();


	StartRegisterEnum(ETextureClamp);
	RegisterEnumValueA(mT("Repeat"),ETC_REPEAT);
	RegisterEnumValueA(mT("Clamp"),ETC_CLAMP);
	RegisterEnumValueA(mT("ClampToEdge"),ETC_CLAMP_TO_EDGE);
	RegisterEnumValueA(mT("ClampToBorder"),ETC_CLAMP_TO_BORDER);
	RegisterEnumValueA(mT("Mirror"),ETC_MIRROR);
	EndRegisterEnum();


	StartRegisterEnum(ETextureWrap);
	RegisterEnumValueA(mT("WrapS"),ETW_WrapS);
	RegisterEnumValueA(mT("WrapT"),ETW_WrapT);
	RegisterEnumValueA(mT("WrapR"),ETW_WrapR);
	EndRegisterEnum();


	StartRegisterEnum(ETextureGenType);
	RegisterEnumValueA(mT("GenS"),ETGT_GenS);
	RegisterEnumValueA(mT("GenT"),ETGT_GenT);
	RegisterEnumValueA(mT("GenR"),ETGT_GenR);
	RegisterEnumValueA(mT("GenQ"),ETGT_GenQ);
	EndRegisterEnum();

	StartRegisterEnum(ETextureGenMode);
	RegisterEnumValueA(mT("EyeLinear"),ETGM_EyeLinear);
	RegisterEnumValueA(mT("ObjectLinear"),ETGM_ObjectLinear);
	RegisterEnumValueA(mT("SphereMap"),ETGM_SphereMap);
	EndRegisterEnum();


	StartRegisterEnum(EMaterialRenderType);
	RegisterEnumValueA(mT("SOLID"),MRT_SOLID);
	RegisterEnumValueA(mT("TRANSPARENT"),MRT_TRANSPARENT);
	RegisterEnumValueA(mT("MUL"),MRT_MUL);
	RegisterEnumValueA(mT("ADD"),MRT_ADD);
	RegisterEnumValueA(mT("ADDALPHA"),MRT_ADDALPHA);
	EndRegisterEnum();

	StartRegisterEnum(RenderStates);
	RegisterEnumValueA(mT("Wireframe"),RS_Wireframe);
	RegisterEnumValueA(mT("Smooth"),RS_Smooth);
	RegisterEnumValueA(mT("Lighting"),RS_Lighting);
	RegisterEnumValueA(mT("ZTest"),RS_ZTest);
	RegisterEnumValueA(mT("ZWrite"),RS_ZWrite);
	RegisterEnumValueA(mT("Points"),RS_Points);
	RegisterEnumValueA(mT("CullFace"),RS_CullFace);
	RegisterEnumValueA(mT("AlphaTest"),RS_AlphaTest);
	RegisterEnumValueA(mT("Fog"),RS_Fog);
	RegisterEnumValueA(mT("Blend"),RS_Blend);
	EndRegisterEnum();

	StartRegisterEnum(EState);
	RegisterEnumValueA(mT("DontUse"),ES_DontUse);
	RegisterEnumValueA(mT("Use"),ES_Use);
	RegisterEnumValueA(mT("Unknown"),ES_Unknown);
	EndRegisterEnum();

	StartRegisterEnum(CullFaceMode);
	RegisterEnumValueA(mT("ECFM_CW"),ECFM_CW);
	RegisterEnumValueA(mT("ECFM_CCW"),ECFM_CCW);
	EndRegisterEnum();

	StartRegisterEnum(BlendFactor);
	RegisterEnumValueA(mT("ZERO"),EBF_ZERO);
	RegisterEnumValueA(mT("ONE"),EBF_ONE);
	RegisterEnumValueA(mT("DST_COLOR"),EBF_DST_COLOR);
	RegisterEnumValueA(mT("ONE_MINUS_DST_COLOR"),EBF_ONE_MINUS_DST_COLOR);
	RegisterEnumValueA(mT("SRC_COLOR"),EBF_SRC_COLOR);
	RegisterEnumValueA(mT("ONE_MINUS_SRC_COLOR"),EBF_ONE_MINUS_SRC_COLOR);
	RegisterEnumValueA(mT("SRC_ALPHA"),EBF_SRC_ALPHA);
	RegisterEnumValueA(mT("ONE_MINUS_SRC_ALPHA"),EBF_ONE_MINUS_SRC_ALPHA);
	RegisterEnumValueA(mT("DST_ALPHA"),EBF_DST_ALPHA);
	RegisterEnumValueA(mT("ONE_MINUS_DST_ALPHA"),EBF_ONE_MINUS_DST_ALPHA);
	RegisterEnumValueA(mT("SRC_ALPHA_SATURATE"),EBF_SRC_ALPHA_SATURATE);
	EndRegisterEnum();

	StartRegisterEnum(AlphaFunc);
	RegisterEnumValueA(mT("Never"),EAF_Never);
	RegisterEnumValueA(mT("Less"),EAF_Less);
	RegisterEnumValueA(mT("Equal"),EAF_Equal);
	RegisterEnumValueA(mT("LessEqual"),EAF_LessEqual);
	RegisterEnumValueA(mT("Greater"),EAF_Greater);
	RegisterEnumValueA(mT("NotEqual"),EAF_NotEqual);
	RegisterEnumValueA(mT("GreaterEqual"),EAF_GreaterEqual);
	RegisterEnumValueA(mT("Always"),EAF_Always);
	EndRegisterEnum();

	StartRegisterEnum(ShaderProgramType);
	RegisterEnumValueA(mT("VertexProgram"),EShader_VertexProgram);
	RegisterEnumValueA(mT("FragmentProgram"),EShader_FragmentProgram);
	RegisterEnumValueA(mT("GeometryProgram"),EShader_GeometryProgram);
	EndRegisterEnum();

	StartRegisterEnum(EUniformType);
	RegisterEnumValueA(mT("int"),EUT_Int1);
	RegisterEnumValueA(mT("int2"),EUT_Int2);
	RegisterEnumValueA(mT("int3"),EUT_Int3);
	RegisterEnumValueA(mT("int4"),EUT_Int4);

	RegisterEnumValueA(mT("float"),EUT_Float1);
	RegisterEnumValueA(mT("float2"),EUT_Float2);
	RegisterEnumValueA(mT("float3"),EUT_Float3);
	RegisterEnumValueA(mT("float4"),EUT_Float4);

	RegisterEnumValueA(mT("sampler1D"),EUT_Sampler1D);
	RegisterEnumValueA(mT("sampler2D"),EUT_Sampler2D);
	RegisterEnumValueA(mT("sampler3D"),EUT_Sampler3D);
	RegisterEnumValueA(mT("samplerCUBE"),EUT_SamplerCube);

	RegisterEnumValueA(mT("float2x2"),EUT_Matrix2x2);
	RegisterEnumValueA(mT("float2x3"),EUT_Matrix2x3);
	RegisterEnumValueA(mT("float2x4"),EUT_Matrix2x4);
	RegisterEnumValueA(mT("float3x3"),EUT_Matrix3x3);
	RegisterEnumValueA(mT("float3x4"),EUT_Matrix3x4);
	RegisterEnumValueA(mT("float4x2"),EUT_Matrix4x2);
	RegisterEnumValueA(mT("float4x3"),EUT_Matrix4x3);
	RegisterEnumValueA(mT("float4x4"),EUT_Matrix4x4);

	RegisterEnumValueA(mT("array"),EUT_Array);
	EndRegisterEnum();


	//physics
	using namespace physics;
	StartRegisterEnum(ESupported3DJoints);
	RegisterEnumValueA(mT("RevoluteJoint"),ESJ3D_RevoluteJoint);
	RegisterEnumValueA(mT("PrismaticJoint"),ESJ3D_PrismaticJoint);
	RegisterEnumValueA(mT("DistanceJoint"),ESJ3D_DistanceJoint);
	RegisterEnumValueA(mT("PulleyJoint"),ESJ3D_PulleyJoint);
	RegisterEnumValueA(mT("FixedJoint"),ESJ3D_FixedJoint);
	RegisterEnumValueA(mT("GearJoint"),ESJ3D_GearJoint);
	RegisterEnumValueA(mT("D6Joint"),ESJ3D_D6Joint);
	RegisterEnumValueA(mT("Cylindrical"),ESJ3D_Cylindrical);
	RegisterEnumValueA(mT("Spherical"),ESJ3D_Spherical);
	RegisterEnumValueA(mT("PointOnLine"),ESJ3D_PointOnLine);
	RegisterEnumValueA(mT("PointOnPlane"),ESJ3D_PointOnPlane);
	EndRegisterEnum();



	StartRegisterEnum(EMaterialCombineMode);
	RegisterEnumValueA(mT("Average"),ECM_Average);
	RegisterEnumValueA(mT("Min"),ECM_Min);
	RegisterEnumValueA(mT("Multiply"),ECM_Multiply);
	RegisterEnumValueA(mT("Max"),ECM_Max);
	EndRegisterEnum();

	StartRegisterEnum(ED6JointMotion);
	RegisterEnumValueA(mT("Motion_Locked"),ED6Joint_Motion_Locked);
	RegisterEnumValueA(mT("Motion_Limited"),ED6Joint_Motion_Limited);
	RegisterEnumValueA(mT("Motion_Free"),ED6Joint_Motion_Free);
	EndRegisterEnum();


	StartRegisterEnum(EJointProjectionMode);
	RegisterEnumValueA(mT("None"),EJPM_None);
	RegisterEnumValueA(mT("PointMindist"),EJPM_PointMindist);
	RegisterEnumValueA(mT("LinearMindist"),EJPM_LinearMindist);
	EndRegisterEnum();

	StartRegisterEnum(EVehicleWheelFlags);
	RegisterEnumValueA(mT("SteerableInput"),EVW_Steerable_Input);
	RegisterEnumValueA(mT("SteerableAuto"),EVW_Steerable_Auto);
	RegisterEnumValueA(mT("AffectedByHandbrake"),EVW_AffectedByHandbrake);
	RegisterEnumValueA(mT("Accelerated"),EVW_Accelerated);
	EndRegisterEnum();
}
void DefaultEnum::removeDefaultEnum(){

	EnumManager&em=EnumManager::getInstance();

	em.removeEnum(mT("EVENT_TYPE"));
	em.removeEnum(mT("GUI_EVENT_TYPE"));
	em.removeEnum(mT("JOYSTIC_EVENT_TYPE"));
	em.removeEnum(mT("MOUSE_EVENT_TYPE"));
	em.removeEnum(mT("ENetworkEventType"));
	em.removeEnum(mT("ESystemEventType"));
	em.removeEnum(mT("EMouseButton"));
	em.removeEnum(mT("EFontAligment"));
	em.removeEnum(mT("EAlignment"));
	em.removeEnum(mT("EDock"));
	em.removeEnum(mT("EPosAnchor"));
	em.removeEnum(mT("EAnimationMode"));
	em.removeEnum(mT("EInterpolationType"));
	em.removeEnum(mT("EMeshRenderType"));
	em.removeEnum(mT("ETextureType"));
	em.removeEnum(mT("EPixelFormat"));
	em.removeEnum(mT("ETextureClamp"));
	em.removeEnum(mT("ETextureWrap"));
	em.removeEnum(mT("ETextureGenType"));
	em.removeEnum(mT("ETextureGenMode"));
	em.removeEnum(mT("EMaterialRenderType"));
	em.removeEnum(mT("RenderStates"));
	em.removeEnum(mT("EState"));
	em.removeEnum(mT("ECullFaceMode"));
	em.removeEnum(mT("EAlphaFunc"));
	em.removeEnum(mT("EBlendFactor"));


	em.removeEnum(mT("ESupported3DJoints"));
	em.removeEnum(mT("ED6JointMotion"));
	em.removeEnum(mT("EJointProjectionMode"));
	em.removeEnum(mT("EMaterialCombineMode"));

}

}
