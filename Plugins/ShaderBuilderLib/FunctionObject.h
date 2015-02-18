


/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   14:32
	filename: 	d:\Development\mrayEngine\Engine\ShaderBuilderLib\FunctionObject.h
	file path:	d:\Development\mrayEngine\Engine\ShaderBuilderLib
	file base:	FunctionObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __FunctionObject__
#define __FunctionObject__

#include "IShaderCanvasObject.h"

namespace mray
{
namespace shader
{
	class SGFunction;
}
namespace canvas
{
	class ShaderConnector;

class FunctionObject:public IShaderCanvasObject
{
private:
protected:
	SGFunction* m_function;
	
	TextShape* m_nameShape;

	std::vector<ShaderConnector*> m_inputs;
	std::vector<ShaderConnector*> m_outputs;
public:
	FunctionObject();
	virtual~FunctionObject();

	void SetFunction(SGFunction* func);
	SGFunction* GetFunction();

};

}
}

#endif
