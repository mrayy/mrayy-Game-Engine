



/********************************************************************
	created:	2014/05/14
	created:	14:5:2014   18:38
	filename: 	F:\Development\mrayEngine\Engine\mrayEngine\AImpSceneLoader.h
	file path:	F:\Development\mrayEngine\Engine\mrayEngine
	file base:	AImpSceneLoader
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef __AImpSceneLoader__
#define __AImpSceneLoader__


#include "compileconfig.h"
#include "SMesh.h"
#include "IMeshLoader.h"
#include "IStream.h"

namespace mray
{
namespace loaders
{
	class MeshLoader;
//Assest Importer plugin
class MRAY_DLL AImpSceneLoader :public IMeshLoader
{
protected:
	core::string m_ext;
public:
	AImpSceneLoader(const core::string& ext);
	virtual ~AImpSceneLoader();

	virtual scene::SMesh* load(const  core::string& name);
	virtual scene::SMesh* load(OS::IStream* stream);
	virtual bool canLoad(const  core::string& name);
	const core::string&getExtension();

	static void InitImporters(loaders::MeshLoader* l);
	static void DestroyImporter();
};

}
}

#endif
