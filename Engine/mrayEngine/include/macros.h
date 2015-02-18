



#include "CException.h"
#include "StringConverter.h"

namespace mray{

#define FATAL_ERROR(condition,msg) if(condition){core::CException e(core::stringc(__FILE__).c_str(),core::StringConverter::toString(__LINE__).c_str(),msg);e.runError();}

}





