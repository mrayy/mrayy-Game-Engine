#include "stdafx.h"




#include "CustomTexWriter.h"
#include "IStream.h"
#include "StringUtil.h"

namespace mray{
namespace loaders{


const core::string CustomTexWriter::m_typeExt=mT("mtex");
const core::string CustomTexWriter::m_typeDesc=mT("mtex Image Writer");

CustomTexWriter::CustomTexWriter()
{
}

bool CustomTexWriter::write(video::ImageInfo*tex,OS::IStream* file,video::ETextureType target)
{


	file->write(&target,sizeof(target));
	file->write(&tex->Size,sizeof(tex->Size));
	file->write(&tex->format,sizeof(tex->format));

	file->write(tex->imageData,tex->imageDataSize);

	return 1;

} 

bool CustomTexWriter::canWrite(const  core::string&filename)
{
	core::string fname=core::StringUtil::ToLower(filename);
	if(strSearch(fname.c_str(),mT(".mtex"))!=0)return 1;
	return 0;
}
const core::string& CustomTexWriter::getExt(){
	return m_typeExt;
}

const core::string& CustomTexWriter::getDescription(){
	return m_typeDesc;
}




bool CustomTexWriter::support(video::ETextureType type)
{
	return true;
}



}
}

