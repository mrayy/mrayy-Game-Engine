

/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   16:45
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGOperation.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGOperation
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGOperation__
#define __SGOperation__

#include "SGValueStatement.h"

namespace mray
{
namespace shader
{

class SGOperation:public SGValueStatement
{
public:
private:
protected:
	EOperationType m_opType;
public:
	SGOperation(EOperationType op,video::EUniformType resType);

	//binary operation
	SGOperation(EOperationType op,SGIValueCRef v1,SGIValueCRef v2);
	//assign operation
	SGOperation(SGIValueCRef o,SGIValueCRef v);

	virtual~SGOperation();

	EOperationType GetOperation()const{return m_opType;}

	virtual void Visit(SGIVisitor*visitor);
	virtual bool Load(xml::XMLElement* elem);
	virtual xml::XMLElement* Save(xml::XMLElement* elem);
};

}
}

#endif
