
#ifndef ___CTARGAWRITER___
#define ___CTARGAWRITER___

#include "ITexWriter.h"


namespace mray{
namespace video{
	class ITexture;
}
namespace loaders{

class MRAY_DLL CTargaWriter:public ITexWriter
{
	void writeCompressedRow(OS::IStream* file,video::ImageInfo*tex);

	static const core::string m_typeExt;
	static const core::string m_typeDesc;
public:
	CTargaWriter();
	virtual bool write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target);
	virtual bool canWrite(const  core::string&filename);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
	virtual bool support(video::ETextureType type);
};

}
}


#endif








