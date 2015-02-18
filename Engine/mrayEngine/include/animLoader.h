#ifndef ___animLoader___
#define ___animLoader___

#include "IAnimLoader.h"
#include "TransformationAnimationMixer.h"

namespace mray{
namespace OS{
	class IStream;
}
namespace loaders{

class MRAY_DLL AnimLoader:public IAnimLoader
{
public:
	struct AnimHeader{
		int version;
	};
	struct TrackHeader{
		//char trackName[32];
		int numberOfKeys;
		uchar transType;
		int affectedNodeID;
		ulong catagoryBits;
		ulong priority;
	};

	static const int TRACKGROUP_CHUNK_ID =0x2;
	static const int TRACK_CHUNK_ID =0x3;

protected:

	math::vector3d readTranslate(OS::IStream* file);
	math::vector3d readRotate(OS::IStream* file);
	math::vector3d readScale(OS::IStream* file);

	animation::TransformationAnimationTrack* readTrack(OS::IStream* file,animation::AnimationTrackGroup*owner);
	animation::TransAnimationGroup* readAnimationTrack(OS::IStream* file);
public:
	virtual~AnimLoader(){}
	virtual animation::TransformationAnimationMixer* load(const  core::string& name);
	virtual animation::TransformationAnimationMixer* load(OS::IStream* file);
	virtual bool canLoad(const  core::string& name);
};

};//loaders
};//mray


#endif







