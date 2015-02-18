
#include "stdafx.h"
#include "animWriter.h"
#include "AnimLoader.h"

#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "IStream.h"
#include "TransAnimationGroup.h"
#include "ChunkSerializer.h"
#include "StreamWriter.h"

#include "TransformationAnimationTrack.h"
#include "TransAnimationGroup.h"

#include "StringUtil.h"

namespace mray{
namespace loaders{


bool AnimWriter::write(animation::TransformationAnimationMixer*m,const  core::string& filename){
	if(!canWrite(filename))return 0;
	OS::IStream* file=gFileSystem.openFile(filename,OS::BIN_WRITE);
	if(!file || !file->isOpen())return 0;

	bool ret= write(m,file);
	file->close();
	return ret;
}


void AnimWriter::writeTrack(animation::TransformationAnimationTrack* track,OS::IStream* file)
{

	ChunkSerializer<uchar>::writeChunk(file,AnimLoader::TRACK_CHUNK_ID,0);
	OS::StreamWriter writer(file);


	AnimLoader::TrackHeader theader;
	core::stringc nameStr;
	core::string_to_char(track->getName(),nameStr);

	writer.binWriteString(nameStr);
	const animation::KeyAnimationTrack::KeyFrameList& keyFrames=track->getKeyFrames();
	theader.affectedNodeID=track->getAffectedNodeID();
	theader.catagoryBits=track->getCatagoryBits();
	theader.numberOfKeys=keyFrames.size();
	theader.priority=track->getPriority();
	theader.transType=3;
	file->read(&theader,sizeof(theader));
	math::vector3d tempV;
	for (int j=0;j<keyFrames.size();j++)
	{
		const  animation::TransformKeyFrame* kf=dynamic_cast<const animation::TransformKeyFrame*>(keyFrames[j].getKeyFrame());
		if(!kf)
			continue;
		float time=keyFrames[j].getTime();
		file->write(&time,sizeof(time));

		file->write(&kf->getPos(),sizeof(math::vector3d));
		file->write(&kf->getOrintation(),sizeof(math::quaternion));
		file->write(&kf->getScale(),sizeof(math::vector3d));
	}
}

void AnimWriter::writeAnimationTrack(animation::TransAnimationGroup* group,OS::IStream* file)
{
	ChunkSerializer<uchar>::writeChunk(file,AnimLoader::TRACKGROUP_CHUNK_ID,0);
	OS::StreamWriter writer(file);

	core::stringc nameStr;
	core::string_to_char(group->getName(),nameStr);

	writer.binWriteString(nameStr);
	
	animation::IAnimationTrackList::const_iterator tracks=group->getTracks().begin();
	animation::IAnimationTrackList::const_iterator end=group->getTracks().end();
	for(;tracks!=end;++tracks)
	{
		animation::TransformationAnimationTrack* t=dynamic_cast<animation::TransformationAnimationTrack*>(*tracks);
		if(t)
			writeTrack(t,file);
	}
}

bool AnimWriter::write(animation::TransformationAnimationMixer*m,OS::IStream* file){
	AnimLoader::AnimHeader header;
	header.version=1;
	file->write(&header,sizeof(header));


	animation::IAnimationMixer::ATGroupList::const_iterator tracks=m->getTrackGroups().begin();
	animation::IAnimationMixer::ATGroupList::const_iterator end=m->getTrackGroups().end();
	for(;tracks!=end;++tracks)
	{
		animation::TransAnimationGroup* t= dynamic_cast<animation::TransAnimationGroup*>((*tracks).second);
		if(!t)
			continue;
		writeAnimationTrack(t,file);
		
	}

	return true;
}

bool AnimWriter::canWrite(const  core::string&name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".anim"))!=0;
}






}
}
