


/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   11:07
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGFunctionParameterProtoype.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGFunctionParameterProtoype
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGFunctionParameterProtoype__
#define __SGFunctionParameterProtoype__

#include "SGCommon.h"

namespace mray
{
namespace shader
{

class SGFunctionParameterProtoype
{
private:
protected:
	core::string m_name;
	core::string m_description;
	EVariableUsage m_usage;
	video::EUniformType m_type;
public:
	SGFunctionParameterProtoype(EVariableUsage usage);
	virtual~SGFunctionParameterProtoype();

	const core::string& GetName()const{return m_name;}
	void SetName(const core::string& n){m_name=n;}

	video::EUniformType GetType()const{return m_type;}
	void SetType(video::EUniformType v){m_type=v;}

	const core::string& GetDescription()const{return m_description;}
	void SetDescription(const core::string& d){m_description=d;}

	void Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);

};

}
}

#endif
