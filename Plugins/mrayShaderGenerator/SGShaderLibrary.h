
/********************************************************************
	created:	2011/11/16
	created:	16:11:2011   11:05
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGShaderLibrary.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGShaderLibrary
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGShaderLibrary__
#define __SGShaderLibrary__


namespace mray
{
namespace shader
{

class SGFunctionPrototype;

class SGShaderLibrary
{
public:
	typedef std::list<SGFunctionPrototype*> FunctionsList;
private:
protected:
	core::string m_name;
	core::string m_description;

	FunctionsList m_functions;
public:
	SGShaderLibrary();
	virtual~SGShaderLibrary();

	const core::string& GetName()const{return m_name;}
	void SetName(const core::string& n){m_name=n;}

	const core::string& GetDescription()const{return m_description;}
	void SetDescription(const core::string& d){m_description=d;}


	void AddFunction(SGFunctionPrototype*f);
	const FunctionsList& GetFunctions()const{return m_functions;}

	void ClearFunctions();

	void Load(xml::XMLElement* elem);
	xml::XMLElement* Save(xml::XMLElement* elem);
};


}
}

#endif