
/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   15:03
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGVariable.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGVariable
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGVariable__
#define __SGVariable__

#include "SGIValue.h"

namespace mray
{
namespace shader
{


class SGVariable:public SGIValue
{
private:
protected:
	bool m_isConst;
	EVariableUsage m_usage;
	core::string m_name;
	ushort m_index;
	core::string m_desc;
	EUniformSemantic m_semantic;
public:
	SGVariable(const core::string& name,video::EUniformType type,EVariableUsage usage=EVU_NORMAL,
		EUniformSemantic semantic=EUS_UNKOWN,ushort index=0,const core::string&desc=mT(""));
	virtual~SGVariable();

	const core::string& GetDescription()const{return m_desc;}
	EUniformSemantic GetSemantic()const{return m_semantic;}


	bool IsConst()const{return m_isConst;}
	void SetConst(bool c){m_isConst=c;}

	const core::string& GetName()const{return m_name;}

	ushort GetIndex()const{return m_index;}

	EVariableUsage GetUsage()const{return m_usage;}

	void Visit(SGIVisitor*visitor);
	virtual bool Load(xml::XMLElement* elem);
	virtual xml::XMLElement* Save(xml::XMLElement* elem);

};

MakeSharedPtrType(SGVariable);


}
}

#endif