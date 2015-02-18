
/********************************************************************
	created:	2009/06/23
	created:	23:6:2009   23:14
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\SkeletonLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	SkeletonLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___SkeletonLoader___
#define ___SkeletonLoader___

#include "ISkeletonLoader.h"
#include "StreamReader.h"
#include "ChunkSerializer.h"

namespace mray{
namespace loaders{

class MRAY_DLL SkeletonLoader:public ISkeletonLoader
{
private:

#define BONE_CHUNK_ID 0x1
#define PARENT_CHUNK_ID 0x2

	uchar readChunkID();

	scene::BoneNode* readBone(ChunkSerializer<uchar>::SChunk*c);
	void readParent(int&b,int&p,ChunkSerializer<uchar>::SChunk*c);

	OS::StreamReader m_reader;

	scene::Skeleton* m_skeleton;

public:
	SkeletonLoader();
	virtual~SkeletonLoader();

	virtual scene::Skeleton* load(const  core::string& name);
	virtual scene::Skeleton* load(OS::IStream* file);

	virtual bool canLoad(const core::string& fileName);
	virtual bool canLoad(OS::IStream* file);

	virtual const core::string& getDescription();
};

}
}


#endif //___SkeletonLoader___
