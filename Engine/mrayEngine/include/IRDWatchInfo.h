
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   19:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IRDWatchInfo.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IRDWatchInfo
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___IRDWatchInfo___
#define ___IRDWatchInfo___

#include "mstring.h"

namespace mray{
namespace debug{

class MRAY_DLL IRDWatchInfo
{
private:
protected:
public:
	virtual uint getID()=0;
	virtual const core::string& getName()=0;
	virtual core::string getVal()=0;
	virtual const core::string& getGroup()=0;

};

}
}


#endif //___IRDWatchInfo___
