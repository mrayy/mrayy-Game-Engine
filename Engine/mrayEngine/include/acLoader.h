
/********************************************************************
	created:	2009/03/18
	created:	18:3:2009   20:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\acLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	acLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___acLoader___
#define ___acLoader___


#include "IMeshLoader.h"
#include "IMeshBuffer.h"


namespace mray{
namespace loaders{

class acLoader:public IMeshLoader
{
private:
protected:
public:
	acLoader();
	virtual~acLoader();

	virtual scene::SMesh* load(const  mchar* filename);
	virtual scene::SMesh* load(OS::IStream* stream);
	virtual bool canLoad(const  mchar*filename);
	virtual const core::string&getExtension();

};

}
}


#endif //___acLoader___
