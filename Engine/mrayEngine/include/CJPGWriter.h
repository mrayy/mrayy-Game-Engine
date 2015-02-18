
#ifndef ___CJPGWriter___
#define ___CJPGWriter___


#include "ITexWriter.h"
#include "ITexture.h"

namespace mray{
namespace loaders{

class MRAY_DLL CJPGWriter:public ITexWriter
{
	static const core::string m_typeExt;
	static const core::string m_typeDesc;
public:
	CJPGWriter();
	virtual bool write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target);
	virtual bool canWrite(const core::string&filename);
	virtual bool support(video::ETextureType type);
	virtual const core::string& getExt(){
		return m_typeExt;
	}
	virtual const core::string& getDescription(){
		return m_typeDesc;
	}
};
};//loaders
};//mray


#endif











