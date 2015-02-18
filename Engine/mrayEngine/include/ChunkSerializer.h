
/********************************************************************
	created:	2009/06/29
	created:	29:6:2009   11:02
	filename: 	i:\Programing\GameEngine\mrayEngine\mrayEngine\include\ChunkLoader.h
	file path:	i:\Programing\GameEngine\mrayEngine\mrayEngine\include
	file base:	ChunkLoader
	file ext:	h
	author:		Mohamad Yamen Saraiji
	
	purpose:	
*********************************************************************/

#ifndef ___ChunkSerializer___
#define ___ChunkSerializer___

#include "IStream.h"

namespace mray{
namespace loaders{

template <class T>
class MRAY_DLL ChunkSerializer
{
private:
protected:
public:
	struct SChunk
	{
		T type;
		uint length;
		uint bytesReaded;
	};

	
	static void writeChunk(OS::IStream* stream, T type,int length){
		stream->write(&type,sizeof(type));
		stream->write(&length,sizeof(length));
	}
	static void readChunk(OS::IStream* stream,SChunk*c){

		c->bytesReaded= stream->read(&c->type,sizeof(c->type));
		c->bytesReaded+=stream->read(&c->length,sizeof(c->length));
	}


	static void passChunk(OS::IStream* stream,SChunk*c){
		stream->seek(c->length-c->bytesReaded,OS::ESeek_Cur);
		c->bytesReaded=c->length;
	}

};

}
}


#endif //___ChunkSerializer___
