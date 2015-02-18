

/********************************************************************
	created:	2009/07/27
	created:	27:7:2009   19:58
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\DefaultObjects.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	DefaultObjects
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___DefaultObjects___
#define ___DefaultObjects___

#include "compileConfig.h"
#include "ISingleton.h"
#include "ObjectFactoryManager.h"

namespace mray{

class MRAY_DLL DefaultObjects:public ObjectFactoryManager<IObject>,public ISingleton<DefaultObjects>
{
private:
protected:
public:
	DefaultObjects();
	virtual ~DefaultObjects();

	static void registerDefaultObjects();
	static void removeDefaultObjects();

};

}


#endif //___DefaultObjects___
