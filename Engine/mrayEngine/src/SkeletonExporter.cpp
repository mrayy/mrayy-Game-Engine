#include "stdafx.h"

#include "SkeletonExporter.h"
#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "ChunkSerializer.h"
#include "BoneNode.h"

#include "StringUtil.h"

namespace mray{
namespace loaders{


void SkeletonExporter::writeBone(scene::BoneNode* b){

	static uint staticLength=sizeof(int)+sizeof(math::vector3d)+sizeof(math::quaternion);

	uint dynLen=b->getNodeName().length()+1;

	ChunkSerializer<uchar>::writeChunk(m_writer.getStream(),BONE_CHUNK_ID,
		staticLength+dynLen);

	m_writer.writeString(b->getNodeName());
	int id=b->getID();
	m_writer.write(&id,sizeof(id));

	math::vector3d pos=b->getPosition();
	math::quaternion ori=b->getOrintation();
//	math::vector3d scale=b->getScale();

	m_writer.write(&pos,sizeof(pos));
	m_writer.write(&ori,sizeof(ori));
//	m_writer.write(&scale,sizeof(scale));

}
void SkeletonExporter::writeParent(int&b,int&p){
	ChunkSerializer<uchar>::writeChunk(m_writer.getStream(),PARENT_CHUNK_ID,
		2*sizeof(int));

	m_writer.write(&b,sizeof(b));
	m_writer.write(&p,sizeof(p));
}

SkeletonExporter::SkeletonExporter(){
}
SkeletonExporter::~SkeletonExporter(){
}

void SkeletonExporter::write(scene::Skeleton* s,OS::IStream* file){
	if(!file)
		return;

	uchar ver=1;

	m_writer.setStream(file);

	m_writer.write(&ver,sizeof(ver));


	const scene::BonesList& bones= s->getBones();
	scene::BonesList::const_iterator begin= bones.begin();
	scene::BonesList::const_iterator it;

	for(it=begin;it!=bones.end();++it){
		writeBone(*it);
	}

	for(it=begin;it!=bones.end();++it){
		scene::BoneNode* b=*it;

		int bi,pi;
		if(b->getParent()){
			bi=b->getID();
			pi=b->getParent()->getID();
			writeParent(bi,pi);
		}
	}

	gLogManager.log(mT("Skeleton file exported"),file->getStreamName(),ELL_SUCCESS);
}

bool SkeletonExporter::canWrite(const core::string& fileName){
	core::string str=core::StringUtil::ToLower(fileName);
	return strSearch(str.c_str(),mT(".skel"))!=0;
}
const core::string& SkeletonExporter::getExt(){
	static core::string ext=mT("skel");
	return ext;
}

const core::string& SkeletonExporter::getDescription(){
	static core::string desc=mT("Binary file Skeleton exporter \".skel\"");
	return desc;
}

}
}
