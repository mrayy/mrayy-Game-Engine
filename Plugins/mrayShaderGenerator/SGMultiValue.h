


/********************************************************************
	created:	2011/09/17
	created:	17:9:2011   17:08
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGMultiValue.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGMultiValue
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGMultiValue__
#define __SGMultiValue__

#include "SGValue.h"

namespace mray
{
namespace shader
{

class SGMultiValue:public SGValue
{
private:
protected:
	video::EUniformType GetSingleType(video::EUniformType t);
public:
	SGMultiValue(video::EUniformType type,const core::string&v1);
	SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2);
	SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2,const core::string&v3);
	SGMultiValue(video::EUniformType type,const core::string&v1,const core::string&v2,const core::string&v3,const core::string&v4);
	virtual~SGMultiValue();

};

}
}

#endif
