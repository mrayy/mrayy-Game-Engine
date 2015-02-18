

#include "stdafx.h"
#include "LuaHelpers.h"
#include "StringConverter.h"
#include "RenderMaterial.h"
#include "RenderTechnique.h"
#include "RenderPass.h"
#include "DeviceCapabilites.h"
#include "IVideoDevice.h"
#include "RenderWindow.h"
#include "Viewport.h"
#include "InputManager.h"
#include "luabind/operator.hpp"

using namespace luabind;

namespace mray{
using namespace video;
namespace script{

	void RenderMaterialExport(lua_State* L)
	{
		module(L,"video")[
			class_<RenderMaterial>("RenderMaterial")
				.def(constructor<>())
				.property("name",&RenderMaterial::GetName,&RenderMaterial::SetName)

				.def("SetActiveTechniqueByIndex", &RenderMaterial::SetActiveTechniqueByIndex)
				.def("SetActiveTechniqueByName", &RenderMaterial::SetActiveTechniqueByName)
				.def("GetActiveTechnique", &RenderMaterial::GetActiveTechnique)
				.def("CreateTechnique", &RenderMaterial::CreateTechnique)
				.def("GetTechniques", &RenderMaterial::GetTechniques)
				.def("GetTechniqueAt", &RenderMaterial::GetTechniqueAt)
				.def("GetTechniqueByName", &RenderMaterial::GetTechniqueByName)
				.def("AddTechnique", &RenderMaterial::AddTechnique)
				.def("RemoveTechnique", &RenderMaterial::RemoveTechnique)
				.def("RemoveTechniqueAt", &RenderMaterial::RemoveTechniqueAt)
				.def("ClearTechniques", &RenderMaterial::ClearTechniques)
				.def("Duplicate", &RenderMaterial::Duplicate)
		];
	}
	void RenderTechniqueExport(lua_State* L)
	{
		module(L,"video")[
			class_<RenderTechnique>("RenderTechnique")
				.def(constructor<RenderMaterial*>())
				.property("name",&RenderTechnique::GetName,&RenderTechnique::SetName)
				.property("requiredProfiles",&RenderTechnique::GetRequiredProfiles,&RenderTechnique::SetRequiredProfiles)
				.property("ownerMaterial",&RenderTechnique::GetOwnerMaterial,&RenderTechnique::SetOwnerMaterial)
				.property("depthShader",&RenderTechnique::GetDepthShader,&RenderTechnique::SetDepthShader)


				.def("CreatePass", &RenderTechnique::CreatePass)
				.def("GetPasses", &RenderTechnique::GetPasses)
				.def("GetPassAt", &RenderTechnique::GetPassAt)
				.def("GetPassByName", &RenderTechnique::GetPassByName)
				.def("AddPass", &RenderTechnique::AddPass)
				.def("RemovePass", &RenderTechnique::RemovePass)
				.def("RemovePassAt", &RenderTechnique::RemovePassAt)
				.def("ClearPasses", &RenderTechnique::ClearPasses)
				.def("IsSupported", &RenderTechnique::IsSupported)
				.def("Duplicate", &RenderTechnique::Duplicate)
		];
	}
	void RenderPassExport(lua_State* L)
	{
		module(L,"video")[
			class_<RenderPass>("RenderPass")
				.def(constructor<RenderTechnique*>())
				.property("name",&RenderPass::GetName,&RenderPass::SetName)
				.property("ambient",&RenderPass::GetAmbient,&RenderPass::SetAmbient)
				.property("diffuse",&RenderPass::GetDiffuse,&RenderPass::SetDiffuse)
				.property("specular",&RenderPass::GetSpecular,&RenderPass::SetSpecular)
				.property("emissive",&RenderPass::GetEmissive,&RenderPass::SetEmissive)
				.property("shininess",&RenderPass::GetShininess,&RenderPass::SetShininess)
				.property("alpha",&RenderPass::GetAlpha,&RenderPass::SetAlpha)
				.property("thickness",&RenderPass::GetThickness,&RenderPass::SetThickness)
				.property("srceBlend",&RenderPass::GetSourceBlendFactor,&RenderPass::SetSourceBlendFactor)
				.property("destBlend",&RenderPass::GetDestinationBlendFactor,&RenderPass::SetDestinationBlendFactor)
				.property("alphaFunc",&RenderPass::GetAlphaFunction,&RenderPass::SetAlphaFunction)
				.property("alphaRef",&RenderPass::GetAlphaReferenceValue,&RenderPass::SetAlphaReferenceValue)
				.property("cullFaceDir",&RenderPass::GetCullFaceDirection,&RenderPass::SetCullFaceDirection)
				.property("renderShader",&RenderPass::getRenderShader,&RenderPass::setRenderShader)
				.property("ownerTechnique",&RenderPass::SetOwnerTechnique,&RenderPass::SetOwnerTechnique)

				.def("setTexture", &RenderPass::setTexture)
				.def("setTextureUnit", &RenderPass::setTextureUnit)
				.def("getTexture", &RenderPass::getTexture)
				.def("getTextureUnit", &RenderPass::getTextureUnit)
				.def("getTexturesCount", &RenderPass::getTexturesCount)
				.def("setTexturesCount", &RenderPass::setTexturesCount)
				.def("setRenderState", &RenderPass::setRenderState)
				.def("getRenderState", &RenderPass::getRenderState)
				.def("Duplicate", &RenderPass::Duplicate)
		];
	}

