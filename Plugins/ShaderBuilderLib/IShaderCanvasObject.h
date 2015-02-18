

/********************************************************************
	created:	2011/12/03
	created:	3:12:2011   14:35
	filename: 	d:\Development\mrayEngine\Engine\ShaderBuilderLib\IShaderCanvasObject.h
	file path:	d:\Development\mrayEngine\Engine\ShaderBuilderLib
	file base:	IShaderCanvasObject
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __IShaderCanvasObject__
#define __IShaderCanvasObject__

#include <ICanvasObject.h>

namespace mray
{
namespace canvas
{

class IShaderCanvasObject:public ICanvasObject
{
private:
protected:
public:
	IShaderCanvasObject();
	virtual~IShaderCanvasObject();

};

}
}

#endif

