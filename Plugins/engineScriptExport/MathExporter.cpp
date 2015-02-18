#include "Stdafx.h"
#include "LuaHelpers.h"
#include "StringConverter.h"


using namespace mray;
using namespace math;

std::ostream& operator<<( std::ostream& stream, const Point2d<int>& ent );
std::ostream& operator<<( std::ostream& stream, const Point2d<float>& ent );
std::ostream& operator<<( std::ostream& stream, const Point3d<int>& ent );
std::ostream& operator<<( std::ostream& stream, const Point3d<float>& ent );
std::ostream& operator<<( std::ostream& stream, const quaternion& ent );


#include "luabind/operator.hpp"

using namespace luabind;

namespace mray{
namespace script{

	using namespace math;

template<typename T>
void Vector2DExporter(lua_State*L,const char* name)
{

#define PT Point2d<T>



	module(L)[
	class_<PT>(name)

		.def_readwrite("x",&PT::x)
		.def_readwrite("y",&PT::y)

		.def(constructor<>())
		.def(constructor<T>())
		.def(constructor<T,T>())
		.def(constructor<PT>())
		.def(tostring(self))
		

		.def(self+PT())
		.def(self-PT())
		.def(self*PT())
		.def(self/PT())
		.def(self+T())
		.def(self-T())
		.def(self*T())
		.def(self/T())

		.def("Length",&PT::Length)
		.def("LengthSq",&PT::LengthSQ)
		.def("dotProduct",&PT::dotProduct)
		.def("getDistSQ",&PT::getDistSQ)
		.def("getDist",&PT::getDist)
		.def("Normalize",&PT::Normalize)
		.def("set",&PT::set)
	];

	LUA_CONST_START(name,L)
		LUA_CONST(name,PT,Zero);
		LUA_CONST(name,PT,XAxis);
		LUA_CONST(name,PT,YAxis);
	LUA_CONST_END;

#undef  PT
}

template<typename T>
void Vector3DExporter(lua_State*L,const char* name)
{
#define PT Point3d<T>
	module(L)[
		class_<PT>(name)

			.def_readwrite("x",&PT::x)
			.def_readwrite("y",&PT::y)
			.def_readwrite("z",&PT::z)

			.def(constructor<>())
			.def(constructor<T>())
			.def(constructor<T,T,T>())
			.def(constructor<PT>())
			.def(tostring(self))


			.def(self+other<PT>())
			.def(self-other<PT>())
			.def(self*other<PT>())
			.def(self/other<PT>())
			.def(self+T())
			.def(self-T())
			.def(self*T())
			.def(self/T())

			.def("Length",&PT::Length)
			.def("LengthSq",&PT::LengthSQ)
			.def("dotProduct",&PT::dotProduct)
			.def("getDistSQ",&PT::getDistSQ)
			.def("getDist",&PT::getDist)
			.def("Normalize",&PT::Normalize)
			.def("set",&PT::set)
	];

	LUA_CONST_START(name,L)
		LUA_CONST(name,PT,Zero);
		LUA_CONST(name,PT,XAxis);
		LUA_CONST(name,PT,YAxis);
		LUA_CONST(name,PT,ZAxis);
	LUA_CONST_END;

#undef  PT

}

template<typename T>
void RectExporter(lua_State*L,const char* name)
{
#define RC rect<T>
#define PT Point2d<T>
	module(L)[
		class_<RC>(name)

			.def_readwrite("ULPoint",&RC::ULPoint)
			.def_readwrite("BRPoint",&RC::BRPoint)

			.def(constructor<>())
			.def(constructor<T,T,T,T>())
			.def(constructor<const PT&,const PT&>())
			.def(constructor<const RC&>())

			.def(self+other<PT>())
			.def(self-other<PT>())
			.def(self+other<RC>())
			.def(self-other<RC>())

			.def("IsPointInside",&RC::IsPointInside)
			.def("IsRectCollide",&RC::IsRectCollide)
			.def("IsRectInside",&RC::IsRectInside)
			.def("Repair",&RC::Repair)
			.def("clipRect",&RC::clipRect)
			.def("Center",&RC::getCenter)
			.def("Height",&RC::getHeight)
			.def("Width",&RC::getWidth)
			.def("Size",&RC::getSize)
			.def("isValid",&RC::isValid)
			.def("reset",&RC::reset)
			.def("addPoint",&RC::addPoint)
	];
#undef  PT
#undef  RC


}

void QuaternionExporter(lua_State*L)
{
#define name "Quaternion"
#define Q quaternion
	typedef float T;

	module(L)[
		class_<Q>(name)

			.def_readwrite("x",&Q::x)
			.def_readwrite("y",&Q::y)
			.def_readwrite("z",&Q::z)
			.def_readwrite("w",&Q::w)

			.def(constructor<>())
			.def(constructor<T,T,T>())
			.def(constructor<T,T,T,T>())
			.def(constructor<T,vector3d>())
			.def(constructor<vector3d>())
			.def(constructor<matrix4x4>())
			.def(constructor<Q>())
			.def(tostring(self))


			.def(self*other<Q>())
			.def(self*T())
			.def(self*other<vector3d>())

			.def("LengthSq",&Q::LengthSQ)
			.def("dotProduct",&Q::dotProduct)
			.def("inverse",&Q::inverse)
			.def("fromAngle",&Q::fromAngle)
			.def("inverse",&Q::inverse)
			.def("log",&Q::log)
			.def("exp",&Q::exp)
			.def("fromAxis",&Q::fromAxis)
			.def("getXAxis",&Q::getXAxis)
			.def("getYAxis",&Q::getYAxis)
			.def("getZAxis",&Q::getZAxis)
			.def("fromMatrix",&Q::fromMatrix)
			.def("toMatrix",&Q::toMatrix)
			.def("getPitch",&Q::getPitch)
			.def("getYaw",&Q::getYaw)
			.def("getRoll",&Q::getRoll)
			.def("Slerp",&Q::Slerp)
			.def("Squad",&Q::Squad)
	];

	LUA_CONST_START(name,L)
		LUA_CONST(name,Q,Identity);
	LUA_CONST_END;

#undef  PT

}
void Matrix4x4Exporter(lua_State*L)
{
#define name "Matrix4x4"
#define M matrix4x4
	typedef float T;

	module(L)[
		class_<M>(name)

			.def_readwrite("f11",&M::f11)
			.def_readwrite("f12",&M::f12)
			.def_readwrite("f13",&M::f13)
			.def_readwrite("f14",&M::f14)
			.def_readwrite("f21",&M::f21)
			.def_readwrite("f22",&M::f22)
			.def_readwrite("f23",&M::f23)
			.def_readwrite("f24",&M::f24)
			.def_readwrite("f31",&M::f31)
			.def_readwrite("f32",&M::f32)
			.def_readwrite("f33",&M::f33)
			.def_readwrite("f34",&M::f34)
			.def_readwrite("f41",&M::f41)
			.def_readwrite("f42",&M::f42)
			.def_readwrite("f43",&M::f43)
			.def_readwrite("f44",&M::f44)

			.def(constructor<>())
			.def(constructor<M>())
			
			.def(self+other<M>())
			.def(self-other<M>())
			.def(self*other<M>())
			.def(self*T())
			.def(self*other<vector3d>())

			.def(self*M())
			.def("GetInverse",&M::getInverse)
			.def("makeIdentity",&M::makeIdentity)
			.def("isIdentity",&M::isIdentity)
			.def("getTransposed",&M::getTransposed)
			.def("inverseTransformVector",&M::inverseTransformVector)
			.def("transformNormal",&M::transformNormal)
			.def("rotateX",&M::rotateX)
			.def("rotateY",&M::rotateY)
			.def("rotateZ",&M::rotateZ)
			.def("getRotation",&M::getRotation)
			.def("setTranslation",&M::setTranslation)
			.def("setScale",&M::setScale)
			.def("getTranslation",&M::getTranslation)
			.def("setComponent",&M::setComponent)
	];

	LUA_CONST_START(name,L)
		LUA_CONST(name,M,Identity);
	LUA_CONST_END;

#undef  PT
}
void Matrix3x3Exporter(lua_State*L)
{
#define name "Matrix3x3"
#define M matrix3x3
	typedef float T;

	module(L)[
		class_<M>(name)

			.def_readwrite("f11",&M::f11)
			.def_readwrite("f12",&M::f12)
			.def_readwrite("f13",&M::f13)
			.def_readwrite("f21",&M::f21)
			.def_readwrite("f22",&M::f22)
			.def_readwrite("f23",&M::f23)
			.def_readwrite("f31",&M::f31)
			.def_readwrite("f32",&M::f32)
			.def_readwrite("f33",&M::f33)

			.def(constructor<>())
			.def(constructor<M>())
			
			.def(self+other<M>())
			.def(self-other<M>())
			.def(self*other<M>())
			.def(self*T())

			.def(self*M())
			.def("makeIdentity",&M::makeIdentity)
			.def("isIdentity",&M::isIdentity)
			.def("setAngle",&M::setAngle)
			.def("getAngle",&M::getAngle)
			.def("translate",&M::translate)
			.def("getTranslate",&M::getTranslate)
	];

	LUA_CONST_START(name,L)
		LUA_CONST(name,M,Identity);
	LUA_CONST_END;

#undef  PT
}
void OpenMath(lua_State*L)
{
	Vector2DExporter<float>(L,"Vector2d");
	Vector2DExporter<int>(L,"Vector2di");
	Vector3DExporter<float>(L,"Vector3d");
	Vector3DExporter<int>(L,"Vector3di");
	Matrix3x3Exporter(L);
	Matrix4x4Exporter(L);
	QuaternionExporter(L);
	RectExporter<int>(L,"recti");
	RectExporter<float>(L,"rectf");
}

}
}

std::ostream& operator<<( std::ostream& stream, const Point2d<int>& ent )
{
	stream<<core::StringConverter::toString(ent).c_str();
	return stream;
}
std::ostream& operator<<( std::ostream& stream, const Point2d<float>& ent )
{
	stream<<core::StringConverter::toString(ent).c_str();
	return stream;
}
std::ostream& operator<<( std::ostream& stream, const Point3d<int>& ent )
{
	stream<<core::StringConverter::toString(ent).c_str();
	return stream;
}
std::ostream& operator<<( std::ostream& stream, const Point3d<float>& ent )
{
	stream<<core::StringConverter::toString(ent).c_str();
	return stream;
}
std::ostream& operator<<( std::ostream& stream, const quaternion& ent )
{
	stream<<core::StringConverter::toString(ent).c_str();
	return stream;
}