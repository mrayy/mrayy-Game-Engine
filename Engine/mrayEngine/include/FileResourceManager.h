
/********************************************************************
	created:	2009/01/28
	created:	28:1:2009   0:08
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\FileResourceManager.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	FileResourceManager
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___FileResourceManager___
#define ___FileResourceManager___


#include "IResourceManager.h"
#include "ISingleton.h"
#include "IStream.h"

namespace mray{
class MRAY_DLL FileResourceManager:public IResourceManager,public ISingleton<FileResourceManager>
{
protected:
	virtual IResourcePtr createInternal(const core::string& name);

	virtual IResourcePtr loadResourceFromFile(OS::IStream* file);

	virtual core::string getDefaultGroup();
public:
	FileResourceManager();
	virtual~FileResourceManager();

	OS::IStreamPtr getFile(const core::string&name,OS::FILE_MODE mode=OS::BIN_READ);

	virtual void writeResourceToDist(const core::string&resName,const core::string&fileName);
	virtual void writeResourceToDist(const IResourcePtr& resource,const core::string&fileName);
	virtual void writeResourceToDist(ResourceHandle resource,const core::string&fileName);

};
#define gFileResourceManager mray::FileResourceManager::getInstance()

}



#endif //___FileResourceManager___

