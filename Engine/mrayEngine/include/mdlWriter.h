

/********************************************************************
	created:	2012/02/11
	created:	11:2:2012   16:37
	filename: 	d:\Development\mrayEngine\Engine\mrayEngine\mdlWriter.h
	file path:	d:\Development\mrayEngine\Engine\mrayEngine
	file base:	mdlWriter
	file ext:	h
	author:		MHD Yamen Saraiji
	
	purpose:	
*********************************************************************/


#ifndef __mdlWriter__
#define __mdlWriter__

#include "IModelWriter.h"

namespace mray
{
namespace scene
{
	class Skeleton;
	class BoneNode;
}
namespace loaders
{

class MRAY_DLL mdlWriter:public IModelWriter
{
private:
protected:
	static const core::string m_extension;
	static const core::string m_description;

	void WriteBone(scene::BoneNode* bone,OS::IStream* stream);
	void WriteSkeleton(scene::Skeleton*skeleton,OS::IStream* stream);
public:
	mdlWriter();
	virtual~mdlWriter();

	virtual bool write(scene::SMesh*m,OS::IStream* stream);

	virtual bool canWrite(const core::string& fileName);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
};


}
}

#endif

