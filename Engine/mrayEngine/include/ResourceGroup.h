

/********************************************************************
	created:	2008/12/26
	created:	26:12:2008   0:22
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\src\ResourceGroup.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\src
	file base:	ResourceGroup
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	this class contains set of Resource Managers that's the engine contains
*********************************************************************/

#ifndef ___ResourceGroup___
#define ___ResourceGroup___


#include "IResourceManager.h"
#include "ISingleton.h"

namespace mray{

typedef std::list<GCPtr<IResourceManager>> ResManagersList;
typedef ResManagersList::iterator ResManagersListIT;

class MRAY_DLL ResourceGroup:public ISingleton<ResourceGroup>
{
	ResManagersList m_managers;

public:

	ResourceGroup();
	virtual~ResourceGroup();

	void addManager(GCPtr<IResourceManager> manager);
	void removeManager(GCPtr<IResourceManager> manager);

	ResManagersListIT begin();
	ResManagersListIT end();

};

}


#endif //___ResourceGroup___
