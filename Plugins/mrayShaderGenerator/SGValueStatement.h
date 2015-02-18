

/********************************************************************
	created:	2011/09/15
	created:	15:9:2011   16:28
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGValueStatement.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGValueStatement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGValueStatement__
#define __SGValueStatement__

#include "SGIValue.h"
#include "SGIStatement.h"
#include "SGVariable.h"

namespace mray
{
namespace shader
{

class SGValueStatement:public SGIValue,public SGIStatement
{
private:
protected:
	std::vector<SGIValuePtr> m_inputValues;
	std::vector<SGVariablePtr> m_outputValues;

public:
	SGValueStatement(video::EUniformType type=video::EUT_Unkown);
	virtual~SGValueStatement();

	virtual bool IsConst()const{return true;}

	void AddInputValue(SGIValueCRef val);
	bool AddOutputValue(SGVariablePtr val);

	const std::vector<SGIValuePtr>& GetInputValues()const;
	const std::vector<SGVariablePtr>& GetOutputValues()const;

	virtual bool Load(xml::XMLElement* elem);
	virtual xml::XMLElement* Save(xml::XMLElement* elem);
};

}
}

#endif
