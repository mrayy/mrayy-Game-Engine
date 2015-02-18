

/********************************************************************
	created:	2011/09/21
	created:	21:9:2011   14:44
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGXMLVisitor.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGXMLVisitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGXMLVisitor__
#define __SGXMLVisitor__


#include "SGIVisitor.h"

namespace mray
{
namespace xml
{
	class XMLElement;
}
namespace shader
{

class SGXMLVisitor:public SGIVisitor
{
private:
protected:
	xml::XMLElement* m_current;
	xml::XMLElement* m_element;
public:
	SGXMLVisitor();
	~SGXMLVisitor();

	xml::XMLElement* Traverse(SGProgram*program);

	void OnVisit(SGProgram*v);

	void OnVisit(SGFunction*v);

	void OnVisit(SGFunctionCall*v);
	void OnVisit(SGOperation*v);
	void OnVisit(SGForStatement*v);

	void OnVisit(SGValue*v);
	void OnVisit(SGVariable*v);
	void OnVisit(SGDirectValue*v);

};

}
}

#endif
