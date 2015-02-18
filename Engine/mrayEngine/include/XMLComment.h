


/********************************************************************
	created:	2011/09/19
	created:	19:9:2011   16:26
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\XMLComment.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	XMLComment
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __XMLComment__
#define __XMLComment__

#include "IXMLNode.h"


namespace mray
{
namespace xml
{

class XMLComment:public IXMLNode
{
private:
protected:
	core::string m_value;
public:
	XMLComment(const core::string& val):m_value(val)
	{}
	virtual~XMLComment(){}

	virtual ENodeType GetType()const{return ENT_Comment;}

	void SetValue(const core::string& val){m_value=val;}
	const core::string& GetValue()const{return m_value;}
};


}
}

#endif