	void ITextureExport(lua_State* L)
	{
		module(L,"video")[
			class_<ITexture>("Texture")
				.property("renderTarget",&ITexture::isRenderTarget,&ITexture::setRenderTarget)
				.property("mipmapsFilter",&ITexture::getMipmapsFilter,&ITexture::setMipmapsFilter)
				.property("bilinearFilter",&ITexture::getBilinearFilter,&ITexture::setBilinearFilter)
				.property("trilinearFilter",&ITexture::getTrilinearFilter,&ITexture::setTrilinearFilter)
				.property("anisotropicFilter",&ITexture::getAnisotropicFilter,&ITexture::setAnisotropicFilter)
				.property("mipmapsCount",&ITexture::GetNumberOfMipmaps,&ITexture::SetNumberOfMipmaps)

				.def("getSurface", &ITexture::getSurface)
				.def("getSize", &ITexture::getSize)
				.def("getDataType", &ITexture::getDataType)
				.def("getImageFormat", &ITexture::getImageFormat)
				.def("getDuplicate", &ITexture::getDuplicate)
		];
	}

	void TextureUnitExport(lua_State* L)
	{
		module(L,"video")[
			class_<TextureUnit>("TextureUnit")
				.def(constructor<>())
				.def(constructor<TextureUnit*>())
				.property("texture",&TextureUnit::GetTexture,&TextureUnit::SetTexture)
				.property("textureSource",&TextureUnit::GetTextureSource,&TextureUnit::SetTextureSource)
				.property("edgeColor",&TextureUnit::GetEdgeColor,&TextureUnit::SetEdgeColor)
				.property("edgeColor",&TextureUnit::GetEdgeColor,&TextureUnit::SetEdgeColor)
				.property("edgeColor",&TextureUnit::GetEdgeColor,&TextureUnit::SetEdgeColor)

				.def("setTextureGenMode", &TextureUnit::setTextureGenMode)
				.def("getTextureGenMode", &TextureUnit::getTextureGenMode)
				.def("enableGenMode", &TextureUnit::enableGenMode)
				.def("isEnableGenMode", &TextureUnit::isEnableGenMode)
				.def("setTextureClamp", &TextureUnit::setTextureClamp)
				.def("getTextureClamp", &TextureUnit::getTextureClamp)
				.def("getTransformationMatrix", &TextureUnit::getTransformationMatrix)
				.def("updateMatrix", &TextureUnit::updateMatrix)
				
				.def_readwrite("trans",&TextureUnit::trans)
				.def_readwrite("rotate",&TextureUnit::rotate)
				.def_readwrite("scale",&TextureUnit::scale)
		];
	}
	void SColorExport(lua_State* L)
	{
		module(L,"video")[
			class_<SColor>("Color")
				.def(constructor<>())
				.def(constructor<float,float,float,float>())
				.def(constructor<float>())


				.def(self+float())
				.def(self-float())
				.def(self*float())
				.def(self/float())
				.def(self+other<SColor>())
				.def(self-other<SColor>())
				.def(self*other<SColor>())

				.def_readwrite("R",&SColor::R)
				.def_readwrite("G",&SColor::G)
				.def_readwrite("B",&SColor::B)
				.def_readwrite("A",&SColor::A)

				.def("getLuminance", &SColor::getLuminance)
				.def("setHSL", &SColor::setHSL)
				.def("saturate", &SColor::saturate)

		];
	}
	void ITextureSourceExport(lua_State* L)
	{
		module(L,"video")[
			class_<ITextureSource>("TextureSource")
				.def("GetTexture", &ITextureSource::GetTexture)
				.def("GetSourceType", &ITextureSource::GetSourceType)
				.def("Update", &ITextureSource::Update)
				.def("IsLoaded", &ITextureSource::IsLoaded)
				.def("Load", &ITextureSource::Load)
				.def("Unload", &ITextureSource::Unload)
		];
	}
	void RenderWindowExport(lua_State* L)
	{
		module(L,"video")[
			class_<RenderWindow>("RenderWindow")
				.property("name", &RenderWindow::GetName)
				.property("position", &RenderWindow::GetPosition, &RenderWindow::SetPosition)
				.property("colorDepth", &RenderWindow::GetColorDepth)
				.property("size", &RenderWindow::GetSize, &RenderWindow::Resize)
				.property("active", &RenderWindow::IsActive, &RenderWindow::SetActive)
				.property("deactivateOnFocusChange", &RenderWindow::IsDeactivateOnFocusChange, &RenderWindow::SetDeactivateOnFocusChange)
				
				.def("CreateViewport", &RenderWindow::CreateViewport)


				.def("GetName", &RenderWindow::GetName)
				.def("SetActiveWindow", &RenderWindow::SetActiveWindow)
				.def("SetFullScreen", &RenderWindow::SetFullScreen)
				.def("IsFullScreen", &RenderWindow::IsFullScreen)
				.def("Show", &RenderWindow::Show)
				.def("Hide", &RenderWindow::Hide)
				.def("IsVisible", &RenderWindow::IsVisible)
				.def("Show", &RenderWindow::Show)
				.def("IsClosed", &RenderWindow::IsClosed)
				.def("IsPrimary", &RenderWindow::IsPrimary)
				.def("TakeScreenShot", &RenderWindow::TakeScreenShot)
				.def("GetCustomParam", &RenderWindow::GetCustomParam)
				.def("GetInputManager", &RenderWindow::GetInputManager)
		];
	}
	void DeviceCapabilitesExport(lua_State* L)
	{
		module(L,"video")[
			class_<DeviceCapabilites>("DeviceCapabilites")
				.def("isFeatureSupported", &DeviceCapabilites::isFeatureSupported)
				.def("getDeviceName", &DeviceCapabilites::getDeviceName)
				.def("getVendor", &DeviceCapabilites::getVendor)
				.def("getMaxLightsUnits", &DeviceCapabilites::getMaxLightsUnits)
				.def("getMaxAnisotropy", &DeviceCapabilites::getMaxAnisotropy)
				.def("getMaxTextureUnits", &DeviceCapabilites::getMaxTextureUnits)
				.def("getMaxWorldMatrixUnits", &DeviceCapabilites::getMaxWorldMatrixUnits)
				.def("getStencilBufferBits", &DeviceCapabilites::getStencilBufferBits)
				.def("getNumVertexBlendMat", &DeviceCapabilites::getNumVertexBlendMat)
				.def("getNumMultiRenderTargets", &DeviceCapabilites::getNumMultiRenderTargets)
				.def("isShaderProfileSupported", &DeviceCapabilites::isShaderProfileSupported)
				.def("getShaderProfiles", &DeviceCapabilites::getShaderProfiles)
		];
	}
	void VideoDeviceExport(lua_State* L)
	{
		module(L,"video")[
			class_<IVideoDevice>("IVideoDevice")
				.property("clearColor",&IVideoDevice::getClearColor,&IVideoDevice::setClearColor)
				.property("multiSampling",&IVideoDevice::getMultiSampling,&IVideoDevice::setMultiSampling)
				.property("clearColor",&IVideoDevice::getClearColor,&IVideoDevice::setClearColor)
				.property("viewport",&IVideoDevice::getViewport,&IVideoDevice::setViewport)
				.property("scissorTest",&IVideoDevice::getScissorTest,&IVideoDevice::setScissorTest)
				.property("scissorRect",&IVideoDevice::getScissorRect,&IVideoDevice::setScissorRect)
				.property("scissorTest",&IVideoDevice::getScissorTest,&IVideoDevice::setScissorTest)
				.property("scissorTest",&IVideoDevice::getScissorTest,&IVideoDevice::setScissorTest)

				.def("getCapabilities", &IVideoDevice::getCapabilities)
				.def("CreateRenderWindow", &IVideoDevice::CreateRenderWindow)
				.def("getRenderTargetSize", &IVideoDevice::getRenderTargetSize)
				.def("setTransformationState", &IVideoDevice::setTransformationState)
				.def("getTransformationState", &IVideoDevice::getTransformationState)
				.def("setRenderState", &IVideoDevice::setRenderState)
				.def("getRenderState", &IVideoDevice::getRenderState)
				.def("SetClipPlane", &IVideoDevice::SetClipPlane)
				.def("AddClipPlane", &IVideoDevice::AddClipPlane)
				.def("RemoveClipPlane", &IVideoDevice::RemoveClipPlane)
				.def("ClearClipPlanes", &IVideoDevice::ClearClipPlanes)
				.def("createTexture1D", &IVideoDevice::createTexture1D)
				.def("createTexture2D", &IVideoDevice::createTexture2D)
				.def("createTexture3D", &IVideoDevice::createTexture3D)
				.def("createTextureCube", &IVideoDevice::createTextureCube)
				.def("createEmptyTexture1D", &IVideoDevice::createEmptyTexture1D)
				.def("createEmptyTexture2D", &IVideoDevice::createEmptyTexture2D)
				.def("createEmptyTexture3D", &IVideoDevice::createEmptyTexture3D)
				.def("createEmptyTextureCube", &IVideoDevice::createEmptyTextureCube)
				.def("set2DMode", &IVideoDevice::set2DMode)
				.def("set3DMode", &IVideoDevice::set3DMode)
				.def("setLineWidth", &IVideoDevice::setLineWidth)
				.def("setPointSize", &IVideoDevice::setPointSize)
				.def("draw2DRectangleRot", &IVideoDevice::draw2DRectangleRot)
				.def("useRenderPass", &IVideoDevice::useRenderPass)
				.def("useTexture", &IVideoDevice::useTexture)
				.def("disableTexture", &IVideoDevice::disableTexture)
				.def("useShader", &IVideoDevice::useShader)
				.def("unuseShader", &IVideoDevice::unuseShader)
				.def("createRenderTarget", &IVideoDevice::createRenderTarget)
				.def("setRenderTarget", &IVideoDevice::setRenderTarget)
				.def("getRenderTarget", &IVideoDevice::getRenderTarget)
				.def("createMRenderTarget", &IVideoDevice::createMRenderTarget)
				.def("setDepthBias", &IVideoDevice::setDepthBias)


		];
	}
	void IRenderAreaExport(lua_State*L)
	{
		module(L)[
			class_<IRenderArea>("IRenderArea")
				.def("getSize",&IRenderArea::getSize)
				.def("getColorTexture",&IRenderArea::getColorTexture)
				.def("GetParameter",&IRenderArea::GetParameter)
		];
	}

