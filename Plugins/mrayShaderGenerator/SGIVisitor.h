

/********************************************************************
	created:	2011/09/14
	created:	14:9:2011   16:39
	filename: 	d:\Development\mrayEngine\Engine\mrayShaderGenerator\SGIVisitor.h
	file path:	d:\Development\mrayEngine\Engine\mrayShaderGenerator
	file base:	SGIVisitor
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __SGIVisitor__
#define __SGIVisitor__


namespace mray
{
namespace shader
{

class SGProgram;
class SGFunction;
class SGFunctionCall;
class SGOperation;
class SGValue;
class SGVariable;
class SGForStatement;
class SGDirectValue;

class SGIVisitor
{
private:
protected:
public:
	SGIVisitor(){}
	virtual~SGIVisitor(){}

	virtual void OnVisit(SGProgram*v)=0;

	virtual void OnVisit(SGFunction*v)=0;

	virtual void OnVisit(SGFunctionCall*v)=0;
	virtual void OnVisit(SGOperation*v)=0;
	virtual void OnVisit(SGForStatement*v)=0;

	virtual void OnVisit(SGValue*v)=0;
	virtual void OnVisit(SGVariable*v)=0;
	virtual void OnVisit(SGDirectValue*v)=0;

};

}
}

#endif
