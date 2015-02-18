

#include "stdafx.h"

#include "LuaHelpers.h"
#include "StringConverter.h"
#include "luabind/operator.hpp"

using namespace luabind;

namespace mray{
using namespace core;
namespace script{

template<typename T>
void StringExport(lua_State* L,const char* name)
{
#define STR tstring<T>

	module(L)[
		class_<std::string>("std_string"),
		class_<STR,std::string>(name)
			.def(constructor<>())
			.def(constructor<const T*>())
			/*.def(constructor<const std::string&>())
			
			.def(self+STR())
			.def("compare",&STR::compare_ignore_case)
			.def("length",&STR::length)
			.def("c_str",&STR::c_str)
			.def("make_lower",&STR::make_lower)
			.def("make_upper",&STR::make_upper)
			.def("equals_ignore_case",&STR::equals_ignore_case)
			.def("findFirstNotInList",&STR::findFirstNotInList)
			.def("findlastNotInList",&STR::findlastNotInList)*/
	];

#undef STR
}

void OpenCore(lua_State*L)
{
	StringExport<char>(L,"string");
//	StringExport<wchar_t>(L,"stringw");
}

}
}