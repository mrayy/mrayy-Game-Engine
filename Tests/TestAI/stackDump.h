
/********************************************************************
	created:	2009/04/06
	created:	6:4:2009   20:56
	filename: 	i:\Programing\GameEngine\mrayEngine\TestAI\stackDump.h
	file path:	i:\Programing\GameEngine\mrayEngine\TestAI
	file base:	stackDump
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___stackDump___
#define ___stackDump___

#include <Windows.h>

namespace mray{

class stackDump
{
private:
protected:
	void inner_dumpStack(DWORD64 *trace,int length);
public:
	stackDump();
	virtual~stackDump();

	void dumpStack();
};

}


#endif //___stackDump___
