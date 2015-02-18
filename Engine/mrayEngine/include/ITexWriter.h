


#ifndef ___ITEXWRITER
#define ___ITEXWRITER

#include "ImageInfo.h"
#include "IFileWriter.h"
#include "GCPtr.h"

namespace mray{
namespace loaders{

class ITexWriter:public IFileWriter
{
protected:
public:
	ITexWriter(){};
	virtual~ITexWriter(){};

	virtual bool write(video::ImageInfo*tex,OS::IStream* stream,video::ETextureType target)=0;
	virtual bool support(video::ETextureType type)=0;
};
};//loaders
};//mray

#endif





