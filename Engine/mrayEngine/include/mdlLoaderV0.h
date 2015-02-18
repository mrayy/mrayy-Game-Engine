


/********************************************************************
	created:	2012/02/11
	created:	11:2:2012   19:45
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\include\mdlLoaderV0.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine\include
	file base:	mdlLoaderV0
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __mdlLoaderV0__
#define __mdlLoaderV0__


#include "SMesh.h"
#include "IMeshLoader.h"
#include "IStream.h"

namespace mray
{
	namespace scene
	{
		class ISkinCluster;
		class Skeleton;
		class BoneNode;
	}
namespace loaders
{

class MRAY_DLL mdlLoaderV0:public IMeshLoader
{
private:
protected:
	static const core::string m_extension;
	static const core::string m_description;

	scene::BoneNode* LoadBone(scene::Skeleton*skeleton,scene::BoneNode*parent,OS::IStream* stream);
	scene::Skeleton* LoadSkeleton(OS::IStream* stream);
public:
	mdlLoaderV0();
	virtual~mdlLoaderV0();

	virtual scene::SMesh* load(const  core::string& name);
	virtual scene::SMesh* load(OS::IStream* stream);
	virtual bool canLoad(const  core::string& name);
	const core::string&getExtension();

};

}
}

#endif

