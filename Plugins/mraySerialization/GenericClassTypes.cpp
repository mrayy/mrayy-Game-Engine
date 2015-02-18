
#include "stdafx.h"
#include "GenericClassTypes.h"

namespace mray
{
namespace serialize
{
	//core
	 GenericTypeClassSerialize<core::string>	g_stringClassSerialize(mT("string"));
	 GenericTypeClassSerialize<int>				g_intClassSerialize(mT("int"));
	 GenericTypeClassSerialize<uint>			g_uintClassSerialize(mT("uint"));
	 GenericTypeClassSerialize<float>			g_floatClassSerialize(mT("float"));
	 GenericTypeClassSerialize<double>			g_doubleClassSerialize(mT("double"));
	 GenericTypeClassSerialize<long>			g_longClassSerialize(mT("long"));
	 GenericTypeClassSerialize<ulong>			g_ulongClassSerialize(mT("ulong"));
	 GenericTypeClassSerialize<bool>			g_boolClassSerialize(mT("bool"));

	//math
	 GenericTypeClassSerialize<math::Plane>		g_planeClassSerialize(mT("plane"));
	 GenericTypeClassSerialize<math::box3d>		g_box3dClassSerialize(mT("box3d"));
	 GenericTypeClassSerialize<math::rectf>		g_rectfClassSerialize(mT("rectf"));
	 GenericTypeClassSerialize<math::line3d>	g_line3dClassSerialize(mT("line3d"));
	 GenericTypeClassSerialize<math::line2d>	g_line2dClassSerialize(mT("line2d"));
	 GenericTypeClassSerialize<math::matrix4x4>	g_matrix4x4ClassSerialize(mT("matrix4x4"));
	 GenericTypeClassSerialize<math::quaternion>g_quaternionClassSerialize(mT("quaternion"));
	 GenericTypeClassSerialize<math::vector2d>	g_vector2dClassSerialize(mT("vector2d"));
	 GenericTypeClassSerialize<math::vector2di>	g_vector2diClassSerialize(mT("vector2di"));
	 GenericTypeClassSerialize<math::vector3d>	g_vector3dClassSerialize(mT("vector3d"));
	 GenericTypeClassSerialize<math::vector3di>	g_vector3diClassSerialize(mT("vector3di"));
	 GenericTypeClassSerialize<math::vector4d>	g_vector4dClassSerialize(mT("vector4d"));
	 GenericTypeClassSerialize<math::vector4di>	g_vector4diClassSerialize(mT("vector4di"));

	 GenericTypeClassSerialize<video::SColor>	g_colorClassSerialize(mT("SColor"));
}
}