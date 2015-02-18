


/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   10:36
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGDefaultFunctions.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGDefaultFunctions
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGDefaultFunctions__
#define __SGDefaultFunctions__


namespace mray
{
namespace shader
{

#define SGLIB_COMMON 						"slib_common"
#define SGLIB_TEXTURE 						"slib_texture"
#define SGLIB_TRANSFORM 					"slib_transform"
#define SGLIB_LIGHTING 						"slib_lighting"


	// in SGLIB_COMMON library
#define SGFUNC_CONSTRUCT 					"slib_construct"
#define SGFUNC_LERP 						"slib_lerp"
#define SGFUNC_DOT							"slib_dot"

	// in SGLIB_TRANSFORM
#define SGFUNC_TRANSFORM					"slib_Transform"

	// in SGLIB_TEXTURE
#define SGFUNC_TC_TRANSFORM					"slib_transformTexcoord"
#define SGFUNC_TC_GEN_ENVMAP_NORMAL			"slib_generateTexcoord_EnvMap_Normal"
#define SGFUNC_TC_GEN_ENVMAP_SPHERE			"slib_generateTexcoord_EnvMap_Sphere"
#define SGFUNC_TC_GEN_ENVMAP_REFLECT		"slib_generateTexcoord_EnvMap_Reflect"
#define SGFUNC_TC_GEN_ENVMAP_PROJECTION		"slib_generateTexcoord_Projection"
#define SGFUNC_TC_SAMPLE					"slib_sampleTexture"
#define SGFUNC_TC_SAMPLE_PROJ				"slib_sampleTextureProj"


	// in SGLIB_LIGHTING
#define SGFUNC_LIGHT_DIR_DIFFUSE			"slib_Light_Directional_Diffuse"
#define SGFUNC_LIGHT_DIR_SPECULAR			"slib_Light_Directional_Specular"
#define SGFUNC_LIGHT_POINT_DIFFUSE			"slib_Light_Point_Diffuse"
#define SGFUNC_LIGHT_POINT_SPECULAR			"slib_Light_Point_Specular"


}
}

#endif

