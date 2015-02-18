
#include "stdafx.h"
#include "SGIValue.h"
#include "SGValue.h"
#include "SGVariable.h"
#include "SGDirectValue.h"

namespace mray
{
namespace shader
{
	SGIValue* SGIValue::CreateValue(const core::string&type)
	{
		if(type.equals_ignore_case(mT("Value")))
			return new SGValue();
		if(type.equals_ignore_case(mT("Variable")))
			return new SGVariable(mT(""),video::EUT_Unkown);
		if(type.equals_ignore_case(mT("DirectValue")))
			return new SGDirectValue(video::EUT_Unkown,mT(""));
		return 0;
	}
}
}
