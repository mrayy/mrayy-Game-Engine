


/********************************************************************
	created:	2011/09/20
	created:	20:9:2011   8:56
	filename: 	d:\Development\mrayEngine\Engine\mraySerialization\GenericClassTypes.h
	file path:	d:\Development\mrayEngine\Engine\mraySerialization
	file base:	GenericClassTypes
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __GenericClassTypes__
#define __GenericClassTypes__

#include "GenericTypeClassSerialize.h"

namespace mray
{
namespace serialize
{
	//core
	extern GenericTypeClassSerialize<core::string>	g_stringClassSerialize;
	extern GenericTypeClassSerialize<int>				g_intClassSerialize;
	extern GenericTypeClassSerialize<uint>			g_uintClassSerialize;
	extern GenericTypeClassSerialize<float>			g_floatClassSerialize;
	extern GenericTypeClassSerialize<double>			g_doubleClassSerialize;
	extern GenericTypeClassSerialize<long>			g_longClassSerialize;
	extern GenericTypeClassSerialize<ulong>			g_ulongClassSerialize;
	extern GenericTypeClassSerialize<bool>			g_boolClassSerialize;

	//math
	extern GenericTypeClassSerialize<math::Plane>		g_planeClassSerialize;
	extern GenericTypeClassSerialize<math::box3d>		g_box3dClassSerialize;
	extern GenericTypeClassSerialize<math::rectf>		g_rectfClassSerialize;
	extern GenericTypeClassSerialize<math::line3d>	g_line3dClassSerialize;
	extern GenericTypeClassSerialize<math::line2d>	g_line2dClassSerialize;
	extern GenericTypeClassSerialize<math::matrix4x4>	g_matrix4x4ClassSerialize;
	extern GenericTypeClassSerialize<math::quaternion>g_quaternionClassSerialize;
	extern GenericTypeClassSerialize<math::vector2d>	g_vector2dClassSerialize;
	extern GenericTypeClassSerialize<math::vector2di>	g_vector2diClassSerialize;
	extern GenericTypeClassSerialize<math::vector3d>	g_vector3dClassSerialize;
	extern GenericTypeClassSerialize<math::vector3di>	g_vector3diClassSerialize;
	extern GenericTypeClassSerialize<math::vector4d>	g_vector4dClassSerialize;
	extern GenericTypeClassSerialize<math::vector4di>	g_vector4diClassSerialize;

	extern GenericTypeClassSerialize<video::SColor>	g_colorClassSerialize;
}
}

#endif
