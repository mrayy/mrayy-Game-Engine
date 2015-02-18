

#ifndef ___MDLLOADER___
#define ___MDLLOADER___

#include "SMesh.h"
#include "IMeshLoader.h"
#include "IStream.h"

namespace mray{
	namespace scene
	{
		class ISkinCluster;
		class Skeleton;
		class BoneNode;
	}
namespace loaders{


class MRAY_DLL mdlLoader:public IMeshLoader
{
	std::vector<ushort> m_tmpInd;
	std::vector<math::Point3di> m_tmpTri;

	char tmpStr[32];
	core::string m_tmpStr;

	OS::IStreamPtr m_stream;
	OS::IStreamPtr outStream;

	bool loadWeights(scene::SMesh*mesh,scene::Skeleton*skeleton);
	scene::BoneNode* loadJoint(scene::Skeleton*skeleton,scene::BoneNode*parent);
	scene::Skeleton* readSkeleton();
public:
	mdlLoader(){};
	virtual scene::SMesh* load(const  core::string& name);
	virtual scene::SMesh* load(OS::IStream* stream);
	virtual bool canLoad(const  core::string& name);
	const core::string&getExtension();
};
};//loaders
};//mray

#endif