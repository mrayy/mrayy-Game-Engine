

/********************************************************************
	created:	2010/10/15
	created:	15:10:2010   9:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\IResourceHolder.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	IResourceHolder
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef IResourceHolder_h__
#define IResourceHolder_h__

#include "CompileConfig.h"


namespace mray
{
	class IResource;

class MRAY_DLL IResourceHolder
{
private:
protected:

	std::list<IResource*> m_dependendResources;
	std::list<IResourceHolder*> m_dependendResourcesHolders;

	void _AddDependedResource(IResource*res);
	void _RemoveDependedResource(IResource*res);

	void _AddDependedResourceHolders(IResourceHolder*res);
	void _RemoveDependedResourceHolders(IResourceHolder*res);

public:
	IResourceHolder();
	virtual ~IResourceHolder();

	const std::list<IResource*>& GetRequiredResources();
	const std::list<IResourceHolder*>& GetRequiredResourcesHolders();

	virtual void UnuseResources();
	virtual void UseResources();
	virtual void ReloadResources();

};

}
#endif // IResourceHolder_h__
