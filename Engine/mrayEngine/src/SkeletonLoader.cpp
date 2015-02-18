#include "stdafx.h"

#include "SkeletonLoader.h"
#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "BoneNode.h"

#include "StringUtil.h"

namespace mray{
namespace loaders{


SkeletonLoader::SkeletonLoader(){
	m_skeleton=0;
}
SkeletonLoader::~SkeletonLoader(){
}


scene::BoneNode* SkeletonLoader::readBone(ChunkSerializer<uchar>::SChunk*c){
	core::string name=m_reader.readString();
	int id;
	c->bytesReaded+=m_reader.read(&id,sizeof(id));
	scene::BoneNode* b=new scene::BoneNode(name,id,m_skeleton,0);

	math::vector3d pos;
//	math::vector3d scale;
	math::quaternion ori;
	c->bytesReaded+= m_reader.read(&pos,sizeof(pos));
	c->bytesReaded+=m_reader.read(&ori,sizeof(ori));
//	c->bytesReaded+=m_reader.read(&scale,sizeof(scale));

	b->setPosition(pos);
	b->setOrintation(ori);
//	b->setScale(scale);

	return b;
}

void SkeletonLoader::readParent(int &b,int &p,ChunkSerializer<uchar>::SChunk*c){
	c->bytesReaded+=m_reader.read(&b,sizeof(b));
	c->bytesReaded+=m_reader.read(&p,sizeof(p));
}


scene::Skeleton* SkeletonLoader::load(const  core::string& filename){
	if(!canLoad(filename))return 0;
	OS::IStreamPtr file=gFileSystem.openFile(filename);
	if(!file->isOpen())return 0;

	return load(file);
}


scene::Skeleton* SkeletonLoader::load(OS::IStream* file){
	uchar ver;
	file->read(&ver,sizeof(ver));
	if(ver!=1){
		gLogManager.log(mT("Skeleton file version unkown: ")+ver,file->getStreamName(),ELL_WARNING);
		return 0;
	}

	m_reader.setStream(file);

	scene::Skeleton* skeleton=new scene::Skeleton(mT(""));

	m_skeleton=skeleton;

	ChunkSerializer<uchar>::SChunk chunk;
	while(!file->eof()){

		ChunkSerializer<uchar>::readChunk(file,&chunk);
		switch (chunk.type)
		{
			case BONE_CHUNK_ID:
			{
				scene::BoneNode* b=readBone(&chunk);
				if(b){
					int id=b->getID();
					skeleton->addBone(b);
					b->setID(id);
				}
			}break;
			case PARENT_CHUNK_ID:
			{
				int b,p;
				readParent(b,p,&chunk);
				if(b>=0 && p>=0){
					scene::BoneNode* bb=skeleton->getBone(b);
					scene::BoneNode* bp=skeleton->getBone(p);
					bp->addChild(bb);
				}
			}break;
			default:
				ChunkSerializer<uchar>::passChunk(file,&chunk);
		}
	}
	gLogManager.log(mT("Skeleton file loaded"),file->getStreamName(),ELL_SUCCESS);

	return skeleton;
}

bool SkeletonLoader::canLoad(const core::string& fileName){
	core::string str=core::StringUtil::ToLower(fileName);
	return strSearch(str.c_str(),mT(".skel"))!=0;
}
bool SkeletonLoader::canLoad(OS::IStream* file){
	core::string str=core::StringUtil::ToLower(file->getStreamName());
	return strSearch(str.c_str(),mT(".skel"))!=0;
}

const core::string& SkeletonLoader::getDescription(){
	static core::string desc=mT("Binary file Skeleton Loader \".skel\"");
	return desc;
}

}
}
