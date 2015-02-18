
/********************************************************************
	created:	2009/06/23
	created:	23:6:2009   23:40
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SkeletonExporter.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SkeletonExporter
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SkeletonExporter___
#define ___SkeletonExporter___

#include "ISkeletonExporter.h"
#include "StreamWriter.h"

namespace mray{
namespace loaders{

class MRAY_DLL SkeletonExporter:public ISkeletonExporter
{
private:
protected:

#define BONE_CHUNK_ID 0x1
#define PARENT_CHUNK_ID 0x2

	void writeBone(scene::BoneNode* b);
	void writeParent(int&b,int&p);

	OS::StreamWriter m_writer;

public:
	SkeletonExporter();
	virtual~SkeletonExporter();

	virtual void write(scene::Skeleton* s,OS::IStream* file);

	virtual bool canWrite(const core::string& fileName);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
};

}
}


#endif //___SkeletonExporter___
