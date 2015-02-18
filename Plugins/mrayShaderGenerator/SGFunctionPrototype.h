


/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   11:06
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGFunctionPrototype.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGFunctionPrototype
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGFunctionPrototype__
#define __SGFunctionPrototype__


namespace mray
{
namespace shader
{

	class SGShaderLibrary;
	class SGFunctionParameterProtoype;

class SGFunctionPrototype
{
public:
	typedef std::list<SGFunctionParameterProtoype*> ParamsList;
private:
protected:

	core::string m_name;
	core::string m_description;

	ParamsList m_inputParams;
	ParamsList m_outputParams;

	video::EUniformType m_retType;

	SGShaderLibrary* m_ownerLib;

public:
	SGFunctionPrototype(SGShaderLibrary* lib);
	virtual~SGFunctionPrototype();


	const core::string& GetName()const{return m_name;}
	void SetName(const core::string& n){m_name=n;}

	video::EUniformType GetReturnType()const{return m_retType;}
	void SetReturnType(video::EUniformType v){m_retType=v;}

	const core::string& GetDescription()const{return m_description;}
	void SetDescription(const core::string& d){m_description=d;}

	const ParamsList& GetInputParams()const{return m_inputParams;}
	void SetInputParams(const ParamsList& p){m_inputParams=p;}

	const ParamsList& GetOutputParams()const{return m_outputParams;}
	void SetOutputParams(const ParamsList& p){m_outputParams=p;}

	bool Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};

}
}

#endif
