
#ifndef ___CBMPWRITER___
#define ___CBMPWRITER___

#include "ITexWriter.h"

namespace mray{
namespace loaders{

class MRAY_DLL CBmpWriter:public ITexWriter
{
	static const core::string m_typeExt;
	static const core::string m_typeDesc;
public:
	CBmpWriter();
	virtual bool write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target);
	virtual bool canWrite(const  core::string&filename);
	virtual const core::string& getExt();

	virtual const core::string& getDescription();
	virtual bool support(video::ETextureType type);
};
};//loaders
};//mray


#endif








