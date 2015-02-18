

/********************************************************************
	created:	2010/08/09
	created:	9:8:2010   20:26
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MeshV2Loader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MeshV2Loader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MeshV2Loader_h__
#define MeshV2Loader_h__

#include "IMeshLoader.h"

namespace mray
{
namespace loaders
{

class MeshV2Loader:public IMeshLoader
{
private:
protected:
public:
	MeshV2Loader();
	virtual ~MeshV2Loader();

	virtual scene::SMesh* load(const  core::string& name);
	virtual scene::SMesh* load(OS::IStream* stream);
	virtual bool canLoad(const  core::string&name);
	virtual const core::string&getExtension();
};

}
}
#endif // MeshV2Loader_h__