	void RenderTargetExport(lua_State*L)
	{
		static IRenderTarget* NullValue=0;
		module(L)[
			class_<IRenderTarget,IRenderArea>("IRenderTarget")
				.def("getDepthBuffer",&IRenderTarget::getDepthBuffer)
				.def("getStencilBuffer",&IRenderTarget::getStencilBuffer)
				.def("attachRenderTarget",&IRenderTarget::attachRenderTarget)
				.def("deattachRenderTarget",&IRenderTarget::deattachRenderTarget)
				.def("GetColorTextureCount",&IRenderTarget::GetColorTextureCount)
				.def("Resize",&IRenderTarget::Resize)
				.def("clear",&IRenderTarget::clear)
				.def("bind",&IRenderTarget::bind)
				.def("unbind",&IRenderTarget::unbind)
		];

		object table=globals(L)["IRenderTarget"];
		table["Null"]=object();
		table["Null"]=NullValue;
	}


	void VideoEnumExport(lua_State*L)
	{
		module(L,"video")[
			class_<EMeshRenderType>("MeshRenderType")
				.enum_("constants")
				[
					value("Points",MR_POINTS),
					value("Lines",MR_LINES),
					value("LineLoop",MR_LINE_LOOP),
					value("LineStrip",MR_LINE_STRIP),
					value("Triangles",MR_TRIANGLES),
					value("TriangleStrip",MR_TRIANGLE_STRIP),
					value("TriangleFan",MR_TRIANGLE_FAN),
					value("Quads",MR_QUADS),
					value("QuadStrip",MR_QUAD_STRIP),
					value("Polygon",MR_POLYGON)
				],
			class_<ETransformationState>("TransformationState")
				.enum_("constants")
				[
					value("View",TS_VIEW),
					value("Projection",TS_PROJECTION),
					value("World",TS_WORLD),
					value("Texture0",TS_TEXTURE0),
					value("Texture1",TS_TEXTURE1),
					value("Texture2",TS_TEXTURE2),
					value("Texture3",TS_TEXTURE3)
				],
			class_<ETextureType>("TextureType")
				.enum_("constants")
				[
					value("Tex1D",ETT_1DTex),
					value("Tex2D",ETT_2DTex),
					value("Tex3D",ETT_3DTex),
					value("TexCube",ETT_CubeTex)
				],
			class_<EPixelFormat>("PixelFormat")
				.enum_("constants")
				[
					value("Unkown",EPixel_Unkown),
					value("LUMINANCE8",EPixel_LUMINANCE8),
					value("LUMINANCE16",EPixel_LUMINANCE16),
					value("Alpha8",EPixel_Alpha8),
					value("Alpha4Luminance4",EPixel_Alpha4Luminance4),
					value("Alpha8Luminance8",EPixel_Alpha8Luminance8),
					value("R5G5B5",EPixel_R5G6B5),
					value("R8G8B8",EPixel_R8G8B8),
					value("R8G8B8A8",EPixel_R8G8B8A8),
					value("X8R8G8B8",EPixel_X8R8G8B8),
					value("B8G8R8",EPixel_B8G8R8),
					value("B8G8R8A8",EPixel_B8G8R8A8),
					value("X8B8G8R8",EPixel_X8B8G8R8),
					value("Float16_R",EPixel_Float16_R),
					value("Float16_RGB",EPixel_Float16_RGB),
					value("Float16_RGBA",EPixel_Float16_RGBA),
					value("Float16_GR",EPixel_Float16_GR),
					value("Float32_R",EPixel_Float32_R),
					value("Float32_RGB",EPixel_Float32_RGB),
					value("Float32_RGBA",EPixel_Float32_RGBA),
					value("Float32_GR",EPixel_Float32_GR),
					value("Depth",EPixel_Depth),
					value("Stencil",EPixel_Stecil),
					value("Short_RGBA",EPixel_Short_RGBA),
					value("Short_RGB",EPixel_Short_RGB),
					value("Short_GR",EPixel_Short_GR)
				],
			class_<EPixel_TYPE>("PixelType")
				.enum_("constants")
				[
					value("Byte",EPixelType_Byte),
					value("Short",EPixelType_Short),
					value("Float16",EPixelType_Float16),
					value("Float32",EPixelType_Float32)
				],
			class_<ETextureClamp>("TextureClamp")
				.enum_("constants")
				[
					value("Repeat",ETC_REPEAT),
					value("Clamp",ETC_CLAMP),
					value("ClampToEdge",ETC_CLAMP_TO_EDGE),
					value("ClampToBorder",ETC_CLAMP_TO_BORDER),
					value("Mirror",ETC_MIRROR)
				],
			class_<ETextureWrap>("ETextureWrap")
				.enum_("constants")
				[
					value("WrapS",ETW_WrapS),
					value("WrapT",ETW_WrapT),
					value("WrapR",ETW_WrapR)
				],
			class_<ETextureGenType>("TextureGenType")
				.enum_("constants")
				[
					value("GenS",ETGT_GenS),
					value("GenT",ETGT_GenT),
					value("GenR",ETGT_GenR),
					value("GenQ",ETGT_GenQ)
				],
			class_<ETextureGenMode>("TextureGenMode")
				.enum_("constants")
				[
					value("EyeLinear",ETGM_EyeLinear),
					value("ObjectLinear",ETGM_ObjectLinear),
					value("SphereMap",ETGM_SphereMap)
				],
			class_<EMeshStreamType>("MeshStreamType")
				.enum_("constants")
				[
					value("Position",EMST_Position),
					value("Normal",EMST_Normal),
					value("BlendIndicies",EMST_BlendIndicies),
					value("BlendWeights",EMST_BlendWeights),
					value("Color",EMST_Color),
					value("Specular",EMST_Specular),
					value("Tangent",EMST_Tangent),
					value("Binormal",EMST_Binormal),
					value("Texcoord",EMST_Texcoord)
				],
			class_<EStreamDataType>("StreamDataType")
				.enum_("constants")
				[
					value("Point1f",ESDT_Point1f),
					value("Point2f",ESDT_Point2f),
					value("Point3f",ESDT_Point3f),
					value("Point4f",ESDT_Point4f),
					value("Point1s",ESDT_Point1s),
					value("Point2s",ESDT_Point2s),
					value("Point3s",ESDT_Point3s),
					value("Point4s",ESDT_Point4s),
					value("Color4b",ESDT_Color4b)
				],
			class_<RenderStates>("RenderStates")
				.enum_("constants")
				[
					value("Wireframe",RS_Wireframe),
					value("Smooth",RS_Smooth),
					value("Lighting",RS_Lighting),
					value("ZTest",RS_ZTest),
					value("ZWrite",RS_ZWrite),
					value("Points",RS_Points),
					value("CullFace",RS_CullFace),
					value("AlphaTest",RS_AlphaTest),
					value("Fog",RS_Fog),
					value("Blend",RS_Blend)
				],
			class_<EState>("EState")
				.enum_("constants")
				[
					value("DontUse",ES_DontUse),
					value("Use",ES_Use),
					value("Unkown",ES_Unknown)
				],
			class_<ECullFaceMode>("CullFaceMode")
				.enum_("constants")
				[
					value("CW",ECFM_CW),
					value("CCW",ECFM_CCW)
				],
			class_<EBlendFactor>("BlendFactor")
				.enum_("constants")
				[
					value("Zero",EBF_ZERO),
					value("One",EBF_ONE),
					value("DstColor",EBF_DST_COLOR),
					value("OneMinusDstColor",EBF_ONE_MINUS_DST_COLOR),
					value("SrcColor",EBF_SRC_COLOR),
					value("OneMinusSrcColor",EBF_ONE_MINUS_SRC_COLOR),
					value("SrcAlpha",EBF_SRC_ALPHA),
					value("OneMinusSrcAlpha",EBF_ONE_MINUS_SRC_ALPHA),
					value("DstAlpha",EBF_DST_ALPHA),
					value("OneMinusDstAlpha",EBF_ONE_MINUS_DST_ALPHA),
					value("SrcAlphaSaturate",EBF_SRC_ALPHA_SATURATE)
				]
		];
	}

	void OpenVideo(lua_State*L)
	{
		RenderPassExport(L);
		RenderTechniqueExport(L);
		RenderMaterialExport(L);
		ITextureExport(L);
		TextureUnitExport(L);
		SColorExport(L);
		ITextureSourceExport(L);
		VideoEnumExport(L);
		DeviceCapabilitesExport(L);
		VideoDeviceExport(L);
		RenderWindowExport(L);
		IRenderAreaExport(L);
		RenderTargetExport(L);
	}

}
}
