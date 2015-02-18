

/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   16:45
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGIStatement.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGIStatement
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGIStatement__
#define __SGIStatement__

#include "SGCommon.h"
#include <vector>

namespace mray
{
namespace shader
{
class SGIVisitor;

class SGIStatement
{
private:
protected:
	core::string m_desc;
public:
	SGIStatement();
	virtual~SGIStatement();

	void SetDescription(const core::string&desc){m_desc=desc;}
	const core::string& GetDescription()const {return m_desc;}

	virtual void Visit(SGIVisitor*visitor)=0;
	virtual bool Load(xml::XMLElement* elem)=0;
	virtual xml::XMLElement* Save(xml::XMLElement* elem)=0;

	static SGIStatement* CreateStatement(const core::string& name);
};

MakeSharedPtrType(SGIStatement);

}
}

#endif
