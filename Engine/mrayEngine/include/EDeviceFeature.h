
#ifndef ___EDeviceFeature___
#define ___EDeviceFeature___



namespace mray{
namespace video{

enum EDeviceFeature{
	//! Is driver able to render to a surface?
	EDF_RenderToTarget= 0,

	//! Are multiple textures per material possible?
	EDF_MultiTexture,

	//! Is driver able to render with a bilinear filter applied?
	EDF_BilinearFilter,

	//! Can the driver handle mip maps?
	EDF_MIPMAP,

	//! Can the driver update mip maps automatically?
	EDF_MIPMAP_AUTO_UPDATE,

	//! Are stencilbuffers switched on and does the device support stencil buffers?
	EDF_StencilBuffer,

	//! Are ARB vertex programs supported?
	EDF_VertexProgram,

	//! Are ARB fragment programs supported?
	EDF_FragmentProgram,

	EDF_GeometryProgram,

	//! is VBO Supported
	EDF_VertexBufferObject,


	//! Are non-power-of-two textures supported?
	EDF_TextureNP2,

	//! Are framebuffer objects supported?
	EDF_FrameBufferObject,

	//! Is Float Textures supported?
	EDF_FloatTexture,

	//! Is DXT compressed Textures supported?
	EDF_DXT_Compression,

	//! is Packed Depth+Stencil supported
	EDF_PackedDepthStencil,

	EDF_SeperateStencilBuffer,

	EDF_MultiSample,

	EDF_SeparateSpecularColor,

	EDF_CubeMapping,

	EDF_ScissorTest,

	EDF_Texture3D,

	EDF_PointSprites,

	EDF_Anisotropy,

	EDF_OcclusionQuery,

	EDF_DOT3,

	//! Auto Generate Mipmaps
	EDF_AutoMipmap, 

	EDF_Blending,

	EDF_COUNT
};

}
}


#endif



