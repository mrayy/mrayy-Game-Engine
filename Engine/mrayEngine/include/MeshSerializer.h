


/********************************************************************
	created:	2010/08/08
	created:	8:8:2010   21:31
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\MeshSerializer.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	MeshSerializer
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef MeshSerializer_h__
#define MeshSerializer_h__

#include "CompileConfig.h"

namespace mray
{
	namespace OS
	{
		class IStream;
		class StreamWriter;
		class StreamReader;
	}
	namespace video
	{
		class IHardwareStreamBuffer;
		struct MeshStreamKey;
	}
namespace scene
{

	class SMesh;
	class MeshBufferData;
	class ISkinCluster;
	class Skeleton;
	class BoneNode;
	class IMeshBuffer;

class MRAY_DLL MeshSerializer
{
private:
protected:
	static const char HeaderMagicNumber[4];
	static const int CurrentMeshVersion;

	BoneNode* _readBone(Skeleton*sk,OS::StreamReader*reader);
	Skeleton* _readSkeleton(OS::StreamReader*reader);
	ISkinCluster* _readSkinCluster(SMesh*mesh,OS::StreamReader*reader);
	void _readBufferData(SMesh*mesh,MeshBufferData*data,OS::StreamReader*reader);
	void _readStream(IMeshBuffer*buff,OS::StreamReader*reader);

	void _writeBone(BoneNode*bone,OS::StreamWriter*writer);
	void _writeSkeleton(Skeleton*sk,OS::StreamWriter*writer);
	void _writeSkinCluster(SMesh*mesh,ISkinCluster*c,OS::StreamWriter*writer);
	void _writeBufferData(SMesh*mesh,MeshBufferData*buff,OS::StreamWriter*writer);
	void _writeStream(const video::MeshStreamKey*key, video::IHardwareStreamBuffer*stream,OS::StreamWriter*writer);
public:
	MeshSerializer();
	virtual ~MeshSerializer();
	
	SMesh* Deserialize(OS::IStream*stream);
	void Serialize(SMesh*mesh,OS::IStream*stream);
};

}
}
#endif // MeshSerializer_h__
