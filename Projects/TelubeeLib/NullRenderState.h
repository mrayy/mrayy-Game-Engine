

/********************************************************************
	created:	2013/02/03
	created:	3:2:2013   13:10
	filename: 	C:\Development\mrayEngine\Projects\TELUBee\NullRenderState.h
	file path:	C:\Development\mrayEngine\Projects\TELUBee
	file base:	NullRenderState
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __NullRenderState__
#define __NullRenderState__


#include "IRenderingState.h"

namespace mray
{
namespace TBee
{

class NullRenderState:public IRenderingState
{
protected:
public:
	NullRenderState():IRenderingState("Null")
	{m_exitCode=STATE_EXIT_CODE;}
	virtual~NullRenderState(){}
};

}
}


#endif
