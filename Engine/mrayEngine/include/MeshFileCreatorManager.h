

/********************************************************************
	created:	2011/02/23
	created:	23:2:2011   15:00
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MeshFileCreatorManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MeshFileCreatorManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MeshFileCreatorManager_h__
#define MeshFileCreatorManager_h__

#include "ISingleton.h"


namespace mray
{
namespace scene
{

	class IMeshFileCreator;
	class MeshRenderableNode;

class MRAY_DLL MeshFileCreatorManager:public ISingleton<MeshFileCreatorManager>
{
private:
protected:
	typedef std::map<core::string,IMeshFileCreator*> CreatorMap;
	CreatorMap m_creators;
public:
	MeshFileCreatorManager();
	virtual ~MeshFileCreatorManager();

	void RegisterCreator(IMeshFileCreator*c);
	void RemoveCreator(const core::string& type);

	MeshRenderableNode* LoadFromFile(const core::string&path,bool instanced);
	
};

}
}

#endif // MeshFileCreatorManager_h__
