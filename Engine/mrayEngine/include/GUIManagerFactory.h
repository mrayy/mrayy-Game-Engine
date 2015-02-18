

/********************************************************************
	created:	2011/01/02
	created:	2:1:2011   22:54
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\GUIManagerFactory.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	GUIManagerFactory
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef GUIManagerFactory_h__
#define GUIManagerFactory_h__

#include "ObjectFactoryManager.h"

namespace mray
{
namespace GUI
{

	class IGUIManager;

class GUIManagerFactory:public ObjectFactoryManager<IGUIManager>
{
private:
protected:
public:
	GUIManagerFactory()
	{
	}
	virtual ~GUIManagerFactory()
	{
	}	
};

}
}

#endif // GUIManagerFactory_h__

