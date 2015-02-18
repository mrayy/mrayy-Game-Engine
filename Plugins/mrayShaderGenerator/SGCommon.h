

/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   8:51
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGCommon.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGCommon
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGCommon__
#define __SGCommon__

#include "GCPtr.h"

namespace mray
{
namespace shader
{

	enum EVariableUsage
	{
		EVU_NORMAL,
		EVU_UNIFORM,
		EVU_INPUT,
		EVU_OUTPUT,
		EVU_COUNT
	};
	enum EFunctionType
	{
		EFT_DEFAULT,
		EFT_MAIN_VERTEX,
		EFT_MAIN_FRAGMENT
	};
	enum EProgramType
	{
		EPT_VERTEX,
		EPT_FRAGMENT,
		EPT_GEOMETRY,
		EPT_COUNT
	};
	enum EUniformSemantic
	{
		EUS_UNKOWN,

		EUS_POSITION,
		EUS_NORMAL,
		EUS_TEXCOORD,
		EUS_COLOR,
		EUS_BLEND_WEIGHTS,
		EUS_BLEND_INDICIES,
		EUS_BINORMAL,
		EUS_TANGENT
	};


	enum EOperationType
	{
		EOT_ADD,
		EOT_SUB,
		EOT_MULT,
		EOT_DIV,
		EOT_ASSIGN,
		EOT_COUNT
	};

	extern const core::string g_varUsageString[];
}
}

#endif
