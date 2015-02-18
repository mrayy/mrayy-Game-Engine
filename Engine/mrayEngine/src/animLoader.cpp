#include "stdafx.h"


#include "AnimLoader.h"
#include "IStream.h"
#include "ILogManager.h"
#include "IFileSystem.h"
#include "IStream.h"
#include "TransAnimationGroup.h"
#include "ChunkSerializer.h"

#include "TransformationAnimationTrack.h"
#include "StreamReader.h"

#include "StringUtil.h"

namespace mray{
namespace loaders{

math::vector3d AnimLoader::readTranslate(OS::IStream* file){
	math::vector3d v;
	file->read(&v,sizeof(v));
	return v;
}
math::vector3d AnimLoader::readRotate(OS::IStream* file){
	math::vector3d v;
	file->read(&v,sizeof(v));
	return v;
}
math::vector3d AnimLoader::readScale(OS::IStream* file){
	math::vector3d v;
	file->read(&v,sizeof(v));
	return v;
}

animation::TransformationAnimationMixer* AnimLoader::load(const  core::string& filename){
	if(!canLoad(filename))return 0;
	OS::IStream* file=gFileSystem.openFile(filename);
	if(!file || !file->isOpen())return 0;

	return load(file);
}


animation::TransformationAnimationTrack* AnimLoader::readTrack(OS::IStream* file,animation::AnimationTrackGroup*owner){

	animation::TransformationAnimationTrack* ttAT;
	OS::StreamReader reader(file);


	TrackHeader theader;
	core::stringc nameStr= reader.binReadStringC();
	file->read(&theader,sizeof(theader));
	animation::KeyAnimationTrack* track;
	core::string name;
	core::char_to_string(nameStr.c_str(),name);
	switch (theader.transType)
	{
	case 0://Translate
	case 1://Rotate
	case 2://Scale
	case 3://Transformation
		track=ttAT=new animation::TransformationAnimationTrack(name,theader.affectedNodeID,theader.catagoryBits,theader.priority,owner);
		break;
	default:
		gLogManager.log(mT("Transformation Type unkown: ")+core::StringConverter::toString(theader.transType),file->getStreamName(),ELL_WARNING);
		return 0;

	}
	math::vector3d tempV;
	for (int j=0;j<theader.numberOfKeys;j++)
	{
		float time=0;
		file->read(&time,sizeof(time));
		switch (theader.transType)
		{
		case 0://Translate
			tempV=readTranslate(file);
			ttAT->createKeyFrame(time,tempV,1,math::quaternion::Identity);
			break;
		case 1://Rotate
			tempV=readRotate(file);
			ttAT->createKeyFrame(time,math::vector3d::Zero,1,math::quaternion(tempV));
			break;
		case 2://Scale
			tempV=readScale(file);
			ttAT->createKeyFrame(time,math::vector3d::Zero,tempV,math::quaternion::Identity);
			break;
		case 3://Transformation
			{
				math::vector3d p=readTranslate(file);
				math::vector3d r=readRotate(file);
				math::vector3d s=readScale(file);
				ttAT->createKeyFrame(time,p,s,r);
			}
			break;
		}
	}
	return ttAT;
}

animation::TransAnimationGroup* AnimLoader::readAnimationTrack(OS::IStream* file)
{
	animation::TransAnimationGroup* group=new animation::TransAnimationGroup(mT(""));

	ChunkSerializer<uchar>::SChunk chunk;

	OS::StreamReader reader(file);
	core::string gname;
	core::string nameStr;
	nameStr=core::char_to_string(reader.binReadStringC().c_str());
	gname=nameStr;
	group->setName(gname);
	while(!file->eof())
	{
		ChunkSerializer<uchar>::readChunk(file,&chunk);
		switch (chunk.type)
		{
		case TRACK_CHUNK_ID:
			{
				animation::KeyAnimationTrack* track=readTrack(file,group);
				if(track)
					group->addTrack((animation::TransformationAnimationTrack*)track);
			}break;
		}
	}
	return group;
}

animation::TransformationAnimationMixer* AnimLoader::load(OS::IStream* file){
	AnimHeader header;
	file->read(&header,sizeof(header));
	if(header.version!=1){
		gLogManager.log(mT("Animation file version unkown: ")+header.version,file->getStreamName(),ELL_WARNING);
		return 0;
	}


	animation::TransformationAnimationMixer* mixer=new animation::TransformationAnimationMixer();

	ChunkSerializer<uchar>::SChunk chunk;

	while(!file->eof())
	{
		ChunkSerializer<uchar>::readChunk(file,&chunk);
		switch (chunk.type)
		{
		case TRACKGROUP_CHUNK_ID:
			{
				animation::AnimationTrackGroup* group=readAnimationTrack(file);
				mixer->addTrackGroup((animation::TransAnimationGroup* )group);
			}break;
		}
	}

	gLogManager.log(mT("Animation file loaded"),file->getStreamName(),ELL_SUCCESS);
	return mixer;


}

bool AnimLoader::canLoad(const  core::string&name)
{
	core::string str=core::StringUtil::ToLower(name);
	return strSearch(str.c_str(),mT(".anim"))!=0;
}






}
}
